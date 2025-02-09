#pragma once

#include "BumbleAIBridge/AIClient.h"
#include "BumbleAIBridge/AIConfig.h"
#include "BumbleAIBridge/OperationResult.h"
#include <QScopedPointer>


namespace BumbleAIBridge {
namespace Internal {

class OllamaClientPrivate;

/**
 * @brief Client for interfacing with an Ollama AI model.
 */
class OllamaClient : public BumbleAIBridge::AIClient {
    Q_OBJECT
    Q_DECLARE_PRIVATE(OllamaClient)

public:
    explicit OllamaClient(const AIConfig& config, QObject* parent = nullptr);
    ~OllamaClient() override;

    /**
     * @brief Sends a request to the Ollama AI model.
     * @param prompt Input text.
     * @return Operation result indicating success or failure.
     */
    OperationResult sendRequest(const QString& prompt) override;

private:
    void handleReply(QNetworkReply* reply);

};

} // namespace Internal
} // namespace BumbleAIBridge
