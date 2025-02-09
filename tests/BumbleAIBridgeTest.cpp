#include "BumbleAIBridgeTest.h"
#include <QtTest/QtTest>
#include "BumbleAIBridge/AIClient.h"
#include "BumbleAIBridge/AIFactory.h"
#include "BumbleAIBridge/AIConfig.h"

using namespace BumbleAIBridge;

namespace {
#if 0
    const QString OpenAIUrl = "https://api.openai.com";
    const QString OpenAIModelName = "gpt-4";
#else
    // Ollama provides experimental compatibility with parts of the OpenAI API to help connect existing applications to Ollama.
    const QString OpenAIUrl = "http://localhost:11434";
    const QString OpenAIModelName = "llama3.2:3b";
#endif
}

BumbleAIBridgeTest::BumbleAIBridgeTest(QObject* parent) {}

BumbleAIBridgeTest::~BumbleAIBridgeTest() {}

void BumbleAIBridgeTest::test_getWelcomeMessage() 
{
    QString msg = "BumbleAIBridge::getWelcomeMessage()";
    QVERIFY(!msg.isEmpty());
}

void BumbleAIBridgeTest::test_pingPongOllama()
{
    AIConfig config;
    config.serverUrl = "http://127.0.0.1:11434";
    config.modelName = "llama3.2:3b";
    
    std::unique_ptr<AIClient> client(AIFactory::createClient(Ollama, config));
    QVERIFY(client != nullptr);
    
    QSignalSpy responseSpy(client.get(), &AIClient::responseReceived);
    QSignalSpy errorSpy(client.get(), &AIClient::errorOccurred);
    
    client->sendRequest("Answer only lowercase 'pong'");

    QEventLoop loop;
    QObject::connect(client.get(), &AIClient::responseReceived, &loop, &QEventLoop::quit);
    QObject::connect(client.get(), &AIClient::errorOccurred, &loop, &QEventLoop::quit);

    QTimer::singleShot(50000, &loop, &QEventLoop::quit); // Timeout after 50s
    loop.exec(); // Wait for either signal

    if (!errorSpy.isEmpty()) {
        QFAIL("Test failed due to error: " + errorSpy.takeFirst().at(0).toString().toUtf8());
    }
    
    QVERIFY(!responseSpy.isEmpty());
    QCOMPARE(responseSpy.takeFirst().at(0).toString(), "pong");
}

void BumbleAIBridgeTest::test_pingPongOpenAI()
{
    AIConfig config;

    config.serverUrl = OpenAIUrl;
    config.modelName = OpenAIModelName;

    config.apiKey = qgetenv("BUMBLE_OPENAI_API_KEY");

    std::unique_ptr<AIClient> client(AIFactory::createClient(OpenAI, config));
    QVERIFY(client != nullptr);

    QSignalSpy responseSpy(client.get(), &AIClient::responseReceived);
    QSignalSpy errorSpy(client.get(), &AIClient::errorOccurred);

    client->sendRequest("MUST answer only lowercase 'pong'");

    QEventLoop loop;
    QObject::connect(client.get(), &AIClient::responseReceived, &loop, &QEventLoop::quit);
    QObject::connect(client.get(), &AIClient::errorOccurred, &loop, &QEventLoop::quit);

    QTimer::singleShot(50000, &loop, &QEventLoop::quit); // Timeout after 50s
    loop.exec(); // Wait for either signal

    if (!errorSpy.isEmpty()) {
        QFAIL("Test failed due to error: " + errorSpy.takeFirst().at(0).toString().toUtf8());
    }

    QVERIFY(!responseSpy.isEmpty());
    QCOMPARE(responseSpy.takeFirst().at(0).toString(), "pong");
}


