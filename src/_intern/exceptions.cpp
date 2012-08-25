
#include "../../include/_intern.h"

namespace fff {
namespace _Intern {


String Exception::exception() const
{
    return
        getParent() + " " +
        location() + ": " +
        getMessage();
}

String Exception::location() const
{
    StringStream ss;
    ss << m_file << "[" << m_line << "]";
    return ss.str();
}



void Exception::print(logstream &stream) const
{
    stream(getParent());
    ~(stream >> "File" << getFile());
    ~(stream >> "Line" << getLine());
    ~(stream >> "Message" << getMessage());
    stream();
}

}
}