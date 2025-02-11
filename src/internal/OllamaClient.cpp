/**
 * @file BumbleOllamaClient.cpp
 */

#include "OllamaClient.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "AIClient_p.h"
#include "LLMOutputProcessor.h"

namespace BumbleAIBridge {
namespace Internal {

/**
 * @brief Private implementation for OllamaClient, inheriting from AIClientPrivate.
 */
class OllamaClientPrivate : public AIClientPrivate {
public:
    using AIClientPrivate::AIClientPrivate;  // Inherit constructor

    bool done = false;          // final JSON object received
    QByteArray incompleteData;  // buffers incomplete data
    QString responseText;       // buffers response answer
    QString parseError;

    QJsonArray currentContext;  // accumulates user promts and assistant response to keep context

    struct {
        QString role;
        QString content;
        void clear() { role.clear(); content.clear(); }
    } incompleteResponseMessage;       // use to concatenate multiple sequence messages with same role


    void setParseError(const QString& errorString) {
        qWarning() << errorString;
        parseError = errorString;
    }


    QNetworkRequest createRequest(const QString& apiEndpoint)
    {
        QNetworkRequest request;

        request.setUrl(QUrl(config.serverUrl + apiEndpoint));

        return request;
    }


    void cleanupReply()
    {
        activeReply->deleteLater(); // Clean up the reply
        activeReply.clear();
    }
};


OllamaClient::OllamaClient(const AIConfig& config, QObject* parent)
    : AIClient(new OllamaClientPrivate(config), parent) {}

OllamaClient::~OllamaClient() = default;


void OllamaClient::resetContext() {
    Q_D(OllamaClient);
    d->currentContext = QJsonArray();
}

void OllamaClient::appendMessageToContext(const QJsonObject& message)
{
    Q_D(OllamaClient);

    d->currentContext.append( message );
    emit contextExtended(message);
}

bool OllamaClient::handleOneJsonObject(const QByteArray& data)
{
    Q_D(OllamaClient);

    QJsonParseError parseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        d->setParseError( "JSON parsing error: " + parseError.errorString());
        return false;
    }
    QJsonObject jsonObj = jsonResponse.object();

    if (jsonObj.contains("error") && jsonObj["error"].isString()) {
        d->setParseError( jsonObj["error"].toString() );
        return false;
    }

    // /api/generate
    if (jsonObj.contains("response") && jsonObj["response"].isString()) {
        d->responseText += jsonObj["response"].toString();
    }

    // /api/chat
    QString role;
    QString content;
    if (jsonObj.contains("message") && jsonObj["message"].isObject()) {
        QJsonObject messageObj = jsonObj["message"].toObject();
        role = messageObj["role"].toString();
        content = messageObj["content"].toString();
        if (!role.isEmpty() && !content.isEmpty()) {
            emit messageReceived(role, content);
        }
    }

    if (jsonObj["done"].toBool(false)) {
        d->done = true;
    }

    // for /api/chat
    if (!d->incompleteResponseMessage.role.isEmpty() && (d->done || role != d->incompleteResponseMessage.role)) {
        qDebug() << d->incompleteResponseMessage.role;

        if (d->incompleteResponseMessage.role == "assistant") {
            QJsonObject message{{"role", "assistant"}, {"content", d->incompleteResponseMessage.content}};
            appendMessageToContext(message);
        }

        d->incompleteResponseMessage.role.clear();
        d->incompleteResponseMessage.content.clear();
    }

    if (d->incompleteResponseMessage.role.isEmpty()) {
        d->incompleteResponseMessage.role = role;
    }
    d->incompleteResponseMessage.content += content;


