#include <QCoreApplication>
#include <QDebug>
#include "BumbleAIBridge/BumbleAIBridge.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    // Advanced usage example: maybe using multiple API functions and settings
    qDebug() << "Advanced Example: " << BumbleAIBridge::getWelcomeMessage();
    return 0;
}
