/**
 * @file AIConfig.h
 * @brief Configuration class for AI model clients.
 */
#pragma once

#include <QString>

namespace BumbleAIBridge {

/**
 * @brief Configuration settings for AI model clients.
 */
class AIConfig {
public:
    QString serverUrl;
    QString apiKey;
    QString modelName;
};

}