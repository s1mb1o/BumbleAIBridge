/**
 * @file BumbleOllamaClient.cpp
 */

#include "OllamaClient.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
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

    QJsonValue currentContext;
};


OllamaClient::OllamaClient(const AIConfig& config, QObject* parent)
    : AIClient(new OllamaClientPrivate(config), parent) {}

OllamaClient::~OllamaClient() = default;

void OllamaClient::handleReply(QNetworkReply* reply) {
    Q_D(OllamaClient);

    if (reply->error() == QNetworkReply::NoError) {
        const QByteArray responseData = reply->readAll();
        
        bool done = false;      // final JSON object received
        QJsonObject jsonObj;    // final JSON object

        QString responseText;   // assembled response text from all JSON objects

        // Parse JSON lines individually and add response text to responseText
        for (const QByteArray& line : responseData.split('\n')) {
            if (line.trimmed().isEmpty()) continue;

            QJsonParseError parseError;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(line, &parseError);
            if (parseError.error != QJsonParseError::NoError) {
                emit errorOccurred("JSON parsing error: " + parseError.errorString());
                reply->deleteLater();
                return;
            }
            jsonObj = jsonResponse.object();
            
            if (jsonObj.contains("error") && jsonObj["error"].isString()) {
                emit errorOccurred(jsonObj["error"].toString());
                reply->deleteLater();
                return;
            }

            if (jsonObj.contains("response") && jsonObj["response"].isString()) {
                responseText += jsonObj["response"].toString();
            }
            
            if (jsonObj.contains("done") && jsonObj["done"].toBool(false)) {
                done = true;
                break;
            }
        }

        if (!done) {
            emit errorOccurred("Incomplete response: missing 'done' flag");
            reply->deleteLater();
            return;
        }

        if (jsonObj.contains("context")) {
            d->currentContext = jsonObj["context"];
        }

        responseText = LLMOutputProcessor(d->config.modelName).processInput(responseText);

        emit responseReceived(responseText.trimmed());
        emit responseMetadataReceived(done, d->currentContext);
    } else {
        emit errorOccurred(reply->errorString());
    }
    reply->deleteLater();
}

OperationResult OllamaClient::sendRequest(const QString& prompt) {
    Q_D(OllamaClient);
    
    QNetworkRequest request(QUrl(d->config.serverUrl + "/api/generate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = d->config.modelName;
    json["prompt"] = prompt;
    json["stream"] = false;
    //json["raw"] = false; // true tells Ollama to return only the final output without streaming.
    if (!d->currentContext.isNull()) {
        json["context"] = d->currentContext;
    }

    QNetworkReply* reply = d->networkManager.post(request, QJsonDocument(json).toJson());
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });

    return OperationResult::success();
}


} // namespace Internal
} // namespace BumbleAIBridge
