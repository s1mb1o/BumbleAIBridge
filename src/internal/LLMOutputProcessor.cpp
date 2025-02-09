#include "LLMOutputProcessor.h"
#include "LLMOutputProcessor_p.h"
#include <QRegularExpression>



namespace BumbleAIBridge {


LLMOutputProcessor::LLMOutputProcessor(const QString& llmModelName, QObject* parent)
    : QObject(parent), d_ptr(new LLMOutputProcessorPrivate) 
{
    Q_D(LLMOutputProcessor);

    d->llmModelName = llmModelName;

}

LLMOutputProcessor::~LLMOutputProcessor() = default;

void LLMOutputProcessor::setKeepThink(bool keep) {
    Q_D(LLMOutputProcessor);
    d->keepThink = keep;
}

bool LLMOutputProcessor::getKeepThink() const {
    Q_D(const LLMOutputProcessor);
    return d->keepThink;
}

QString LLMOutputProcessor::processInput(const QString& input) const {
    Q_D(const LLMOutputProcessor);
    return cleanInput(input);
}

QString LLMOutputProcessor::cleanInput(const QString& input) const
{

    QRegularExpression regex(R"(<think>.*?</think>\s*)", QRegularExpression::DotMatchesEverythingOption);

    QString output = input;
    output.replace(regex, "");
    output.replace("\"", "\"");
    output.replace("\\n", "\n");
    return output.trimmed();
}



} // namespace BumbleAIBridge
