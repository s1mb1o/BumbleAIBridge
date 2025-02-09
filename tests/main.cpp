#include <QCoreApplication>
#include <QtTest/QtTest>
#include "BumbleAIBridgeTest.h"

int main(int argc, char *argv[]) 
{
    QCoreApplication app(argc, argv);
    BumbleAIBridgeTest test;
    return QTest::qExec(&test, argc, argv);
}