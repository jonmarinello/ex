#ifndef _EX_H_
#define _EX_H_

#include <string>
#include <memory>
#include <sstream>
#include <exception>
#include <windows.h>
#include <exception>
#include <boost\shared_ptr.hpp>


/**
* @namespace Ex Contains the exception and logging framework.
*/
namespace Ex
{
    /*
    * Forward declaration needed for ExceptionPtr.
    */
    class Exception;


    /**
    * The Exception shared pointer type
    */
    typedef boost::shared_ptr< Exception > ExceptionPtr;


    /**
    *  Exception is the abstract base class that all your exceptions
    *  should be derived from.
    *
    *  You must implement the constructor in your derived class
    *  and use the INSERT_EXCEPTION_DEFS( className ) macro.
    */
    class Exception
    {
    public:
        /**
        * The Exception constructor.
        *
        * @param [in] pExceptionToNest - optional exception to nest.
        */
        Exception( Exception* pExceptionToNest = 0 );


        /**
        * The Exception copy constructor.
        *
        * @param [in] exceptionToCopy - exception to copy.
        */
        Exception( const Exception& exceptionToCopy );


        /**
        * The Exception destructor.
        */
        virtual ~Exception() {}


        /**
        * GetName()
        *
        * Pure virtual GetName method. Derived classes must implement using
        * INSERT_EXCEPTION_DEFS().
        */
        virtual std::string GetName() const = 0;


        /**
        * Clone()
        *
        * Pure virtual Clone method. Derived classes must implement using
        * INSERT_EXCEPTION_DEFS().
        */
        virtual ExceptionPtr Clone() const = 0;


        /**
        * AsString()
        *
        * Returns a string representaion of the Exception.
        */
        std::string AsString() const;


        /**
        * Log()
        *
        * Logs the Exception to the event log with a message.
        *
        * @param [in] message - message to log at top of exception stack.
        */
        void Log( const std::string& message ) const;


        /**
        * SafeLog()
        *
        * Logs the Exception to the event log with a message. This version
        * of logging does not throw exceptions and is intended to be used in
        * destructors.
        *
        * @param [in] message - message to log at top of exception stack.
        */
        void SafeLog( const std::string& message ) const;


        /**
        * AddStackFrame
        *
        * @param [in] fileName     - the file where the exception was caught.
        * @param [in] functionName - the function the exception was caught in.
        * @param [in] line         - the line where the exception was caught.
        */
        void AddStackFrame( const char* fileName, const char* functionName, int line );


        /**
        * Returns a reference to the static global s_variableData for use in
        * unit testing.
        */
        static bool& GetVariableData() { return s_variableData; }

    protected:
        /**
        * ExceptionPtr
        */
        ExceptionPtr        m_pNestedException; // Optional nested exception

        /**
        * Stack back trace.
        */
        std::string         m_stack;            // The string containing the stack back trace

        /**
        * Exception stream that contains the error message as a stream created in
        * derieved class' constructor.
        */
        std::stringstream   m_stream;           // The stream containing the error message
        

        /**
        * LocationAsString
        *
        * @param [in] fileName     - the file nama.
        * @param [in] functionName - the name of the function.
        * @param [in] lineNumber   - the line number.
        */
        static std::string LocationAsString( const char* fileName, const char* functionName, int lineNumber );


        /**
        * StreamInto()
        *
        * @param [in,out] stream - stream to stream into.
        *
        * Streams Exception into stream.
        */
        void StreamInto( std::stringstream& stream ) const;

    private:
        /**
        * s_variableData
        */
        static bool s_variableData;
        
        Exception& operator=( const Exception& ); // Hide assignment operator
    };

