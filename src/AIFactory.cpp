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


QList<AIRunnerType> AIFactory::availableRunners() {
    return { Ollama, OpenAI };
}

QString AIFactory::runnerTypeName(AIRunnerType type) {
    switch (type) {
    case Ollama:
        return "Ollama";
    case OpenAI:
        return "OpenAI";
    default:
        return "Unknown";
    }
}

} // namespace BumbleAIBridge
