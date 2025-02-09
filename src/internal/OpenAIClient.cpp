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


void OpenAIClient::handleReply(QNetworkReply* reply)
{
    Q_D(OpenAIClient);

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            emit errorOccurred("JSON parsing error: " + parseError.errorString());
            reply->deleteLater();
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
        qDebug() << reply->readAll();
        emit errorOccurred(reply->errorString());
    }
    reply->deleteLater();
}

OperationResult OpenAIClient::sendRequest(const QString& prompt)
{
    Q_D(OpenAIClient);


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
    messages.append(QJsonObject{{"role", "user"}, {"content", prompt}});
    json["messages"] = messages;
    json["max_tokens"] = 50;

    QNetworkReply* reply = d->networkManager.post(request, QJsonDocument(json).toJson());
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleReply(reply); });

    return OperationResult::success();
}



} // namespace Internal
} // namespace BumbleAIBridge
