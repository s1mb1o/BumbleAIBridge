#pragma once

namespace BumbleAIBridge {

class LLMOutputProcessorPrivate {
public:
    bool keepThink = false;

    QString llmModelName;   ///< ex: deepseek-r1:1.5b
};

} // namespace BumbleAIBridge
