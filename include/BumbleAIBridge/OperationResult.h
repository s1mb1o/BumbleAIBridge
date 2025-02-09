/**
 * @file OperationResult.h
 * @brief Class for reporting operation result
 */
#pragma once

#include <QString>

namespace BumbleAIBridge {

struct OperationResult {
    bool ok;              // true if the operation succeeded
    QString errorText;    // error message (empty if ok)

    // Allow the OperationResult to be used in boolean contexts.
    // The explicit keyword prevents unwanted implicit conversions.
    explicit operator bool() const { return ok; }

    static OperationResult success() {
        return { true, QString() };
    }
    static OperationResult failure(const QString &error) {
        return { false, error };
    }
};

}