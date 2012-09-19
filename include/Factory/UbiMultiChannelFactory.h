//---------------------------------------------------------+
// fff/include/Factory/UbiMultiChannelFactory.h
//---------------------------------------------------------+
//  License:
//    
//    The Fast Filtering Framework implements an LTI filter
//    with Khronos Group's OpenCL.
//    Copyright (C) 2012  Philipp Renoth <fff@aitch.de>
//
//    This program is free software: you can redistribute
//    it and/or modify it under the terms of the
//    GNU General Public License as published by the
//    Free Software Foundation, either version 3 of the
//    License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will
//    be useful, but WITHOUT ANY WARRANTY; without even the
//    implied warranty of MERCHANTABILITY or
//    FITNESS FOR A PARTICULAR PURPOSE.
//    See the GNU General Public License for more details.
//
//    You should have received a copy of the
//    GNU General Public License along with this program.
//    If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------+
//!
//!	\file		UbiMultiChannelFactory.h
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Information of computation data.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifndef __ubimultichannelfactory_h__included__
#define __ubimultichannelfactory_h__included__

#include "../Buffer/Complex/Ubiquitous/UbiMultiChannel.h"

#include "../Buffer/Complex/Host/IHostMultiChannel.h"
#include "../Buffer/Complex/Host/HostMultiChannel.h"
#include "../Buffer/Complex/Host/HostMultiChannelPreallocated.h"

#include "../Buffer/Complex/Device/DevMultiChannel.h"

using namespace fff::Buffer::Complex::Host;
using namespace fff::Buffer::Complex::Device;

namespace fff {
namespace Factory {

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

#endif