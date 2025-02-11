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
    explicit OpenAIClient(QObject* parent = nullptr) : OpenAIClient(AIConfig{}, parent) {}
    ~OpenAIClient() override;

    AIRunnerType type() const override {
        return AIRunnerType::OpenAI;
    }

    void resetContext() override;

protected:
    /**
     * @brief Sends a request to the OpenAI model.
     * @param prompt Input text.
     * @return Operation result indicating success or failure.
     */
    OperationResult handleSpecificRequest(const QString& prompt, const QString& system, const QList<QByteArray>& imagesBase64) override;

    // TODO: implement me
    OperationResult handleListModelsRequest() override { return OperationResult::notImplemented(); }

private:
    void handleReply(QNetworkReply* reply);

};

} // namespace Internal
} // namespace BumbleAIBridge
