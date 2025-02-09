/**
 * @file AIFactory.cpp
 */

#include "BumbleAIBridge/AIFactory.h"
#include "internal/OllamaClient.h"
#include "internal/OpenAIClient.h"

namespace BumbleAIBridge {

AIClient* AIFactory::createClient(AIRunnerType type, const AIConfig& config) {
    switch (type) {
        case Ollama:
            return new Internal::OllamaClient(config);
        case OpenAI:
            return new Internal::OpenAIClient(config);
        default:
            return nullptr;
    }
}

}