#pragma once

/**
 * @file AIFactory.h
 * @brief Factory class for creating AI model clients.
 */

#include <QList>
#include "BumbleAIBridge/AIClient.h"
#include "BumbleAIBridge/AIConfig.h"

namespace BumbleAIBridge {



/**
 * @brief Factory class for creating AI clients.
 */
class AIFactory {
public:
    /**
     * @brief Creates an AI client based on the specified model type.
     * @param type The type of AI runner model.
     * @param config The AI configuration.
     * @return A pointer to the created AI client or nullptr if the type is invalid.
     */
    static AIClient* createClient(AIRunnerType type, const AIConfig& config = AIConfig{});

    /**
     * @brief Returns a list of available AI runner types.
     * @return QList<AIRunnerType> containing all supported runner types.
     */
    static QList<AIRunnerType> availableRunners();

    /**
     * @brief Returns the string name of the given AIRunnerType.
     * @param type The AI runner type.
     * @return QString containing the name of the AI runner type.
     */
    static QString runnerTypeName(AIRunnerType type);
};

} // namespace BumbleAIBridge
