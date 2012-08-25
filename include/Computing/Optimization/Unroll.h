
#ifndef __unroll_h__included__
#define __unroll_h__included__

#include "../../_intern.h"

namespace fff {
namespace Computing {
namespace Optimization {

class Unroll
    :
    public iprintable
{
public:
    Unroll(Bool unroll=false)
        :
        m_unroll(unroll)
    {
    }

    Bool isUnrolled() const throw()
    {
        return
            m_unroll;
    }

    StringStream &generateCompileOptions(
        StringStream &options) const
    {
        if(isUnrolled())
            options << " -D _fff_UNROLL ";

        return
            options;
    }

    virtual void print(logstream &out) const
    {
        out("Unroll");
        !fff_LOG_VAL(out, isUnrolled());
        out();
    }

    bool operator!() const
    {
        return false;
    }

private:
    Bool
        m_unroll;
};

}
}
}

#endif