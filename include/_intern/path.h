
#ifndef ___path_h__included__
#define ___path_h__included__

#include "_base.h"
#include "exceptionsinternal.h"
#include "_types.h"

#ifdef _fff_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

using namespace fff::_Intern::Exceptions;

namespace fff {
namespace _Intern {

#define fff_FILENAME(PATH) \
    (fff::_Intern::path( (PATH) ).getLastToken())

#define fff_FILE() \
    fff_FILENAME( __FILE__ )

#define fff_CSTRFILE() \
    fff_FILENAME( __FILE__ ).c_str()

#define fff_EXPANDPATH(PATH) \
    ((const char *)fff::_Intern::path( (PATH) ))

class path
{
public:
    path(const char *path)
    {
        #ifdef _fff_WIN
            DWORD size = ExpandEnvironmentStringsA(path, NULL, 0);
            char *p = new char[size+1];
            ExpandEnvironmentStringsA(path, p, size+1);
        #else
            // someone'd like to implement this?
            #error Use of this class might went wrong!
        #endif

        m_path = String(path);

        #ifdef _fff_WIN
            delete [] p;
        #endif
    }

    ~path() throw()
    {
    }

    String getLastToken() const;

    operator const char *() const
    {
        return
            getPath().c_str();
    }

    const String &getPath() const
    {
        return
            m_path;
    }

    bool operator!() const throw()
    {
        return
            false;
    }

private:
    String
        m_path;
};

}
}


#endif