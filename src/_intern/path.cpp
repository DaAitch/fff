

#include "../../include/_intern.h"


namespace fff {
namespace _Intern {

String path::getLastToken() const
{
    size_t lastToken = getPath().find_last_of(fff_DS) + 1;
    return getPath().substr(lastToken);
}

}
}