#pragma once

/**
 * @file AIFactory.h
 * @brief Factory class for creating AI model clients.
 */

#include "BumbleAIBridge/AIClient.h"
#include "BumbleAIBridge/AIConfig.h"


namespace BumbleAIBridge {

/**
 * @brief Enum representing supported AI model runner types.
 */
enum AIRunnerType {
    Ollama,
    OpenAI
};

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
    static AIClient* createClient(AIRunnerType type, const AIConfig& config);
};

} // namespace BumbleAIBridge