    /**
    * This macro must be inserted into each derived class definition 
    */
    #define INSERT_EXCEPTION_DEFS( className )                              \
        public:                                                             \
            std::string GetName() const { return #className; }              \
            Ex::ExceptionPtr Clone() const                                  \
            {                                                               \
                return ( Ex::ExceptionPtr )new className( *this );          \
            }                                                               \
                                                                            \
        private:                                                            \
            className& operator=( const className& );                       \
                                                                            \
        public:

    /**
    * The NamedException base class - all programmer defined named exceptions
    * should be dreived from this abstract base class.
    */
    class NamedException : public Exception
    {
    protected:
        /**
        *  Constructor
        *
        *  @param [in] pExecptionToNest - optional exception to nest.
        */
        NamedException( Exception* pExecptionToNest = 0 )
            : Exception( pExecptionToNest )
        {
        }

        /**
        * framework required exception macro
        */
        INSERT_EXCEPTION_DEFS( NamedException )
    };


    /**
    *  STL exception wrapper.
    */
    class StlException : public NamedException
    {
    public:
        /**
        *  Constructor
        *
        *  @param [in] what - the string representing the STL exception.
        *  @param [in] pExecptionToNest - optional exception to nest.
        */
        StlException( const std::string& what, Exception* pExecptionToNest = 0 );

        /**
        * framework required exception macro
        */
        INSERT_EXCEPTION_DEFS( StlException );
    };


    /**
    * The ProgrammerException base class - all programmer error exceptions
    * should be dreived from this abstract base class.
    */
    class ProgrammerException : public Exception
    {
    protected:
        /**
        *  Constructor
        *
        *  @param [in] pExecptionToNest - optional exception to nest.
        */
        ProgrammerException( Exception* pExecptionToNest = 0 )
            : Exception( pExecptionToNest )
        {
        }

        /**
        * framework required exception macro
        */
        INSERT_EXCEPTION_DEFS( ProgrammerException )
    };



    /**
    * The standard Assert exception class.
    */
    class Assert : public ProgrammerException
    {
    public:
        /**
        *  Constructor
        *
        *  @param [in] predicate - the predicate that failed and caused this exception to be thrown,.
        *  @param [in] fileName - the file the error occured in.
        *  @param [in] functionName - the function the error occured in.
        *  @param [in] line - the line the error occured on.
        *  @param [in] pExecptionToNest - optional exception to nest.
        */
        Assert( const char* predicate, const char* fileName, const char* functionName, 
                      int line, Exception* pExecptionToNest = 0 );

        /**
        * framework required exception macro
        */
        INSERT_EXCEPTION_DEFS( Assert );
    };



    /**
    * The standard Precondition exception class.
    */
    class Precondition : public ProgrammerException
    {
    public:
        /**
        *  Constructor
        *
        *  @param [in] predicate - the predicate that failed and caused this exception to be thrown,.
        *  @param [in] fileName - the file the error occured in.
        *  @param [in] functionName - the function the error occured in.
        *  @param [in] line - the line the error occured on.
        *  @param [in] pExecptionToNest - optional exception to nest.
        */
        Precondition( const char* predicate, const char* fileName, const char* functionName, 
                      int line, Exception* pExecptionToNest = 0 );

        /**
        * framework required exception macro
        */
        INSERT_EXCEPTION_DEFS( Precondition );
    };


    /**
    *  Use with the EX_ASSERT and EX_PRECONDITION macros to avoid compiler warning C4127.
    */
    bool False();
}

#define EX_CATCH_AND_RETHROW( exceptionName )                                               \
    catch ( exceptionName& ex )                                                             \
    {                                                                                       \
        ex.AddStackFrame( __FILE__, __FUNCTION__, __LINE__ );                               \
        throw;                                                                              \
    }


#define EX_CATCH_AND_IGNORE( exceptionName )                                                \
    catch ( exceptionName& )                                                                \
    {                                                                                       \
    }


#define EX_CATCH_AND_LOG                                                                    \
    EX_CATCH_AND_LOG_WITH_MESSAGE( __FUNCTION__ )


#define EX_CATCH_AND_LOG_WITH_MESSAGE( message )                                            \
    catch ( Ex::Exception& ex )                                                             \
    {                                                                                       \
        ex.AddStackFrame( __FILE__, __FUNCTION__, __LINE__ );                               \
        ex.Log( message  );                                                                 \
    }                                                                                       \
    catch ( exception& stlEx )                                                              \
    {                                                                                       \
        Ex::StlException ex( stlEx.what() );                                                \
        ex.AddStackFrame( __FILE__, __FUNCTION__, __LINE__ );                               \
        ex.Log( message  );                                                                 \
    }


#define EX_DTOR_CATCH_AND_LOG                                                               \
    catch ( Ex::Exception& ex )                                                             \
    {                                                                                       \
        ex.SafeLog( __FUNCTION__ );                                                         \
    }                                                                                       \
    catch ( exception& stlEx )                                                              \
    {                                                                                       \
        Ex::StlException ex( stlEx.what() );                                                \
        ex.SafeLog( __FUNCTION__ );                                                         \
    }


#define EX_STACK_BACKTRACE                                                                  \
    catch ( Ex::Exception& ex )                                                             \
    {                                                                                       \
        ex.AddStackFrame( __FILE__, __FUNCTION__, __LINE__ );                               \
        throw;                                                                              \
    }


#define EX_ASSERT( predicate )                                                              \
    if ( !( predicate ) )                                                                   \
    {                                                                                       \
        throw Ex::Assert( #predicate, __FILE__, __FUNCTION__, __LINE__ );                   \
    }


#define EX_PRECONDITION( predicate )                                                        \
    if ( !( predicate ) )                                                                   \
    {                                                                                       \
        throw Ex::Precondition( #predicate, __FILE__, __FUNCTION__, __LINE__ );             \
    }

#define EX_PRECONDITION1( predicate, value )                                                \
    if ( !( predicate ) )                                                                   \
    {                                                                                       \
        std::stringstream stream;                                                           \
        stream << #predicate ", " #value " = " << value;                                    \
        throw Ex::Precondition( stream.str().c_str(), __FILE__, __FUNCTION__, __LINE__ );   \
    }   

#define EX_PRECONDITION2( predicate, value1, value2 )                                      \
    if ( !( predicate ) )                                                                  \
    {                                                                                      \
        std::stringstream stream;                                                          \
        stream << #predicate ", " #value1 " = " << value1 << ", " #value2 " = " << value2; \
        throw Ex::Precondition( stream.str().c_str(), __FILE__, __FUNCTION__, __LINE__ );  \
    }   
#endif
