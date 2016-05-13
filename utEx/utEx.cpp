// utEx Implementation

#include "HhUnitTestFramework\UnitTest.h"
#include "Ex\Ex.h"
#include "Ex\ConsoleLogger.h"
#include "Stub\Win32Stub.h"
#include "Win32\Exceptions.h"

// This is the unit test for the Exception classes
class utEx : public HH_UnitTest
{
public:
    utEx( bool& variableDataGeneration, bool interactive )
        : HH_UnitTest( variableDataGeneration, interactive )
    {
    }

protected:
    void TestCommands();

    bool Compare( int a, int b )
    {
        return a == b;
    };

private:
    Ex::ExceptionPtr    m_pException;
};

/* ------------------------------- test exceptions ------------------------------*/

class FileOpenException : public Ex::NamedException
{
public:
    FileOpenException( const std::string& fileName, Ex::Exception* pExecptionToNest = 0 )
        : Ex::NamedException( pExecptionToNest )
    {
        m_stream << "Can't open \"" << fileName << "\"";
    }

    INSERT_EXCEPTION_DEFS( FileOpenException );
};

class NestedException : public Ex::NamedException
{
public:
    NestedException( Ex::Exception* pExecptionToNest = 0 )
        : Ex::NamedException( pExecptionToNest )
    {
        m_stream << "Description of Nested exception";
    }

    INSERT_EXCEPTION_DEFS( NestedException );
};

class DataException : public Ex::NamedException
{
public:
    DataException( int intValue, const std::string stringValue, Ex::Exception* pExecptionToNest = 0 )
        : Ex::NamedException( pExecptionToNest ),
          m_intValue( intValue ),
          m_stringValue( stringValue )
    {
        m_stream << "DataException should contain intValue = " << intValue << " stringValue = \"" << stringValue << "\"";;
    }

    INSERT_EXCEPTION_DEFS( DataException );

    int m_intValue;
    std::string m_stringValue;
};

class AnException : public Ex::NamedException
{
public:
    AnException( Ex::Exception* pExecptionToNest = 0 )
        : Ex::NamedException( pExecptionToNest )
    {
        m_stream << "AnException";
    }

    INSERT_EXCEPTION_DEFS( AnException );
};


class ThrowInDtor
{
public:
    ThrowInDtor()
    {
    }
    ~ThrowInDtor()
    {
        try
        {
            throw AnException();
        }
        EX_DTOR_CATCH_AND_LOG
    }
};


/* ------------------------------- stack backtrace test functions ------------------------------*/

void f3( )
{
    try
    {
        EX_ASSERT( Ex::False() );
    }
    EX_STACK_BACKTRACE
}


void f2( )
{
    try
    {
        f3( );
    }
    EX_STACK_BACKTRACE
}


void f1( )
{
    try
    {
        f2( );
    }
    EX_STACK_BACKTRACE
}

void ShowStackWithNesting1( int numberOfStackFrames )
{
    try 
    {
        if ( numberOfStackFrames == 0 )
        {
            try
            {
                f1();
            }
            catch( Ex::Exception& ex )
            {
                throw NestedException( &ex );
            }
        }
        else
        {
            ShowStackWithNesting1( --numberOfStackFrames );
        }
    }
    EX_STACK_BACKTRACE
}

void ShowStackWithNesting( int numberOfStackFrames )
{
    try 
    {
        ShowStackWithNesting1( --numberOfStackFrames );
    }
    catch( Ex::Exception& ex )
    {
        ex.Log( "ShowStackWithNesting() caught exception" );
    }
}


/* ------------------------------- utility test functions ------------------------------*/


/* ------------------------------- utEx member methods ------------------------------*/


