#ifndef _CONSOLE_LOGGER_H_
#define _CONSOLE_LOGGER_H_

#include <Ex\Logger.h>
#include <Ex\RawCriticalSectionAutoLock.h>
#include <iostream>

namespace Ex
{
    /**
    *  ConsoleLogger is the concrete class used for logging to the console.
    */
    class ConsoleLogger : public Logger
    {
    public:
        /**
        * The ConsoleLogger constructor.
        */
        ConsoleLogger()
        {
            Win32::InitializeCriticalSection( &m_rawCriticalSection );
        }

        /**
        * The ConsoleLogger destructor.
        */
        virtual ~ConsoleLogger()
        {
            DtorResetDefaultLogger();
            Win32::DeleteCriticalSection( &m_rawCriticalSection );
        }

    protected:
        /**
        * Logs the formatted message to the std::out.
        *
        * @param [in] formattedMessage - formatted message to log.
        */
        virtual void LogFormattedMessage( const std::string& formattedMessage )
        {
            RawCriticalSectionAutoLock rawCriticalSectionAutoLock( m_rawCriticalSection );
            std::cout << formattedMessage << std::endl << std::endl;
        }

    private:
        CRITICAL_SECTION    m_rawCriticalSection;

        NO_STANDARD_MEMBERS( ConsoleLogger ); // hide copy ctor() and = operator()
    };
}

#endif //_CONSOLE_LOGGER_H_
