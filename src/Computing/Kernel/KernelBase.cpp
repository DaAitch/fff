
#include "../../../include/Computing/Kernel/KernelBase.h"
#include "../../../include/_intern.h"
#include "../../../include/Computing/Compiler.h"


using namespace fff::_Intern::RtCLC;
using namespace fff::_Intern::Exceptions::RtCLC;

namespace fff {
namespace Computing {
namespace Kernel {

KernelBase::KernelBase(
	const Compiler &compiler,
	const String &kernelName)
	:
    m_compiler(
		compiler),
    m_kernelName(
        kernelName),
    m_devprops(
        compiler.getEnv().getDevice())
{
    fff_EXPECT_VALID_OBJ(compiler);

	m_kernel = getCompiler().kernel(getKernelName());

    fff_EXPECT_VALID_CLOBJ(getKernel());
}

UInt KernelBase::argsCount() const
{
	fff_RTCLC_ERR_INIT();

	UInt count;

	fff_RTCLC_SEQ_CHECK(
		count = getKernel().getInfo<CL_KERNEL_NUM_ARGS>(
			fff_RTCLC_ERR_PTR));

	return count;
}

void KernelBase::enqueueNDRange(
    UInt global,
    UInt local)
{
    _enqueueNDRange(global, local);
}

void KernelBase::_enqueueNDRange(
    UInt global,
    UInt local)
{
    const ::cl::CommandQueue &queue =
        getCompiler().getEnv().getQueue();

    fff_EXPECT_VALID_CLOBJ(queue);

	fff_RTCLC_ERR_INIT();

    ::cl::NDRange
        globalRange(global),
        localRange(::cl::NullRange);
    
    if(local > 0)
    {
        fff_EXPECT(global % local, ==, 0);
        localRange = ::cl::NDRange(local);
    }

	fff_RTCLC_SEQ_CHECK_RET(
		queue.enqueueNDRangeKernel(
			getKernel(),
			::cl::NullRange,
			globalRange,
			localRange
			));

}

UInt KernelBase::smartWorkerLimit(UInt items) const
{
    while(items > getDeviceProperties().getMaxWorkerGroupCount())
        items = (items + 1) >> 1;

    return
        items;
}

UInt KernelBase::smartLb2WorkerLimit(UInt lb2Items) const
{
    UInt maxWorker = (UInt)getDeviceProperties().getMaxWorkerGroupCount();
    if(fff_POW2(lb2Items) > maxWorker)
        lb2Items = cl::lb2Multiple2In(maxWorker);

    return
        lb2Items;
}


UInt KernelBase::dontOptimizeArgs(
    UInt defaultLb2BlockN,
    UInt defaultLb2W,
    UInt defaultM,
	UInt startArgIndex)
{
    const Worker &worker =
        getCompiler().getWorker();

	if(!worker.isOptimized())
	{
	    fff_RTCLC_ERR_INIT();

	    fff_RTCLC_SEQ_CHECK_RET(
		    getKernel().setArg(startArgIndex++,
                defaultLb2BlockN));
	    fff_RTCLC_SEQ_CHECK_RET(
		    getKernel().setArg(startArgIndex++,
                defaultLb2W));
	    fff_RTCLC_SEQ_CHECK_RET(
		    getKernel().setArg(startArgIndex++,
                defaultM));
    }

	return startArgIndex;
}



}
}
}