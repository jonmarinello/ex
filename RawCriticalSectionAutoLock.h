#ifndef _RAW_CRITICAL_SECTION_AUTO_LOCK_H_
#define _RAW_CRITICAL_SECTION_AUTO_LOCK_H_

#include <Win32\Win32.h>
#include <include\stdhdr.h>

namespace Ex
{
    /*
    * This class is used to lock a raw critical section for any classes
    * derived from Ex::Logger that need to protect their LogFormattedMessage()
    * method from cuncuurent access.
    *
    * Sync::CriticalSection cannot be used beacuse this would create
    * a cross-coupled dependency between the Sync and Ex name spaces.
    *
    * If you need a general purpose Cfritical Section warpper see the Sync
    * name space. This class should only be used by classes in the Ex
    * name space. No other consumers should ever use this class.
    */
    class RawCriticalSectionAutoLock
    {
    public:
        RawCriticalSectionAutoLock( CRITICAL_SECTION& rawCriticalSection )
            : m_rawCriticalSection( rawCriticalSection )
        {
            Win32::EnterCriticalSection( &m_rawCriticalSection );
        }

        ~RawCriticalSectionAutoLock()
        {
            Win32::LeaveCriticalSection( &m_rawCriticalSection );
        }

    private:
        NO_STANDARD_MEMBERS( RawCriticalSectionAutoLock ); // hide copy ctor() and = operator()

    private:
        CRITICAL_SECTION    m_rawCriticalSection;
    };
}

#endif //_RAW_CRITICAL_SECTION_AUTO_LOCK_H_
