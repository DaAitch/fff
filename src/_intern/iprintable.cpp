
#include "../../include/_intern.h"

using namespace fff::_Intern;
using namespace std;

ostream &operator<<(ostream &stream, const iprintable &printable)
{
    logstream ls(stream);
    printable.print(ls);
    return stream;
}

/*
logstream &operator<<(logstream &lhs, const iprintable &printable)
{
    printable.print(lhs);
    return
        lhs;
}
*/