#ifndef _MESSAGE_BOX_LOGGER_H_
#define _MESSAGE_BOX_LOGGER_H_

#include <Ex\Logger.h>
#include <Win32\Win32.h>
#include <windows.h>

namespace Ex
{
    /**
    * MessageBoxLogger is the concrete class used for logging to a windows
    * message box.
    */
    class MessageBoxLogger : public Logger
    {
    public:
        /**
        * The MessageBoxLogger constructor.
        */
        MessageBoxLogger( const std::string& applicationName = "" )
            : m_applicationName( applicationName )
        {
        }

        /**
        * The MessageBoxLogger destructor.
        */
        virtual ~MessageBoxLogger()
        {
            DtorResetDefaultLogger();
        }

    protected:
        /**
        * Logs the formatted message to a windows message box.
        *
        * @param [in] formattedMessage - formatted message to log.
        */
        virtual void LogFormattedMessage( const std::string& formattedMessage )
        {
            Win32::MessageBox( NULL, formattedMessage.c_str(), m_applicationName.c_str(), MB_ICONSTOP | MB_OK );
        }

    private:
        const std::string m_applicationName;

        NO_STANDARD_MEMBERS( MessageBoxLogger ); // hide copy ctor() and = operator()
    };
}

#endif //_MESSAGE_BOX_LOGGER_H_
