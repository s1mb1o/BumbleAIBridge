#pragma once 

#include <QObject>
class BumbleAIBridgeTest : public QObject 
{
    Q_OBJECT

public:
    BumbleAIBridgeTest(QObject* parent = nullptr);

    virtual ~BumbleAIBridgeTest();

private slots:
    void test_getWelcomeMessage();
    void test_pingPongOllama();
    void test_pingPongOpenAI();
};
