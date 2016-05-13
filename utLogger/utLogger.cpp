// utLogger Implementation

#include <algorithm>
#include <cctype>

#include "HhUnitTestFramework\UnitTest.h"
#include "Ex\Ex.h"
#include "Ex\ConsoleLogger.h"
#include "Ex\MessageBoxLogger.h"
#include "Ex\FileLogger.h"
#include "Stub\Win32Stub.h"

// This is the unit test for the Logger classes
class utLogger : public HH_UnitTest
{
public:
    utLogger( bool& variableDataGeneration, bool interactive )
        : HH_UnitTest( variableDataGeneration, interactive )
    {
    }

protected:
    void TestCommands();

private:
    boost::shared_ptr< Ex::ConsoleLogger >      m_pConsoleLogger;
    boost::shared_ptr< Ex::MessageBoxLogger >   m_pMessageBoxLogger;
    boost::shared_ptr< Ex::FileLogger >         m_pFileLogger;
};


/* ------------------------------- utLogger member methods ------------------------------*/

void utLogger::TestCommands()
{
    try
    {
        /* -------------------------------------------------------- */

        WIN32_TEST_COMMANDS;

        /* -------------------------------------------------------- */

        HH_TESTCMD( "Assert" )
        {
            EX_ASSERT( Ex::False() );
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "Log" )
        {
            HH_TESTCMD_PARAM( HH_MultiTokenString, message );
            Ex::Logger::GetDefaultLogger().Log( message.GetString() );

        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "NewConsoleLogger" )
        {
            m_pConsoleLogger = boost::shared_ptr< Ex::ConsoleLogger >( new Ex::ConsoleLogger() );
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "NewMessageBoxLogger" )
        {
            HH_TESTCMD_PARAM( HH_MultiTokenString, applicationName );
            m_pMessageBoxLogger = boost::shared_ptr< Ex::MessageBoxLogger >(
                new Ex::MessageBoxLogger( applicationName.GetString() ) );
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "NewFileLogger" )
        {
            HH_TESTCMD_PARAM( std::string, name );

            m_pFileLogger = boost::shared_ptr< Ex::FileLogger >( new Ex::FileLogger() );
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "SetDefaultLogger" )
        {
            HH_TESTCMD_PARAM( std::string, name );

            std::transform( name.begin(), name.end(), name.begin(), std::tolower );
            if ( name == "console" )
            {
                HH_OBJECT_MUST_EXIST( m_pConsoleLogger.get(), ConsoleLogger );
                Ex::Logger::SetDefaultLogger( *m_pConsoleLogger );
            }
            else if ( name == "messagebox" )
            {
                HH_OBJECT_MUST_EXIST( m_pMessageBoxLogger.get(), MessageBoxLogger );
                Ex::Logger::SetDefaultLogger( *m_pMessageBoxLogger );
            }
            else if ( name == "file" )
            {
                HH_OBJECT_MUST_EXIST( m_pFileLogger.get(), FileLogger );
                Ex::Logger::SetDefaultLogger( *m_pFileLogger );
            }
            else
            {
                std::cout << "invalid logger name" << std::endl;
            }
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "GetDefaultLogger" )
        {
            Ex::Logger::GetDefaultLogger();
        }
    }
    EX_CATCH_AND_LOG
}



void main( int argc, char*[] )
{
    try
    {
        // NOTE: Do not declare a console logger here like other unit tests so we can
        //       test the default message box logger.
        utLogger utLogger( Ex::Exception::GetVariableData(), argc == 1 );
        utLogger.Run();
    }
    EX_CATCH_AND_LOG
}
