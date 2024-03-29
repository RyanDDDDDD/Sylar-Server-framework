#ifndef __SERVER_LOG_HPP__
#define __SERVER_LOG_HPP__

#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>
#include <time.h>
#include <string.h>
#include <map>

#include "util.hpp"
#include "singleton.hpp"
#include "thread.hpp"

// retrieve event input stream from logger
#define SERVER_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level) \
        Server::LogEventWrap(   \
            Server::LogEvent::ptr(new Server::LogEvent( \
            logger, level, __FILE__, __LINE__, 0, Server::getThreadId(), \
            Server::getFiberId(), time(0), Server::Thread::GetName()))).getSS()

#define SERVER_LOG_DEBUG(logger) SERVER_LOG_LEVEL(logger, Server::LogLevel::Level::DEBUG)
#define SERVER_LOG_INFO(logger) SERVER_LOG_LEVEL(logger, Server::LogLevel::Level::INFO)
#define SERVER_LOG_WARN(logger) SERVER_LOG_LEVEL(logger, Server::LogLevel::Level::WARN)
#define SERVER_LOG_ERROR(logger) SERVER_LOG_LEVEL(logger, Server::LogLevel::Level::ERROR)
#define SERVER_LOG_FATAL(logger) SERVER_LOG_LEVEL(logger, Server::LogLevel::Level::FATAL)

// Support user defined format
#define SERVER_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        Server::LogEventWrap(Server::LogEvent::ptr(new Server::LogEvent(logger, level, \
        __FILE__, __LINE__,  0, Server::getThreadId(), \
        Server::getFiberId(), time(0), Server::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__)

#define SERVER_LOG_FMT_DEBUG(logger, fmt, ...) SERVER_LOG_FMT_LEVEL(logger, Server::LogLevel::Level::DEBUG, fmt, __VA_ARGS__)
#define SERVER_LOG_FMT_INFO(logger, fmt, ...) SERVER_LOG_FMT_LEVEL(logger, Server::LogLevel::Level::INFO, fmt, __VA_ARGS__)
#define SERVER_LOG_FMT_WARN(logger, fmt, ...) SERVER_LOG_FMT_LEVEL(logger, Server::LogLevel::Level::WARN, fmt, __VA_ARGS__)
#define SERVER_LOG_FMT_ERROR(logger, fmt, ...) SERVER_LOG_FMT_LEVEL(logger, Server::LogLevel::Level::ERROR, fmt, __VA_ARGS__)
#define SERVER_LOG_FMT_FATAL(logger, fmt, ...) SERVER_LOG_FMT_LEVEL(logger, Server::LogLevel::Level::FATAL, fmt, __VA_ARGS__)

// Get default logger from Mgr
#define SERVER_LOG_ROOT() Server::LoggerMgr::getInstance()->getRoot()
// Get logger with specific name 
#define SERVER_LOG_NAME(name) Server::LoggerMgr::getInstance()->getLogger(name)

namespace Server {

class Logger;
class LoggerManager;

// Log (message) Level
class LogLevel {
public:
    enum class Level {
        UNKNOWN = 0,
        DEBUG   = 1,
        INFO    = 2,
        WARN    = 3,
        ERROR   = 4,
        FATAL   = 5
    };
    
    // convert Log level to string format
    static const char* ToString (LogLevel::Level level);

    // convert string format log to LogLevel class
    static  LogLevel::Level FromString(const std::string& str);
};

// Log (message) Event
class LogEvent {
public:
    using ptr = std::shared_ptr<LogEvent>;
    LogEvent (){};
    
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
            const char* file, int32_t line, uint32_t elapse, 
            uint32_t threadId,uint32_t fiberId, uint64_t time,
            const std::string& threadName);

    const char* getFile () const { return m_file; };

    int32_t getLine () const { return m_line; };

    uint32_t getElapse () const { return m_elapse; };

    uint32_t getFiberId () const { return m_fiberId; };

    uint32_t getThreadId () const {return m_threadId; };

    uint32_t getTime () const { return m_time; };

    std::string getContent () const { return m_ss.str(); };

    std::shared_ptr<Logger> getLogger() const { return m_logger; };

    LogLevel::Level getLevel() const { return m_level; };

    std::stringstream& getSS() { return m_ss; };

    const std::string& getThreadName() const { return m_threadName; };

    // use user-defined formatter to format event
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);

private:
    const char* m_file    = nullptr;  // file name
    int32_t m_line        = 0;        // lines' number
    uint32_t m_elapse     = 0;        // milliseconds between program start and current
    uint32_t m_threadId   = 0;        // thread Id
    uint32_t m_fiberId    = 0;        // coroutine Id
    uint64_t m_time       = 0;        // time stamp
    std::string m_threadName;         // Thread name
    std::stringstream m_ss;           // input stream for user input
    std::shared_ptr<Logger> m_logger; // ptr to the logger which format current event
    LogLevel::Level m_level;          // event level
};