    return true;
}






void OllamaClient::onReadyRead() {
    Q_D(OllamaClient);

    if (!d->activeReply) {
        return;
    }

    // Read all available data and append it to our persistent buffer.
    QByteArray newData = d->activeReply->readAll();
    d->incompleteData.append(newData);
    if (d->incompleteData.isEmpty()) {
        return;
    }

    int startIndex = -1;   // Marks the beginning of a JSON object.
    int braceCount = 0;    // Tracks the balance of '{' and '}'.
    int processedUpTo = 0; // Index up to which data has been successfully processed.

    // Iterate over the complete buffer.
    for (int i = 0; i < d->incompleteData.size(); ++i) {
        char ch = d->incompleteData.at(i);

        if (ch == '{') {
            // If we're not already inside a JSON object, mark this as the start.
            if (startIndex == -1)
                startIndex = i;
            braceCount++;
        } else if (ch == '}') {
            // If there is no matching '{', we have an error (negative brace count).
            if (braceCount <= 0) {
                qDebug() << "Unmatched closing brace encountered at index" << i
                         << ". Discarding data up to this point.";
                // Discard everything up to and including this unmatched brace.
                processedUpTo = i + 1;
                // Reset our state.
                startIndex = -1;
                braceCount = 0;
                // Continue scanning from the next character.
                continue;
            } else {
                braceCount--;
            }
        }

        // If braces are balanced and we had started a JSON object,
        // we assume we have a complete JSON message.
        if (braceCount == 0 && startIndex != -1) {
            QByteArray jsonData = d->incompleteData.mid(startIndex, i - startIndex + 1);

            // Mark the data up to i+1 as processed.
            processedUpTo = i + 1;
            // Reset state to be ready for the next JSON object.
            startIndex = -1;

            if (!handleOneJsonObject(jsonData)) {
                // Exit loop on error
                break;
            }
        }
    }

    // Discard the data that has been processed. Any remaining bytes represent an
    // incomplete JSON object (or trailing invalid data) and are kept for the next read.
    if (processedUpTo > 0) {
        d->incompleteData = d->incompleteData.mid(processedUpTo);
    }

    if (!d->parseError.isEmpty()) {
        d->activeReply->abort();    // QNetworkReply::finished() emitted
        // error will be emitted in QNetworkReply::finished() slot
    }
}


void OllamaClient::handleFinishedError() {
    Q_D(OllamaClient);

    QString errorString = !d->parseError.isEmpty() ? d->parseError : d->activeReply->errorString();
    d->parseError = QString();

    d->cleanupReply();

    emit errorOccurred(errorString);
}



OperationResult OllamaClient::handleSpecificRequest(const QString& prompt, const QString& system, const QList<QByteArray>& imagesBase64) {
    Q_D(OllamaClient);

    QString apiEndpoint;


    QJsonObject json;
    json["model"] = d->config.modelName;
    if (d->config.isStreamingMode) {
        // Generate the next message in a chat with a provided model.
        apiEndpoint = "/api/chat";

        QJsonObject promptMessage{{"role", "user"}, {"content", prompt}};
        if (!imagesBase64.isEmpty()) {
            QJsonArray imagesArray;
            for(const auto& base64 : imagesBase64) {
                imagesArray += QString::fromLatin1(base64);
            }
            promptMessage["images"] = imagesArray;
        }

        // store promt message in context for next handleSpecificRequest()
        appendMessageToContext( promptMessage );

        QJsonArray messages = d->currentContext;

        // Ollama does not allow modifying the system message dynamically within an ongoing conversation (persistent context).
        // Instead, you can change it per request by sending a new system message at the start of the messages array.
        // That's why system is not added to d->currentContext
        if (!system.isEmpty()) {
            QJsonObject systemMessage{{"role", "system"}, {"content", system}};
            messages.prepend( systemMessage );
        }

        json["messages"] = messages;
    } else {
        // The /api/generate endpoint was initially designed for single-turn interactions
        apiEndpoint = "/api/generate";
        json["prompt"] = prompt;
        json["stream"] = false;
        if (!system.isEmpty()) {
            json["system"] = system;
        }
        if (!imagesBase64.isEmpty()) {
            QJsonArray imagesArray;
            for(const auto& base64 : imagesBase64) {
                imagesArray += QString::fromLatin1(base64);
            }
            json["images"] = imagesArray;
        }
    }

    qDebug() << json;


    d->done = false;
    d->incompleteData = QByteArray();
    d->responseText = QString();
    d->parseError = QString();
    d->incompleteResponseMessage.role = QString();
    d->incompleteResponseMessage.content.clear();

    QNetworkRequest request = d->createRequest(apiEndpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    d->activeReply = d->networkManager.post(request, QJsonDocument(json).toJson());


    QObject::connect(d->activeReply, &QNetworkReply::readyRead, this, &OllamaClient::onReadyRead);

    QObject::connect(d->activeReply, &QNetworkReply::finished, this, [this]() {
        Q_D(OllamaClient);

        if (d->activeReply->error() == QNetworkReply::NoError) {
            onReadyRead();

            if (d->done) {
                if (!d->responseText.isEmpty()) {
                    d->responseText = LLMOutputProcessor(d->config.modelName).processInput(d->responseText);
                    emit responseReceived(d->responseText);
                }
                d->cleanupReply();
                emit responseDone();
            }
        } else {
            handleFinishedError();
        }
    });

    return OperationResult::success();
}



OperationResult OllamaClient::handleListModelsRequest()
{
    Q_D(OllamaClient);

    QNetworkRequest request = d->createRequest("/api/tags");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    d->activeReply = d->networkManager.get(request);

    QObject::connect(d->activeReply, &QNetworkReply::finished, this, [this]() {
        Q_D(OllamaClient);

        if (d->activeReply->error() == QNetworkReply::NoError) {
            QByteArray responseData = d->activeReply->readAll();
            QJsonParseError parseError;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);
            QStringList modelNames;

            if (parseError.error == QJsonParseError::NoError && jsonResponse.isObject()) {
                QJsonObject jsonObj = jsonResponse.object();
                if (jsonObj.contains("models") && jsonObj["models"].isArray()) {
                    QJsonArray modelsArray = jsonObj["models"].toArray();
                    for (const QJsonValue& modelValue : modelsArray) {
                        if (modelValue.isObject()) {
                            QJsonObject modelObj = modelValue.toObject();
                            if (modelObj.contains("name")) {
                                modelNames.append(modelObj["name"].toString());
                            }
                        }
                    }
                }

                if (!modelNames.isEmpty()) {
                    emit modelNamesReceived(modelNames);
                }

                d->cleanupReply();
                emit responseDone();
                return;
            } else {
                d->parseError = QString("JSON parsing error: " + parseError.errorString());
            }
        }
        Q_ASSERT(d->activeReply->error() != QNetworkReply::NoError);
        handleFinishedError();
    });

    return OperationResult::success();

}


} // namespace Internal
} // namespace BumbleAIBridge
