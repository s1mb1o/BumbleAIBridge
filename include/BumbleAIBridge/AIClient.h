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

class AIClientPrivate;

/**
 * @brief Abstract base class for AI model runner clients.
 */
class AIClient : public QObject 
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AIClient)
public:
    explicit AIClient(AIClientPrivate* priv, QObject* parent = nullptr);
    virtual ~AIClient();
    
    /**
     * @brief Sends a request to the AI model.
     * @param prompt Input text.
     * @return Response from the AI model.
     */
    virtual OperationResult sendRequest(const QString& prompt) = 0;

protected:
    QScopedPointer<AIClientPrivate> d_ptr;

signals:
    /// Emitted when a valid LLM response is received.
    void responseReceived(const QString &response);

    void responseMetadataReceived(bool done, const QJsonValue& context);

    /// Emitted if an error occurs.
    void errorOccurred(const QString &errorString);
};

} // namespace BumbleAIBridge