// A Wrapper for Event, which can be used to retrieve input stream of event
class LogEventWrap {
public:
    LogEventWrap(LogEvent::ptr e);
    ~LogEventWrap();

    // get log event
    LogEvent::ptr getEvent() const { return m_event;}

    // get event input stream
    std::stringstream& getSS();
private:
    LogEvent::ptr m_event;
};

// Log formatter
class LogFormatter {
public:
    using ptr = std::shared_ptr<LogFormatter>;

    LogFormatter (const std::string& pattern);

    std::ostream& format(std::ostream& ofs,
                        std::shared_ptr<Logger> logger,
                        LogLevel::Level level,
                        LogEvent::ptr event);

    std::string format (std::shared_ptr<Logger> logger,
                        LogLevel::Level level,
                        LogEvent::ptr event); // format event to string
public:
    // sub-module to parse Log Event, each formatItem is related to specific pattern(placeholder in pattern)
    class FormatItem {
    public:
        using ptr = std::shared_ptr<FormatItem>;

        // FormatItem (const std::string& fmt = "") {};

        virtual ~FormatItem (){};

        virtual void format (std::ostream& os,
                            std::shared_ptr<Logger> logger,
                            LogLevel::Level level,
                            LogEvent::ptr event) = 0;
    };
    
    void init ();                         // config formatterItem with the given pattern(m_pattern)
    bool isError() const { return m_error; }; 
    const std::string getPattern() const { return m_pattern; }
private:
    std::string m_pattern;                // format event according to pattern
    std::vector<FormatItem::ptr> m_items; // list of base Formatitem to format each event
    bool m_error = false;                 // determine current pattern is invalid
};

// Log ouput destination
class LogAppender {
friend class Logger;
public:
    using ptr = std::shared_ptr<LogAppender>;
    using MutexType = Spinlock;
    
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) = 0; // output log

    // output to yaml string
    virtual std::string toYamlString() = 0;
    
    // set new formatter
    void setFormatter(LogFormatter::ptr val);

    // retrieve formatter
    LogFormatter::ptr getFormatter();

    // set logging level
    void setLevel(LogLevel::Level level){ m_level = level; }

    // retrieve logging level
    LogLevel::Level getLevel() const {return m_level; }

    virtual ~LogAppender(){};

protected:
    LogFormatter::ptr m_formatter;                              // format output to destination
    LogLevel::Level m_level = LogLevel::Level::DEBUG;           // Minimum level of Log the Appender can process
    bool m_hasFormatter = false;                                // Appender has its own formatter, not using logger formatter
    MutexType m_mutex;
};

// Logger
class Logger : public std::enable_shared_from_this<Logger> {
    friend class LoggerManager;
public:
    using ptr = std::shared_ptr<Logger>;
    using MutexType = Spinlock;

    Logger(const std::string& name = "root");

    void log(LogLevel::Level level, const LogEvent::ptr event); // use appender to output log

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    void clearAppenders();

    const std::string& getName() const { return m_name; };

    LogLevel::Level getLevel() const { return m_level; };

    void setLevel(LogLevel::Level val) { m_level = val; };

    void setFormatter(const std::string& val);
    void setFormatter(LogFormatter::ptr val);

    LogFormatter::ptr getFormatter();

    std::string toYamlString();

private:
    std::string m_name;                         // logger name
    LogLevel::Level m_level;                    // minimum Level of Log which can be processed by Logger
    MutexType m_mutex;
    std::list<LogAppender::ptr> m_appenders;    // a list of appenders to output log, we can customize appenders to output to different position
    LogFormatter::ptr m_formatter;              // when the added formatter is not set properly, assign logger formatter to it
    Logger::ptr m_root;
};

// Output to console
class StdoutLogAppender : public LogAppender {
public:
    using ptr = std::shared_ptr<StdoutLogAppender>;
    virtual void log (Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    virtual std::string toYamlString() override;
};

// Output to file
class FileLogAppender : public LogAppender {
public:
    using ptr = std::shared_ptr<FileLogAppender>;

    FileLogAppender(const std::string& filename);

    void log (Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYamlString() override;
    bool reopen ();             // if the output file is opened, close and open it

private:
    std::string m_filename;     // opened file
    std::ofstream m_filestream; // file stream binded to opened file
    uint64_t m_lastTime = 0;
};  

/* add more customized appender below */

// Mangger to store all loggers, we can retrieve logger from Mangager
class LoggerManager {
public:
    // Use Spinock in LogMgr
    using MutexType = Spinlock;
    LoggerManager();

    // get a customized logger
    Logger::ptr getLogger(const std::string& name);

    // retrieve a default logger
    Logger::ptr getRoot() const { return m_root; };

    // store an new loger into manger
    bool storeLogger(const std::string label, const Logger::ptr logger);

    // initialize setting using configuration module
    void init();

    std::string toYamlString ();

private:
    MutexType m_mutex;
    std::map<std::string, Logger::ptr> m_loggers;

    // default logger
    Logger::ptr m_root;
};

using LoggerMgr = Server::Singleton<LoggerManager>;

} // namespace Server

#endif