#ifndef _FILE_LOGGER_H_
#define _FILE_LOGGER_H_

#include <Ex\Logger.h>
#include <windows.h>

namespace Ex
{
    /**
    *  FileLogger is the concrete class used for logging to a file.
    */
    class FileLogger : public Logger
    {
    public:
        /**
        * The FileLogger constructor.
        */
        FileLogger()
        {
        }

        /**
        * The FileLogger destructor.
        */
        virtual ~FileLogger()
        {
            DtorResetDefaultLogger();
        }

    protected:
        /**
        * Logs the formatted message to a file.
        *
        * @param [in] formattedMessage - formatted message to log.
        */
        virtual void LogFormattedMessage( const std::string& formattedMessage )
        {
            // TBD: finish implementation
            formattedMessage; // keep compiler quiet for now
        }

    private:
        NO_STANDARD_MEMBERS( FileLogger ); // hide copy ctor() and = operator()
    };
}

#endif //_FILE_LOGGER_H_
