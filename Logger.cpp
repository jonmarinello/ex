#include <atltime.h>
#include <Windows.h>
#include <iostream>
#include "Ex\Ex.h"
#include "Ex\Logger.h"
#include "Ex\MessageBoxLogger.h"

Ex::Logger* Ex::Logger::s_pDefaultLogger = 0;


Ex::Logger::Logger()
{
}


Ex::Logger::~Logger()
{
}


void Ex::Logger::DtorResetDefaultLogger()
{
    if ( this == s_pDefaultLogger )
    {
        s_pDefaultLogger = NULL;
    }
}


void Ex::Logger::Log( const std::string& message )
{
    EX_PRECONDITION( message.size() > 0 );

    std::string formattedMessage = FormatMessage( message );
    LogFormattedMessage( formattedMessage );
}


Ex::Logger& Ex::Logger::GetDefaultLogger()
{
    static MessageBoxLogger* s_pMessageBoxLogger = new MessageBoxLogger();
    if ( s_pDefaultLogger == NULL )
    {
        s_pDefaultLogger = s_pMessageBoxLogger;
    }
    return *s_pDefaultLogger;
}


Ex::Logger& Ex::Logger::SetDefaultLogger( Logger& newDefaultLogger )
{
    Logger& defaultLogger = GetDefaultLogger();
    s_pDefaultLogger = &newDefaultLogger;
    return defaultLogger;
}


std::string Ex::Logger::FormatMessage( const std::string& message )
{
    CTime time( CTime::GetCurrentTime() );
    tm gmtTime = *( time.GetGmtTm() );

    std::stringstream formattedMessageStream;

    if ( Ex::Exception::GetVariableData()  )
    {
        formattedMessageStream << gmtTime.tm_mday << "/" << gmtTime.tm_mon << "/" << gmtTime.tm_year + 1900 << " "
                               << gmtTime.tm_hour << ":" << gmtTime.tm_min << ":" << gmtTime.tm_sec << " gmt - "
                               << message;
    }
    else
    {
        formattedMessageStream << "<date and time suppressed>" << " - "
                               << message;
    }

    return formattedMessageStream.str();
}
