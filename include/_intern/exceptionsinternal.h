
#ifndef ___internal_h__included__
#define ___internal_h__included__

#include <exception>
#include <string>

#include "_ns.h"

using namespace std;

namespace fff {
namespace _Intern {
namespace Exceptions {

class intern :
    public exception
{
public:
    intern(const char *msg)
        : exception(msg)
    {
    }

    using exception::operator=;
};

class exit :
    public exception
{
public:
    exit(const char *msg)
        : exception(msg)
    {
    }

    using exception::operator=;
};

}
}
}

#endif