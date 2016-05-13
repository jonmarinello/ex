#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <memory>
#include <include\stdhdr.h>

namespace Ex
{
    /**
    * Logger is the abstract base class that all concrete loggers
    * should be derived from.
    *
    * The logger system is based on the concept of a default logger. If
    * no consumer defined logger is explicitly set with SetDefaultLogger()
    * the MessageBoxLogger will be used. This may be changed at any time by
    * calling SetDefaultLogger().
    */
    class Logger
    {
    public:
        /**
        * The Logger constructor.
        */
        Logger();

        /**
        * The Logger destructor.
        */
        virtual ~Logger();

        /**
        * Logs a message to a logger.
        *
        * @param [in] message - message to log.
        */
        virtual void Log( const std::string& message );

        /**
        * Gets the default logger.
        */
        static Logger& GetDefaultLogger();

        /**
        * Sets the default logger.
        *
        * @param [in] newDefaultLogger - new default logger.
        * @return The previous default logger.
        */
        static Logger& SetDefaultLogger( Logger& newDefaultLogger );

    protected:
        /**
        * Formats a message in the standard logging format.
        *
        * @param [in ] message - message to format.
        * @return The formated message.
        */
        std::string FormatMessage( const std::string& message );

        /**
        * Logs the formatted message using the derived classes implementation.
        *
        * @param [in] formattedMessage - formatted message to log.
        */
        virtual void LogFormattedMessage( const std::string& formattedMessage ) = 0;

        /**
        * Must be called by all derived class destructors to properly handle
        * resetting of the MessageBoxLogger.
        */
        void DtorResetDefaultLogger();

    private:
        static Logger*  s_pDefaultLogger;

        NO_STANDARD_MEMBERS( Logger ); // hide copy ctor() and = operator()
    };
}

#endif //_LOGGER_H_
