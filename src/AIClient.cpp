/**
 * @file BumbleAIClient.cpp
 */

#include "BumbleAIBridge/AIClient.h"
#include <QCoreApplication>
#include "AIClient_p.h"

namespace BumbleAIBridge {

AIClient::AIClient(AIClientPrivate* priv, QObject* parent)
    : QObject(parent), d_ptr(priv) {
}

AIClient::~AIClient() 
{
    
}


AIConfig AIClient::config() const {
    Q_D(const AIClient);

    return d->config; // Return the current configuration
}

void AIClient::setConfig(const AIConfig& config) {
    Q_D(AIClient);

    d->config = config; // Set the new configuration
}


bool AIClient::hasActiveRequest() const {
    Q_D(const AIClient);
    return !d->activeReply.isNull();
}


void AIClient::resetContext() {
    // Reset context logic can be overridden in derived classes.
}

void AIClient::abort() {
    Q_D(AIClient);
    if (!d->activeReply.isNull()) {
        d->activeReply->abort();        // QNetworkReply::finished() emitted
    }
}

OperationResult AIClient::sendRequest(const QString& prompt, const QString& system, const QList<QByteArray>& imagesBase64)
{
    Q_D(AIClient);

    // Check if serverUrl, modelName, and apiKey are set
    if (d->config.serverUrl.isEmpty()) {
        return OperationResult::failure("Configuration error: serverUrl is not set.");
    }
    if (d->config.modelName.isEmpty()) {
        return OperationResult::failure("Configuration error: modelName is not set.");
    }

    // Common checks
    if (hasActiveRequest()) {
        return OperationResult::failure("Client is busy with an active request.");
    }

    // Call the subclass-specific request handling
    return handleSpecificRequest(prompt, system, imagesBase64);
}

OperationResult AIClient::requestListModels() {

    Q_D(AIClient);

    // Check if serverUrl
    if (d->config.serverUrl.isEmpty()) {
        return OperationResult::failure("Configuration error: serverUrl is not set.");
    }

    // Common checks
    if (hasActiveRequest()) {
        return OperationResult::failure("Client is busy with an active request.");
    }

    // Call the subclass-specific request handling
    return handleListModelsRequest();
}


}
