#pragma once

#include <QObject>
#include <QString>

namespace BumbleAIBridge {

class LLMOutputProcessorPrivate;

class LLMOutputProcessor : public QObject {
    Q_OBJECT
    QScopedPointer<LLMOutputProcessorPrivate> d_ptr;
    Q_DECLARE_PRIVATE(LLMOutputProcessor)

public:
    explicit LLMOutputProcessor(const QString& llmModelName, QObject* parent = nullptr);
    ~LLMOutputProcessor();

    void setKeepThink(bool keep);
    bool getKeepThink() const;

    QString processInput(const QString& input) const;

private:
    QString cleanInput(const QString& input) const;

};

} // namespace BumbleAIBridge
