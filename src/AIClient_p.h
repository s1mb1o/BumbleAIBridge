#pragma once

#include "BumbleAIBridge/AIConfig.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPointer>

namespace BumbleAIBridge {

class AIClientPrivate {
public:
    AIClientPrivate(AIConfig _config)
        : config(_config)
    {}

    AIConfig config;
    QNetworkAccessManager networkManager;

    QPointer<QNetworkReply> activeReply;
};

}
