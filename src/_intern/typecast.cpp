
#include "../../include/_intern.h"


ostream &operator<<(ostream &stream, const stringified &rhs)
{
    stream << (String)rhs;
    return stream;
}