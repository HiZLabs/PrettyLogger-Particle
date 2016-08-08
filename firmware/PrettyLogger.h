#ifndef PRETTY_LOGGER_H_
#define PRETTY_LOGGER_H_

#include "application.h"

class PrettyStreamLogHandler: public spark::StreamLogHandler {
public:

    /*!
        \brief Constructor.
        \param stream Output stream.
        \param level Default logging level.
        \param filters Category filters.
    */
    explicit PrettyStreamLogHandler(Stream &stream, LogLevel level = LOG_LEVEL_INFO, const Filters &filters = {});

protected:
    /*!
        \brief Formats log message and writes it to output stream.
        \param msg Text message.
        \param level Logging level.
        \param category Category name (can be null).
        \param attr Message attributes.

        Default implementation generates messages in the following format:
        `<timestamp> [category] [file]:[line], [function]: <level>: <message> [attributes]`.
    */
    virtual void logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) override;
//private:
//    void padField(const char* text, const unsigned width);
//    void logMessageStack(const char *msg, LogLevel level, const char *category, const LogAttributes &attr);
//    void logMessage(char* buf, size_t sizeof_buf, const char *msg, LogLevel level, const char *category, const LogAttributes &attr);
};

class PrettySerialLogHandler: public PrettyStreamLogHandler {
public:
    explicit PrettySerialLogHandler(LogLevel level = LOG_LEVEL_INFO, const Filters &filters = {}) :
	PrettyStreamLogHandler(Serial, level, filters) {
        Serial.begin();
        LogManager::instance()->addHandler(this);
    }

    explicit PrettySerialLogHandler(int baud, LogLevel level = LOG_LEVEL_INFO, const Filters &filters = {}) :
            PrettyStreamLogHandler(Serial, level, filters) {
        Serial.begin(baud);
        LogManager::instance()->addHandler(this);
    }

    virtual ~PrettySerialLogHandler() {
        LogManager::instance()->removeHandler(this);
        Serial.end();
    }
};

// spark::StreamLogHandler
inline PrettyStreamLogHandler::PrettyStreamLogHandler(Stream &stream, LogLevel level, const Filters &filters) :
        spark::StreamLogHandler::StreamLogHandler(stream, level, filters) { }

#endif
