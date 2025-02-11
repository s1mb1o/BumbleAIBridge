/**
 * @file AIClient.h
 * @brief Base class for AI model runner clients.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QScopedPointer>
#include "AIConfig.h"
#include "BumbleAIBridge/OperationResult.h"

namespace BumbleAIBridge {

/**
 * @brief Enum representing supported AI model runner types.
 */
enum AIRunnerType {
    Ollama,
    OpenAI
};

class AIClientPrivate;

/**
 * @brief Abstract base class for AI model runner clients.
 */
class AIClient : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AIClient)
public:
    /**
     * @brief Constructs an AIClient object.
     * @param priv Pointer to the private implementation.
     * @param parent Parent QObject.
     */
    explicit AIClient(AIClientPrivate* priv, QObject* parent = nullptr);

    /**
     * @brief Virtual destructor.
     */
    virtual ~AIClient();


    virtual AIRunnerType type() const = 0;

    // Getter for configuration
    AIConfig config() const;

    // Setter for configuration
    void setConfig(const AIConfig& config);


    /**
     * @brief Sends a request to the AI model.
     * @param prompt Input text.
     * @return OperationResult indicating success or failure.
     */
    OperationResult sendRequest(const QString& prompt, const QString& system = QString(), const QList<QByteArray>& imagesBase64 = {});

    /**
     * @brief Checks if there is an active request.
     * @return True if a request is in progress, otherwise false.
     */
    bool hasActiveRequest() const;


    /**
     * @brief Requests the list of available models.
     * @return OperationResult indicating success or failure.
     */
    OperationResult requestListModels();


public slots:
    /**
     * @brief Resets the context for the AI model.
     */
    virtual void resetContext();

    /**
     * @brief Aborts the current request if it is in progress.
     */
    virtual void abort();

protected:
    QScopedPointer<AIClientPrivate> d_ptr;

    virtual OperationResult handleSpecificRequest(const QString& prompt, const QString& system, const QList<QByteArray>& imagesBase64) = 0;
    virtual OperationResult handleListModelsRequest() = 0;
signals:
    /**
     * @brief Emitted when LLM response finished.
     */
    void responseDone();

    /**
     * @brief Emitted when LLM response text is received.
     * @param response The response text.
     */
    void responseReceived(const QString &response);

    /**
     * @brief Emitted when one message is received.
     * @param response The response text.
     *
     * @sa messageAssembled
     */
    void messageReceived(const QString &role, const QString& content);

    /**
     * @brief Emitted when context extended
     * @param message Assembled sequence chat messages with one role
     *
     * { "role": "assistant", "content": "answer is ..." }
     */
    void contextExtended(const QJsonObject& message);


    void modelNamesReceived(const QStringList& modelList);

    /**
     * @brief Emitted when metadata of the response is received.
     * @param context Additional context data.
     */
    void responseMetadataReceived(const QJsonValue& context);

    /**
     * @brief Emitted if an error occurs.
     * @param errorString Description of the error.
     */
    void errorOccurred(const QString &errorString);
};

} // namespace BumbleAIBridge
