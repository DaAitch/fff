
#ifndef __fffcompileoptimization_h__included__
#	define __fffcompileoptimization_h__included__
	
#include "../../_intern.h"
#include "../../_intern/exceptionsruntime.h"
#include "../../../cl/_.h.cl"

using namespace ::fff::_Intern::Exceptions::Runtime;

namespace fff {
namespace Computing {
namespace Optimization {

class Worker
    :
    public iprintable
{
public:
// to be implemented
/*
	Worker(
		UInt blockLb2N,
		UInt lb2W,
		UInt M
		)
		: m_blockLb2N(lb2N),
		m_lb2W(lb2W),
		m_M(M),
		m_optimized(True)
	{
	}
    */

	Worker()
		:
        m_lb2BlockN(0),
		m_lb2W(0),
		m_M(0),
		m_optimized(False)
	{
	}
    
	UInt getLb2BlockN() const
	{
		return
			m_lb2BlockN;
	}

	UInt getBlockN() const
	{
		return
			(UInt)1<<getLb2BlockN();
	}

	UInt getM() const
	{
		return
			m_M;
	}

	UInt getLb2W() const
	{
		return
			m_lb2W;
	}

	UInt getW() const
	{
		return
			(UInt)1<<getLb2W();
	}

	Bool isOptimized() const throw()
	{
		return
			m_optimized;
	}

	StringStream &generateCompileOptions(
		StringStream &options) const
	{
		if(isOptimized())
		{
            options << " ";
			options << " -D _fff_OPTIMIZE ";
			options << " -D _fff_OPTIMIZE_PARAM_LB2N=" << getLb2BlockN();
			options << " -D _fff_OPTIMIZE_PARAM_LB2W=" << getLb2W();
			options << " -D _fff_OPTIMIZE_PARAM_M=" << getM();
			options << " ";
		}

		return options;
	}

    Bool isMOkay(UInt M) const throw()
    {
        if(isOptimized())
            return M == m_M ? True : False;

        return True;
    }

    Bool isBlockNOkay(UInt blockN) const throw()
    {
        if(isOptimized())
            return blockN == getBlockN() ? True : False;

        return True;
    }

    Bool isWOkay(UInt W) const throw()
    {
        if(isOptimized())
            return W == getW() ? True : False;

        return True;
    }

    virtual void print(logstream &out) const
    {
        out("Worker");
        !fff_LOG_VAL(out, isOptimized());
        !fff_LOG_VAL(out, getLb2BlockN());
        !fff_LOG_VAL(out, getLb2W());
        !fff_LOG_VAL(out, getM());
        out();
    }

    bool operator!() const throw()
    {
        return
            false;
    }


private:
	Bool
		m_optimized;

	UInt
		m_lb2BlockN,
		m_lb2W,
		m_M;
};

	
}
}
}
#endif