#pragma once

#include "BumbleAIBridge/AIClient.h"
#include "BumbleAIBridge/AIConfig.h"
#include "BumbleAIBridge/OperationResult.h"
#include <QScopedPointer>

class QNetworkReply;

namespace BumbleAIBridge {
namespace Internal {

class OpenAIClientPrivate;

/**
 * @brief Client for interfacing with OpenAI models.
 */
class OpenAIClient : public AIClient {
    Q_OBJECT
    Q_DECLARE_PRIVATE(OpenAIClient)

public:
    explicit OpenAIClient(const AIConfig& config, QObject* parent = nullptr);
    ~OpenAIClient() override;

    /**
     * @brief Sends a request to the OpenAI model.
     * @param prompt Input text.
     * @return Operation result indicating success or failure.
     */
    OperationResult sendRequest(const QString& prompt) override;

private:
    void handleReply(QNetworkReply* reply);

};

} // namespace Internal
} // namespace BumbleAIBridge
