/**
 * @file OpenAIClient.cpp
 */

#include "OpenAIClient.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "AIClient_p.h"

namespace BumbleAIBridge {
namespace Internal {

/**
 * @brief Private implementation for OllamaClient, inheriting from AIClientPrivate.
 */
class OpenAIClientPrivate : public AIClientPrivate {
public:
    using AIClientPrivate::AIClientPrivate;  // Inherit constructor
};

OpenAIClient::OpenAIClient(const AIConfig& config, QObject* parent)
    : AIClient(new OpenAIClientPrivate(config), parent) 
    {
}

OpenAIClient::~OpenAIClient() 
{

}

void OpenAIClient::resetContext() {
    // No persistent context to reset in OpenAIClient.
}

void OpenAIClient::handleReply(QNetworkReply* reply)
{
    Q_D(OpenAIClient);


    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            emit errorOccurred("JSON parsing error: " + parseError.errorString());
            return;
        }

        if (jsonResponse.isObject()) {
            QJsonObject jsonObj = jsonResponse.object();
            if (jsonObj.contains("choices") && jsonObj["choices"].isArray()) {
                QJsonArray choices = jsonObj["choices"].toArray();
                if (!choices.isEmpty() && choices.first().isObject()) {
                    QString responseText = choices.first().toObject()["message"].toObject()["content"].toString();
                    emit responseReceived(responseText.trimmed());
                }
            }
        }
    } else {
        emit errorOccurred(reply->errorString());
    }
}

// TODO: QList<QByteArray>& imagesBase64
OperationResult OpenAIClient::handleSpecificRequest(const QString& prompt, const QString& system, const QList<QByteArray>& imagesBase64)
{
    Q_D(OpenAIClient);

    if (d->config.apiKey.isEmpty()) {
        return OperationResult::failure("Configuration error: apiKey is not set.");
    }


    QString urlStr = d->config.serverUrl;
    if (!d->config.serverUrl.endsWith("/")) {
        urlStr += "/";
    }
    urlStr += "v1/chat/completions";

    QNetworkRequest request(QUrl{urlStr});
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + d->config.apiKey.toUtf8());

    QJsonObject json;
    json["model"] = d->config.modelName;
    QJsonArray messages;
    if (!system.isEmpty()) {
        messages.append(QJsonObject{{"role", "system"}, {"content", system}});
    }
    messages.append(QJsonObject{{"role", "user"}, {"content", prompt}});
    json["messages"] = messages;
    json["max_tokens"] = 50;    // XXX
    json["stream"] = d->config.isStreamingMode;

    d->activeReply = d->networkManager.post(request, QJsonDocument(json).toJson());
    QObject::connect(d->activeReply, &QNetworkReply::finished, this, [this]() {
        Q_D(OpenAIClient);
        handleReply(d->activeReply);
        d->activeReply->deleteLater();
        d->activeReply.clear();
    });

    return OperationResult::success();
}



} // namespace Internal
} // namespace BumbleAIBridge
