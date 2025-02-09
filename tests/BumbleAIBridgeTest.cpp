#include "BumbleAIBridgeTest.h"
#include <QtTest/QtTest>
#include "BumbleAIBridge/BumbleAIBridge.h"

BumbleAIBridgeTest::BumbleAIBridgeTest(QObject* parent)
{

}

BumbleAIBridgeTest::~BumbleAIBridgeTest()
{

}


void BumbleAIBridgeTest::test_getWelcomeMessage() 
{
    QString msg = BumbleAIBridge::getWelcomeMessage();
    QVERIFY(!msg.isEmpty());
}