void utEx::TestCommands()
{
    try
    {
        /* -------------------------------------------------------- */

        WIN32_TEST_COMMANDS;

        /* -------------------------------------------------------- */

        HH_TESTCMD( "SizeOf" )
        {
            std::cout << sizeof Ex::Exception;
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "ThrowInDtor" )
        {
            HH_TESTCMD_PARAM( bool, activeExceptionOnStack );
            {
                ThrowInDtor throwInDtor;
                if ( activeExceptionOnStack )
                {
                    throw AnException();
                }
            }
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "ThrowInDtorStl" )
        {
            HH_TESTCMD_PARAM( bool, activeStlExceptionOnStack );
            {
                ThrowInDtor throwInDtor;
                if ( activeStlExceptionOnStack )
                {
                    throw std::bad_alloc();
                }
            }
        }

        /* -------------------------------------------------------- */


        HH_TESTCMD( "ThrowStlException" )
        {
            HH_TESTCMD_PARAM( HH_MultiTokenString, what );

            throw Ex::StlException( what.GetString() );
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "Assert" )
        {
            EX_ASSERT( Compare( 1, 1 ) );  // Better not throw
            EX_ASSERT( Compare( 1, 2 ) );  // Should throw
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "Precondition" )
        {
            EX_PRECONDITION( Compare( 1, 1 ) );  // Better not throw
            EX_PRECONDITION( Compare( 1, 2 ) );  // Should throw
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "CreateFileOpenException" )
        {
            HH_TESTCMD_PARAM( HH_MultiTokenString, fileName );

            Win32::Win32Exception   win32Exception( "::CreateFile()", 2 );
            m_pException = ( Ex::ExceptionPtr )new FileOpenException( fileName.GetString(), &win32Exception );
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "AsString" )
        {
            HH_OBJECT_MUST_EXIST( m_pException.get(), "m_pException" );

            std::cout << m_pException->AsString() << std::endl;
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "GetName" )
        {
            HH_OBJECT_MUST_EXIST( m_pException.get(), "m_pException" );

            std::cout << m_pException->GetName() << std::endl;
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "Clone" )
        {
            HH_OBJECT_MUST_EXIST( m_pException.get(), "m_pException" );

            Ex::ExceptionPtr pClonedException = m_pException->Clone();

            std::cout << pClonedException->AsString() << std::endl;
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "Log" )
        {
            HH_TESTCMD_PARAM( HH_MultiTokenString, message );

            HH_OBJECT_MUST_EXIST( m_pException.get(), "m_pException" );

            m_pException->Log( message.GetString() );

        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "Stack" )
        {
            f1();
        }
        
        /* -------------------------------------------------------- */

        HH_TESTCMD( "StackWithNesting" )
        {
            HH_TESTCMD_PARAM( int, numberOfStackFrames );
            ShowStackWithNesting( numberOfStackFrames );
        }
        
        /* -------------------------------------------------------- */

        HH_TESTCMD( "ClearException" )
        {
            m_pException = Ex::ExceptionPtr();
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "DataExceptionClone" )
        {
            HH_TESTCMD_PARAM( int, intValue );
            HH_TESTCMD_PARAM( HH_MultiTokenString, stringValue );

            DataException dataException( intValue, stringValue.GetString() );
            Ex::ExceptionPtr pClonedException = dataException.Clone();

            std::cout << "orginal " << dataException.AsString() << std::endl;
            std::cout << "clone " << pClonedException->AsString() << std::endl;
            std::cout << "original's m_intValue = " << dataException.m_intValue << std::endl;
            std::cout << "clone's m_intValue = " << ( ( DataException* )pClonedException.get() )->m_intValue << std::endl;
            std::cout << "original's m_stringValue = \"" << dataException.m_stringValue << "\"" << std::endl;
            std::cout << "clone's m_stringValue = \"" << ( ( DataException* )pClonedException.get() )->m_stringValue << "\"" << std::endl;
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "ThrowNestedException" )
        {
            throw NestedException( m_pException.get() );
        }

        /* -------------------------------------------------------- */

        HH_TESTCMD( "NestException" )
        {
            m_pException = ( Ex::ExceptionPtr )new AnException( m_pException.get() );
        }
        
    }
    EX_CATCH_AND_LOG
}



void main( int argc, char*[] )
{
    try
    {
        Ex::ConsoleLogger consoleLogger;
        Ex::Logger::SetDefaultLogger( consoleLogger );

        utEx utEx( Ex::Exception::GetVariableData(), argc == 1 );
        utEx.Run();
    }
    EX_CATCH_AND_LOG
}
