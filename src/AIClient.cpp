/**
 * @file BumbleAIClient.cpp
 */

#include "BumbleAIBridge/AIClient.h"
#include "AIClient_p.h"

namespace BumbleAIBridge {

AIClient::AIClient(AIClientPrivate* priv, QObject* parent)
    : QObject(parent), d_ptr(priv) {
}

AIClient::~AIClient() 
{
    
}

}