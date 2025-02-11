#pragma once

#include "BumbleAIBridge/AIClient.h"
#include "BumbleAIBridge/AIConfig.h"
#include "BumbleAIBridge/OperationResult.h"
#include <QScopedPointer>


class QNetworkReply;

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
    explicit OllamaClient(QObject* parent = nullptr) : OllamaClient(AIConfig{}, parent) {}
    ~OllamaClient() override;

    AIRunnerType type() const override {
        return AIRunnerType::Ollama;
    }

    void resetContext() override;

protected:
    /**
     * @brief Sends a request to the Ollama AI model.
     * @param prompt Input text.
     * @return Operation result indicating success or failure.
     */
    OperationResult handleSpecificRequest(const QString& prompt, const QString& system, const QList<QByteArray>& imagesBase64) override;

    // TODO: implement me
    OperationResult handleListModelsRequest() override;


private:
    bool handleOneJsonObject(const QByteArray& data);
    void appendMessageToContext(const QJsonObject& message);
    void handleFinishedError();

private slots:
    void onReadyRead();

};

} // namespace Internal
} // namespace BumbleAIBridge
