#include <Windows.h>
#include "Ex\Ex.h"
#include "Ex\ConsoleLogger.h"
#include <iostream>


bool Ex::Exception::s_variableData = true;

bool Ex::False()
{
    return false;
}

/*
 * The Exception  contructor.
 */
Ex::Exception::Exception( Exception* pExceptionToNest )
    : m_pNestedException( pExceptionToNest ? pExceptionToNest->Clone() : ExceptionPtr() ),
      m_stack( ),
      m_stream( )
{
}


/*
 * The Exception copy contructor.
 */
Ex::Exception::Exception( const Exception& exceptionToCopy )
    : m_pNestedException( exceptionToCopy.m_pNestedException ),
      m_stack( exceptionToCopy.m_stack ),
      m_stream( exceptionToCopy.m_stream.str() )    
{
}


/*
 * AsString()
 *
 * Returns a string representaion of the Exception.
 */
std::string Ex::Exception::AsString() const
{
    std::stringstream   stream; 

    stream << GetName() << " : " << m_stream.str() << m_stack;
    if ( m_pNestedException.get() != 0 )
    {
        stream << std::endl << "Caused by: ";
        m_pNestedException->StreamInto( stream );
    }
    return stream.str();
}


/*
 * Log()
 *
 * Logs the Exception to the default logger with a message.
 */
void Ex::Exception::Log( const std::string& message ) const
{
    Logger::GetDefaultLogger().Log( message + "\n" + AsString() );
}


/*
 * SafeLog()
 *
 * Logs the Exception to the event log with a message. This version
 * of logging does not throw exceptions and is intended to be used in
 * destructors.
 *
 */
void Ex::Exception::SafeLog( const std::string& message ) const
{
    try
    {
        Log( message );
    }
    catch( Exception& )
    {
        // don't do anything in this case (we are in a destructor).
    }
    catch ( exception& )
    {
        // don't do anything in this case (we are in a destructor).
    }
}


/*
 * StreamInto()
 *
 * Streams a string representaion of the Exception into stream.
 */
void Ex::Exception::StreamInto( std::stringstream& stream ) const
{
    stream << AsString();
}


/*
 * LocationAsString
 */
std::string Ex::Exception::LocationAsString( const char* fileName, const char* functionName, int lineNumber )
{
    std::string fileNameString;

    if ( !GetVariableData() )
    {
        lineNumber = 0;
        char *p = strrchr( fileName, '\\' ); // get last backslash
        if ( p )
        {
            fileNameString = p + 1;
        }
        else
        {
            fileNameString = fileName;
        }
        strlwr( const_cast< char* > ( fileNameString.c_str() ) );
    }
    else
    {
        fileNameString = fileName;
    }

    std::stringstream fileAndLine;
    fileAndLine << functionName << " (" << fileNameString << ":" << lineNumber << ")";
    return fileAndLine.str();
}


/*
 * AddStackFrame
 */
void Ex::Exception::AddStackFrame( const char* fileName, const char* functionName, int line )
{
    m_stack = std::string( "\n    at " ) + LocationAsString( fileName, functionName, line ) + m_stack;
}


/*
 *  STL exception wrapper.
 */
Ex::StlException::StlException( const std::string& what, Exception* pExecptionToNest )
    : NamedException( pExecptionToNest )
{
    m_stream << "STL Exception \"" << what << "\"";
}


/*
 *  Assertion programmer error exception.
 */
Ex::Assert::Assert( const char* predicate, const char* fileName, const char* functionName, int line, Exception* pExecptionToNest )
    : ProgrammerException( pExecptionToNest )
{
    m_stream << "Assert( \"" << predicate << "\" ) failed in file " << LocationAsString( fileName, functionName, line );
}


/*
 *  Precondition programmer error exception.
 */
Ex::Precondition::Precondition( const char* predicate, const char* fileName, const char* functionName, int line, Exception* pExecptionToNest )
    : ProgrammerException( pExecptionToNest )
{
    m_stream << "Precondition( \"" << predicate << "\" ) failed in file " << LocationAsString( fileName, functionName, line );
}
