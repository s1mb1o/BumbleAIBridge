#pragma once

#include "BumbleAIBridge/AIConfig.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace BumbleAIBridge {

class AIClientPrivate {
public:
    AIClientPrivate(AIConfig _config)
        : config(_config)
    {}

    AIConfig config;
    QNetworkAccessManager networkManager;
};

}