

#ifndef __ubimultichannelfactory_h__included__
#define __ubimultichannelfactory_h__included__

#include "UbiMultiChannel.h"

#include "../Host/IHostMultiChannel.h"
#include "../Host/HostMultiChannel.h"
#include "../Host/HostMultiChannelPreallocated.h"

#include "../Device/DevMultiChannel.h"

using namespace fff::Buffer::Complex::Host;
using namespace fff::Buffer::Complex::Device;

namespace fff {
namespace Buffer {
namespace Complex {
namespace Ubiquitous {

    template<
        class SampleType
    >
    class UbiMultiChannelFactory
    {
    public:
        typedef UbiMultiChannel<SampleType> umc;
        typedef IHostMultiChannel<SampleType> ihmc;
        typedef DevMultiChannel<SampleType> dmc;
        typedef HostMultiChannel<SampleType> hmc;
        typedef HostMultiChannelPreallocated<SampleType> hmcp;

        struct multichannel
        {
        public:
            bool
                killubi,
                killhost,
                killdev;

            umc *ubi;
            ihmc *host;
            dmc *dev;

            multichannel() : ubi(NULL), host(NULL), dev(NULL), killubi(false), killhost(false), killdev(false) {}
            bool operator!() const throw() { return !ubi||!host||!dev||!*ubi||!*host||!*dev; }
            umc &operator*() { return *ubi; }
            ~multichannel()
            {
                if(host&&killhost)
                {
                    delete host;
                    killhost=false;
                    host=NULL;
                }
                if(dev&&killdev)
                {
                    delete dev;
                    killdev=false;
                    dev=NULL;
                }
                if(ubi&&killubi)
                {
                    delete ubi;
                    killubi=false;
                    ubi=NULL;
                }
            }
        };

        static void create(
            multichannel &mc,
            dmc &d,
            Bool prealloc=False,
            Bool blockingRWs=False)
        {
            mc = multichannel();

            mc.dev = &d;
            mc.killdev=false;

            if(prealloc)
            {
                hmcp *hcp =
                    new hmcp;
                hcp->alloc(d.getChannelCount(), d.getSampleCount());
                mc.host = hcp;
            }
            else
                mc.host = new hmc(d.getChannelCount(), d.getSampleCount());

            mc.ubi = new umc(*mc.host, *mc.dev, blockingRWs);

            mc.killhost = true;
            mc.killubi = true;
        }

        static void create(
            multichannel &mc,
            OpenCLEnvironment &env,
            cl_mem_flags flags,
            UInt channelCount,
            UInt sampleCount,
            Bool prealloc=False,
            Bool blockingRWs=False)
        {
            mc = multichannel();

            if(prealloc)
            {
                hmcp *hcp =
                    new hmcp;
                hcp->alloc(channelCount, sampleCount);
                mc.host = hcp;
            }
            else
                mc.host = new hmc(channelCount, sampleCount);

            
            mc.dev = new dmc(env, flags, channelCount, sampleCount);

            mc.ubi = new umc(*mc.host, *mc.dev, blockingRWs);

            mc.killdev = true;
            mc.killhost = true;
            mc.killubi = true;
        }
    };

}
}
}
}

#endif