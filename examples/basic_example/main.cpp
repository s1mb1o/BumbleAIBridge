#include <QCoreApplication>
#include <QDebug>
#include "BumbleAIBridge/BumbleAIBridge.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    qDebug() << BumbleAIBridge::getWelcomeMessage();
    return 0;
}
