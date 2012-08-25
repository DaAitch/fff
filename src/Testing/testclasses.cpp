//---------------------------------------------------------+
// fff/src/Testing/Classes.cpp
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
//!	\file		Classes.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Testing FFF classes.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifdef _TEST

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../../include/Testing/Base/_testingbase.h"
#include "../../include/Testing/testclasses.h"
#include "../../include/buffer.h"
#include "../../include/computing.h"
#include "../../include/filesystem.h"
//---------------------------------------------------------+


//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
using namespace fff;
using namespace fff::Testing::Classes;
using namespace fff::Testing::Base;

using namespace fff::_Intern;
using namespace fff::_Intern::Exceptions;
using namespace fff::_Intern::Exceptions::RtCLC;
using namespace fff::_Intern::Exceptions::Runtime;
using namespace fff::_Intern::RtCLC;
using namespace fff::Computing;
using namespace fff::Computing::Kernel;
using namespace fff::Computing::Optimization;
using namespace fff::Computing::RuntimeMapping;
using namespace fff::Buffer;
using namespace fff::Buffer::Complex;
using namespace fff::Buffer::Complex::Device;
using namespace fff::Buffer::Complex::Host;
using namespace fff::Buffer::Complex::Ubiquitous;
using namespace fff::Filesystem;

//---------------------------------------------------------+
//! IMPLEMENTATION
//!=========================================================
namespace fff {
namespace Testing {

fff_TEST_BEGIN_IMPL(fff, classes)
    fff_TEST_CALL(classes, OpenCLEnvironment);

    fff_TEST_CALL(classes, Worker);
    fff_TEST_CALL(classes, Unroll);

    fff_TEST_CALL(classes, Compiler);

    fff_TEST_CALL(classes, Vector);
    fff_TEST_CALL(classes, ComplexVector);

    fff_TEST_CALL(classes, ComputingData);
    fff_TEST_CALL(classes, DeviceProperties);
    fff_TEST_CALL(classes, Mapper);
    
    fff_TEST_CALL(classes, DevSingleChannel);

    fff_TEST_CALL(classes, HostSingleChannel);
    fff_TEST_CALL(classes, HostMultiChannel);
    fff_TEST_CALL(classes, HostMultiChannelPreallocated);

    fff_TEST_CALL(classes, UbiMultiChannelBase);
    fff_TEST_CALL(classes, UbiMultiChannel);
    
    fff_TEST_CALL(classes, Convolution);
    
    fff_TEST_CALL(classes, Transformation);
    fff_TEST_CALL(classes, TransferFunction);
    
    fff_TEST_CALL(classes, FastConvolution);

fff_TEST_END_IMPL()

namespace Classes {

//---------------------------------------------------------+
//! \brief Test class Vector.
fff_TEST_BEGIN_IMPL(classes, Vector)

	Vector<Sample> smp;
	
	fff_TEST_FALSE(smp.isAllocated());
	smp.alloc(4);
	smp[0] = 1.f;
	smp[1] = 2.f;
	smp[2] = 3.f;
	smp[3] = 4.f;

	fff_TEST_TRUE(
		smp[0] == 1.f &&
		smp[1] == 2.f &&
		smp[2] == 3.f &&
		smp[3] == 4.f);

	Vector<Sample> smp2;
	fff_TEST_FALSE(smp2.isAllocated());
	smp2 = smp;
	smp.dealloc();

	fff_TEST_TRUE(
		smp2[0] == 1.f &&
		smp2[1] == 2.f &&
		smp2[2] == 3.f &&
		smp2[3] == 4.f);

fff_TEST_END_IMPL()
//---------------------------------------------------------+

//---------------------------------------------------------+
fff_TEST_BEGIN_IMPL(classes, ComplexVector)

	ComplexVector<Sample> cs;
	fff_TEST_FALSE(cs.isAllocated());
		
	cs.alloc(4);
	fff_TEST_TRUE(cs.isAllocated());

	cs.init();
	fff_TEST_TRUE(cs.getReal()[0] == 0.f);
	fff_TEST_TRUE(cs.getReal()[3] == 0.f);
	fff_TEST_TRUE(cs.getImag()[0] == 0.f);
	fff_TEST_TRUE(cs.getImag()[3] == 0.f);

	cs.dealloc();
	fff_TEST_FALSE(cs.isAllocated());

	cs.alloc(2);
	cs.init();
	cs.getReal()[0] = 1.f;
	cs.getImag()[0] = 2.f;
	cs.getReal()[1] = 3.f;
	cs.getImag()[1] = 4.f;

	fff_TEST_TRUE(
		cs.getReal()[0] == 1.f &&
		cs.getReal()[1] == 3.f &&
		cs.getImag()[0] == 2.f &&
		cs.getImag()[1] == 4.f);

	ComplexVector<Sample> cs2;
	fff_TEST_FALSE(cs2.isAllocated());
	cs2 = cs;
	cs.dealloc();
	fff_TEST_TRUE(cs2.isAllocated());
	fff_TEST_TRUE(cs2.getImag()[0] == 2.f);

fff_TEST_END_IMPL()
//---------------------------------------------------------+

fff_TEST_BEGIN_IMPL(classes, OpenCLEnvironment)

    {
    OpenCLEnvironment env;
    fff_TEST_NOK(env);

    fff_TEST_TRY_CATCH
    (
        env.getContext();
        ,
        InvalidCLObjectException
    );

    fff_TEST_TRY_CATCH
    (
        env.getDevice();
        ,
        InvalidCLObjectException
    );

    fff_TEST_TRY_CATCH
    (
        env.getPlatform();
        ,
        InvalidCLObjectException
    );

    fff_TEST_TRY_CATCH
    (
        env.getQueue();
        ,
        InvalidCLObjectException
    );
    }

    {
    OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
    fff_TEST_OK(env);

    OpenCLEnvironment env2;
    fff_TEST_NOK(env2);

    env2 = env;
    fff_TEST_OK(env);
    fff_TEST_OK(env2);

    fff_TEST_TRUE(
        env.getContext()() == env2.getContext()() &&
        env.getDevice()() == env2.getDevice()() &&
        env.getPlatform()() == env2.getPlatform()() &&
        env.getQueue()() == env2.getQueue()());

    }

fff_TEST_END_IMPL()


fff_TEST_BEGIN_IMPL(classes, Worker)
    
    {
        Worker w;
        fff_TEST_OK(w);
        fff_TEST_FALSE(w.isOptimized());
    }

    /*
    // N = 8
    // W = 1
    // M = 5
    {
        Worker w(3,0,5);
        fff_TEST_OK(w);
        fff_TEST_TRUE(w.isOptimized());
    }
    */

fff_TEST_END_IMPL()


fff_TEST_BEGIN_IMPL(classes, Unroll)
    {
        Unroll u;
        fff_TEST_OK(u);
        fff_TEST_FALSE(u.isUnrolled());
    }
    {
        Unroll u1(True), u2(False);
        fff_TEST_OK(u1);
        fff_TEST_OK(u2);
        fff_TEST_TRUE(u1.isUnrolled());
        fff_TEST_FALSE(u2.isUnrolled());

        u1 = u2;
        fff_TEST_OK(u1);
        fff_TEST_OK(u2);
        fff_TEST_FALSE(u1.isUnrolled());
        fff_TEST_FALSE(u2.isUnrolled());
    }
    {
        Unroll u1(True), u2(u1);
        fff_TEST_OK(u1);
        fff_TEST_OK(u2);
        fff_TEST_TRUE(u2.isUnrolled());
    }
    {
        Unroll u1(True), u2(False);
        StringStream s1, s2;
        fff_TEST_TRUE(
            u1.generateCompileOptions(s1).str().find(
                String("_fff_UNROLL")) != String::npos);
        fff_TEST_TRUE(
            u2.generateCompileOptions(s2).str().find(
                String("_fff_UNROLL")) == String::npos);
    }
fff_TEST_END_IMPL()


fff_TEST_BEGIN_IMPL(classes, Compiler)

    OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);

    {
        Compiler c;
        fff_TEST_NOK(c);
        fff_TEST_TRY_CATCH(
            c.getEnv();
            ,
            InvalidObjectException
        );

        fff_TEST_TRY_CATCH(
            c.getProgram();
            ,
            InvalidCLObjectException
        );

        fff_TEST_OK(c.getWorker());
        fff_TEST_FALSE(c.getWorker().isOptimized());

        fff_TEST_FALSE(c.isCompiled());
    }

    {
        Compiler c(env, "cl/");

        fff_TEST_NOK(c);
        fff_TEST_NNULL(c.getProgram()());

        fff_TEST_FALSE(c.isCompiled());

        // compiler not build
        fff_TEST_TRY_CATCH
        (
            c.kernel(fff_STRINGIFY(fff_KERNEL_DCONV));
            ,
            ProgramNotCompiledException
        );


        for(UInt i = 0; i < 3; ++i)
        {
            // test if crash if building 2x or 3x
            c.build();
            fff_TEST_TRUE(c.isCompiled());

            fff_TEST_NNULL(
                c.kernel(fff_STRINGIFY(fff_KERNEL_DCONV))());
            fff_TEST_NNULL(
                c.kernel(fff_STRINGIFY(fff_KERNEL_FCONV))());
            fff_TEST_NNULL(
                c.kernel(fff_STRINGIFY(fff_KERNEL_DFT))());
            fff_TEST_NNULL(
                c.kernel(fff_STRINGIFY(fff_KERNEL_IDFT))());
            fff_TEST_NNULL(
                c.kernel(fff_STRINGIFY(fff_KERNEL_FFT))());
            fff_TEST_NNULL(
                c.kernel(fff_STRINGIFY(fff_KERNEL_IFFT))());
            fff_TEST_NNULL(
                c.kernel(fff_STRINGIFY(fff_KERNEL_TFUNC))());
        }
    }

    {
        Compiler c(env, "WrongPath/");

        fff_TEST_FALSE(c.isCompiled());
        fff_TEST_NOK(c);

        fff_TEST_TRY_CATCH
        (
            c.build();
            ,
            fff::_Intern::Exceptions::Runtime::ProgramBuildException
        )

        fff_TEST_FALSE(c.isCompiled());
    }

    fff_TEST_BEGIN_SECTION("Default compiler")
        Compiler c(env, "cl/");
        fff_TEST_NOK(c);

        fff_TEST_OK(c.getEnv());
        fff_TEST_OK(c.getUnroll());
        fff_TEST_OK(c.getWorker());
        fff_TEST_FALSE(c.isCompiled());
    fff_TEST_END_SECTION()

    fff_TEST_BEGIN_SECTION("Unrolled compiler")
        Compiler c(env, "cl/", Worker(), Unroll(True));
        c.build();

        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_DCONV))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_FCONV))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_DFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_IDFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_FFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_IFFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_TFUNC))());
    fff_TEST_END_SECTION()

    // future implementation
    /*
    fff_TEST_BEGIN_SECTION("Optimized compiler")
        Compiler c(env, "cl/", Worker(10, 9, 10));
        c.build();

        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_DCONV))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_FCONV))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_DFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_IDFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_FFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_IFFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_TFUNC))());
    fff_TEST_END_SECTION()
    */

    /*
    fff_TEST_BEGIN_SECTION("Unrolled, Optimized compiler")
        Compiler c(env, Compiler::CL_LOCATION, Worker(10, 9, 10), Unroll(True));
        c.build();

        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_DCONV))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_FCONV))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_DFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_IDFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_FFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_IFFT))());
        fff_TEST_NNULL(
            c.kernel(fff_STRINGIFY(fff_KERNEL_TFUNC))());
    fff_TEST_END_SECTION()
    */

fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, ComputingData)
    {
        ComputingData<Float> d(100);
        ComputingData<Double> b(10);

        fff_TEST_OK(d);
        fff_TEST_OK(b);

        fff_TEST_EQ(d.getDataTypeSize(), sizeof(Float));
        fff_TEST_EQ(d.getKernelElementsCount(), 100);

        fff_TEST_EQ(b.getDataTypeSize(), sizeof(Double));
        fff_TEST_EQ(b.getKernelElementsCount(), 10);

        ComputingData<Float> e(d); // test copy ctor
        fff_TEST_OK(e);
        fff_TEST_EQ(e, d);

        ComputingData<Float> x(20);
        fff_TEST_OK(x);

        e = x; // test operator=
        fff_TEST_NEQ(e, d);
        fff_TEST_EQ(e, x);

        // zero argument
        fff_TEST_TRY_CATCH(
            ComputingData<Float> q(0);
            ,
            InvalidRelationException
        );

        


    }
fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, DeviceProperties)
    {
        ::cl::Device dev;
        fff_TEST_TRY_CATCH(
            DeviceProperties devprops(dev);
            ,
            InvalidCLObjectException
        );
    }
    {
        OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
        fff_TEST_OK(env);

        DeviceProperties devprops(env.getDevice());
        fff_TEST_OK(devprops);

        fff_TEST_GT(devprops.getComputingUnitsCount(), 0);
        fff_TEST_GTE(devprops.getConstantMemorySize(), 0);
        fff_TEST_GTE(devprops.getGlobalMemorySize(), 0);
        fff_TEST_GTE(devprops.getLocalMemorySize(), 0);
        fff_TEST_GT(devprops.getMaxWorkerGroupCount(), 0);

        devprops.print(logstream());

        DeviceProperties devprops2(devprops);

        fff_TEST_TRUE(
            devprops2==devprops
            );
    }
fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, Mapper)

    UInt lb2W, lb2N;

    // test increase throughput and
    // max worker limit
    // TODO check lb2N !!!
    {
        Mapper::staticCalculate(
            lb2N, lb2W,
            10, 0, 128);
        fff_TEST_EQ(lb2N, 4);
        fff_TEST_EQ(lb2W, 3);

        Mapper::staticCalculate(
            lb2N, lb2W,
            10, 1, 128);
        fff_TEST_EQ(lb2N, 5);
        fff_TEST_EQ(lb2W, 4);

        Mapper::staticCalculate(
            lb2N, lb2W,
            10, 2, 128);
        fff_TEST_EQ(lb2N, 6);
        fff_TEST_EQ(lb2W, 5);

        Mapper::staticCalculate(
            lb2N, lb2W,
            10, 3, 128);
        fff_TEST_EQ(lb2N, 7);
        fff_TEST_EQ(lb2W, 6);

        Mapper::staticCalculate(
            lb2N, lb2W,
            10, 4, 128);
        fff_TEST_EQ(lb2N, 8);
        fff_TEST_EQ(lb2W, 7);

        // 2^^7=128 (max worker)
        Mapper::staticCalculate(
            lb2N, lb2W,
            10, 5, 128);
        fff_TEST_EQ(lb2N, 9);
        fff_TEST_EQ(lb2W, 7);

        Mapper::staticCalculate(
            lb2N, lb2W,
            10, 5, 128);
        fff_TEST_EQ(lb2N, 9);
        fff_TEST_EQ(lb2W, 7);
    }

    // test limits and borders
    {
        Mapper::staticCalculate(
            lb2N, lb2W,
            1024, 0, 1024);
        fff_TEST_EQ(lb2N, 11);
        fff_TEST_EQ(lb2W, 10);

        Mapper::staticCalculate(
            lb2N, lb2W,
            1023, 0, 1024);
        fff_TEST_EQ(lb2N, 10);
        fff_TEST_EQ(lb2W, 9);

        Mapper::staticCalculate(
            lb2N, lb2W,
            1025, 0, 1024);
        fff_TEST_EQ(lb2N, 11);
        fff_TEST_EQ(lb2W, 10);


        Mapper::staticCalculate(
            lb2N, lb2W,
            1023, 1, 1023);
        fff_TEST_EQ(lb2N, 11);
        fff_TEST_EQ(lb2W, 9);

        Mapper::staticCalculate(
            lb2N, lb2W,
            1024, 2, 1023);
        fff_TEST_EQ(lb2N, 13);
        fff_TEST_EQ(lb2W, 9);

        Mapper::staticCalculate(
            lb2N, lb2W,
            1024, 2, 1024);
        fff_TEST_EQ(lb2N, 13);
        fff_TEST_EQ(lb2W, 10);
    }

    OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
    ComputingData<Float> cdf(100);

    DeviceProperties devprops(env.getDevice());
    
    {
        Mapper m(cdf, devprops, 2);
        fff_TEST_OK(m);

        m.getLb2N();
        m.getLb2W();

        fff_TEST_EQ(m.getF(), 2);

        Mapper n(m);
        fff_TEST_EQ(n, m);

        Mapper x(cdf, devprops, 4);
        fff_TEST_OK(x);
        fff_TEST_NEQ(n, x);
        fff_TEST_NEQ(m, x);
        
        n = x;
        fff_TEST_EQ(n, x);
    }



fff_TEST_END_IMPL()


fff_TEST_BEGIN_IMPL(classes, DevSingleChannel)

    {
        OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
        fff_TEST_OK(env);

        DevSingleChannel<Float> sc(env);
        fff_TEST_NOK(sc);

        sc.alloc(CL_MEM_READ_ONLY, 100);
        fff_TEST_OK(sc);

        sc.dealloc();
        fff_TEST_NOK(sc);

        DevSingleChannel<Float> sc2(env);
        fff_TEST_NOK(sc2);

        fff_TEST_TRY_CATCH(
            sc != sc2;
            ,
            InvalidObjectException
        );

        sc.alloc(CL_MEM_READ_WRITE, 100);
        sc2.alloc(CL_MEM_READ_WRITE, 200);
        fff_TEST_OK(sc);
        fff_TEST_OK(sc2);

        fff_TEST_NEQ(sc, sc2);

        sc = sc2;
        fff_TEST_OK(sc);

        fff_TEST_EQ(sc, sc2);

        sc2.dealloc();
        fff_TEST_OK(sc);

        DevSingleChannel<Float> sc3(sc);
        fff_TEST_OK(sc3);
        fff_TEST_EQ(sc, sc3);

        sc.dealloc();
        fff_TEST_OK(sc3);

        DevSingleChannel<Float> sc4(env);
        sc4.copy(sc3);

        fff_TEST_TRUE(
            sc4.getMemFlags() == sc3.getMemFlags() &&
            sc4.getPerBufferSize() == sc3.getPerBufferSize() &&
            sc4.getSampleCount() == sc3.getSampleCount() &&
            sc4.isAllocated());

        fff_TEST_NEQ(sc4, sc3);
        
        sc4.enqueueCopy(sc3);
        sc4.enqueueCopy(sc3, 1, 199);

        fff_TEST_TRY_CATCH(
            sc4.enqueueCopy(sc3, 1, 200);
            ,
            InvalidRelationException
        );

        // Actually valid, but OpenCL
        // dislikes accessing invalid memory
        // although copying zero bytes!
        // Our DevSingleChannel class
        // checks this and does not invoke
        // OpenCL if samples to copy are zero!
        sc4.enqueueCopy(sc3, 200, 0);

        fff_TEST_TRY_CATCH(
            sc4.enqueueCopy(sc3, 1, 200);
            ,
            InvalidRelationException
        );

        sc4.enqueueCopy(sc3, 200, 200, 0);
        sc4.enqueueCopy(sc3, 199, 199, 1);
        sc4.enqueueCopy(sc3, 190, 150, 10);
        sc4.enqueueCopy(sc3, 150, 190, 10);
        sc4.enqueueCopy(sc3, 150, 199, 1);

        fff_TEST_TRY_CATCH(
            sc4.enqueueCopy(sc3, 199, 0, 2);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            sc4.enqueueCopy(sc3, 198, 199, 2);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            sc4.enqueueCopy(sc3, 200, 199, 1);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            sc4.enqueueCopy(sc3, 0, 100, 101);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            sc4.enqueueCopy(sc3, 100, 0, 101);
            ,
            InvalidRelationException
        );

    }

fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, HostSingleChannel)

    {
        // same as a ComplexVector!
        HostSingleChannel<Float> hsc;
        fff_TEST_NOK(hsc);

        hsc.alloc(100);
        fff_TEST_OK(hsc);
    }

fff_TEST_END_IMPL()


fff_TEST_BEGIN_IMPL(classes, HostMultiChannel)

    {
        // same as a ComplexVector!
        HostMultiChannel<Float> hmc;
        fff_TEST_NOK(hmc);
        hmc.alloc(5, 10);
        fff_TEST_OK(hmc);

        fff_TEST_EQ(hmc.getSampleCount(), 10);
        fff_TEST_EQ(hmc.getChannelCount(), 5);
        
        hmc[0].getReal()[0] = 1.f;
        hmc[0].getImag()[0] = .5f;
        
        fff_TEST_EQ(hmc[0].getReal()[0], 1.f);
        fff_TEST_EQ(hmc[0].getImag()[0], .5f);
        fff_TEST_EQ(hmc[1].getReal()[0], 0.f);
        fff_TEST_EQ(hmc[1].getImag()[0], 0.f);
        hmc[1] = hmc[0];

        fff_TEST_EQ(hmc[1].getReal()[0], 1.f);
        fff_TEST_EQ(hmc[1].getImag()[0], .5f);

        hmc.init();
        fff_TEST_EQ(hmc[0].getReal()[0], 0.f);
        fff_TEST_EQ(hmc[0].getImag()[0], 0.f);
        fff_TEST_EQ(hmc[1].getReal()[0], 0.f);
        fff_TEST_EQ(hmc[1].getImag()[0], 0.f);
        
    }

fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, HostMultiChannelPreallocated)
    {
        HostMultiChannelPreallocated<Float> hmcp;
        Float c1[] = {1.f, 2.f};
        Float c2[] = {3.f, 4.f};
        Float *ch[2] = { {c1}, {c2} };

        hmcp.alloc(2, 2);
        hmcp.setPointer(ch, 2);
        fff_TEST_TRUE(hmcp.isAllocated());

        fff_TEST_EQ(hmcp.getRawReal(0)[0], 1.f);
        fff_TEST_EQ(hmcp.getRawReal(0)[1], 2.f);
        fff_TEST_EQ(hmcp.getRawReal(1)[0], 3.f);
        fff_TEST_EQ(hmcp.getRawReal(1)[1], 4.f);

        fff_TEST_EQ(hmcp.getRawImag(0)[0], 0.f);
        fff_TEST_EQ(hmcp.getRawImag(0)[1], 0.f);
        fff_TEST_EQ(hmcp.getRawImag(1)[0], 0.f);
        fff_TEST_EQ(hmcp.getRawImag(1)[1], 0.f);

        fff_TEST_TRY_CATCH(
            hmcp.getRawReal(2);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            hmcp.getRawImag(2);
            ,
            InvalidRelationException
        );
    }


fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, UbiMultiChannelBase)

    {
        OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
        
        HostMultiChannel<Float> hmc(2, 2);
        UbiMultiChannelBase<Float> to(env, CL_MEM_READ_WRITE, &hmc, False);

        Float c1[] = {1.f, 2.f};
        Float c2[] = {3.f, 4.f};
        Float *ch[2] = { {c1}, {c2} };
        HostMultiChannelPreallocated<Float> hmcp;
        UbiMultiChannelBase<Float> from(env, CL_MEM_READ_WRITE, 2, False);
        hmcp.alloc(2, 2);
        hmcp.setPointer(ch, 2);
        from.setHostMultiChannel(&hmcp);
        
        

        fff_TEST_EQ(to.getMultiChannel().getRawReal(0)[0], 0.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(0)[1], 0.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(1)[0], 0.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(1)[1], 0.f);

        // copy first channel

        // copy to device
        from.enqueueDeviceUpdate(0);

        // copy buffer
        from.enqueueCopy(to);

        // fetch destination buffer
        to.enqueueHostUpdate(0);

        env.getQueue().finish();

        fff_TEST_EQ(to.getMultiChannel().getRawReal(0)[0], 1.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(0)[1], 2.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(1)[0], 0.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(1)[1], 0.f);

        // copy second channel

        // copy to device
        from.enqueueDeviceUpdate(1);

        // copy buffer
        from.enqueueCopy(to);

        // fetch destination buffer
        to.enqueueHostUpdate(1);

        env.getQueue().finish();

        fff_TEST_EQ(to.getMultiChannel().getRawReal(0)[0], 1.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(0)[1], 2.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(1)[0], 3.f);
        fff_TEST_EQ(to.getMultiChannel().getRawReal(1)[1], 4.f);
    }

fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, UbiMultiChannel)
    {
        OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
        
        UbiMultiChannel<Float> umc(env, CL_MEM_READ_WRITE, 2, 2, False); 
    }
fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, Convolution)
    
    OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
    fff_TEST_OK(env);

    Compiler c(env, Compiler::CL_LOCATION, Worker(), Unroll());
    c.build();
    fff_TEST_OK(c);

    fff_TEST_BEGIN_SECTION("test invalid buffer sample size and channel count")
        UbiMultiChannel<Float> x1(env, CL_MEM_READ_ONLY, 2, 10, False);
        UbiMultiChannel<Float> x2(env, CL_MEM_READ_ONLY, 3, 10, False);

        UbiMultiChannel<Float> h1(env, CL_MEM_READ_ONLY, 2, 5, False);
        UbiMultiChannel<Float> h2(env, CL_MEM_READ_ONLY, 3, 5, False);

        UbiMultiChannel<Float> y1(env, CL_MEM_WRITE_ONLY, 2, 14, False);
        UbiMultiChannel<Float> y2(env, CL_MEM_WRITE_ONLY, 2, 15, False);
        UbiMultiChannel<Float> y3(env, CL_MEM_WRITE_ONLY, 3, 14, False);

        fff_TEST_TRY_NOTHROW(
            Convolution<Float> ck(c, x1, h1, y1);
            fff_TEST_OK(ck);
            ck.invokeAndWait();
        );

        fff_TEST_TRY_CATCH(
            Convolution<Float> ck(c, x2, h1, y1);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            Convolution<Float> ck(c, x1, h2, y1);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            Convolution<Float> ck(c, x1, h1, y3);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            Convolution<Float> ck(c, x1, h1, y2);
            ,
            InvalidRelationException
        );

    fff_TEST_END_SECTION()


    fff_TEST_BEGIN_SECTION("test buffer accesstypes")

        UbiMultiChannel<Float> xv(env, CL_MEM_READ_ONLY, 2, 100, False);
        UbiMultiChannel<Float> xw(env, CL_MEM_WRITE_ONLY, 2, 100, False);
        UbiMultiChannel<Float> xrw(env, CL_MEM_READ_WRITE, 2, 100, False);

        UbiMultiChannel<Float> hv(env, CL_MEM_READ_ONLY, 2, 100, False);
        UbiMultiChannel<Float> hw(env, CL_MEM_WRITE_ONLY, 2, 100, False);
        UbiMultiChannel<Float> hrw(env, CL_MEM_READ_WRITE, 2, 100, False);

        UbiMultiChannel<Float> yr(env, CL_MEM_READ_ONLY, 2, 199, False);
        UbiMultiChannel<Float> yv(env, CL_MEM_WRITE_ONLY, 2, 199, False);
        UbiMultiChannel<Float> yrw(env, CL_MEM_READ_WRITE, 2, 199, False);

        fff_TEST_OK(xv);
        fff_TEST_OK(xw);
        fff_TEST_OK(xrw);

        fff_TEST_OK(hv);
        fff_TEST_OK(hw);
        fff_TEST_OK(hrw);

        fff_TEST_OK(yr);
        fff_TEST_OK(yv);
        fff_TEST_OK(yrw);

        fff_TEST_TRY_NOTHROW(
            Convolution<Float> ck(c, xv, hv, yv);
            fff_TEST_OK(ck);
            ck.invokeAndWait();
        );

        fff_TEST_TRY_NOTHROW(
            Convolution<Float> ck(c, xrw, hv, yv);
            fff_TEST_OK(ck);
            ck.invokeAndWait();
        );


        fff_TEST_TRY_CATCH(
            Convolution<Float> ck(c, xw, hv, yv);
            ,
            WrongBoolException
        );


        fff_TEST_TRY_NOTHROW(
            Convolution<Float> ck(c, xv, hrw, yv);
            fff_TEST_OK(ck);
            ck.invokeAndWait();
        );

        fff_TEST_TRY_CATCH(
            Convolution<Float> ck(c, xv, hw, yv);
            ,
            WrongBoolException
        )


        fff_TEST_TRY_NOTHROW(
            Convolution<Float> ck(c, xrw, hv, yrw);
            fff_TEST_OK(ck);
            ck.invokeAndWait();
        );

        fff_TEST_TRY_CATCH(
            Convolution<Float> ck(c, xw, hv, yr);
            ,
            WrongBoolException
        );
        
    fff_TEST_END_SECTION()

    fff_TEST_BEGIN_SECTION("test values")
        UbiMultiChannel<Float> x(env, CL_MEM_READ_ONLY, 2, 4, False);
        UbiMultiChannel<Float> h(env, CL_MEM_READ_ONLY, 2, 2, False);
        UbiMultiChannel<Float> y(env, CL_MEM_WRITE_ONLY, 2, 5, False);

        const Float x0[] = {1,2,3,4};
        const Float h0[] = {1,2};
        const Float y0[] = {
            1.f*1.f,
            2.f*1.f + 1.f*2.f,
            3.f*1.f + 2.f*2.f,
            4.f*1.f + 3.f*2.f,
                      4.f*2.f
        };

        const Float x1[] = {2,3,3,1};
        const Float h1[] = {2,3};
        const Float y1[] = {
            2.f*2.f,
            3.f*2.f + 2.f*3.f,
            3.f*2.f + 3.f*3.f,
            1.f*2.f + 3.f*3.f,
                      1.f*3.f
        };

        copySamples(x.getHostBuffer().getRawReal(0), x0, 4);
        copySamples(x.getHostBuffer().getRawReal(1), x1, 4);

        copySamples(h.getHostBuffer().getRawReal(0), h0, 2);
        copySamples(h.getHostBuffer().getRawReal(1), h1, 2);
        
        Convolution<Float> ck(c, x, h, y);
        ck.invokeAndWait();

        fff_TEST_TRUE(
            variance(y.getHostBuffer().getRawReal(0), y0, 5)
            ==
            0.f
        );

        fff_TEST_TRUE(
            variance(y.getHostBuffer().getRawReal(1), y1, 5)
            ==
            0.f
        );

    fff_TEST_END_SECTION()

fff_TEST_END_IMPL()


fff_TEST_BEGIN_IMPL(classes, Transformation)
    OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
    Compiler c(env);

    c.build();

    fff_TEST_OK(env);
    fff_TEST_OK(c);

    fff_TEST_BEGIN_SECTION("test invalid input")
        UbiMultiChannel<Float> vc(env, CL_MEM_READ_WRITE, 2, 2, False);
        UbiMultiChannel<Float> vs(env, CL_MEM_READ_WRITE, 2, 2, False);

        UbiMultiChannel<Float> ic1(env, CL_MEM_READ_WRITE, 2, 1, False);
        UbiMultiChannel<Float> ic2(env, CL_MEM_READ_WRITE, 1, 2, False);

        UbiMultiChannel<Float> is1(env, CL_MEM_READ_WRITE, 2, 1, False);
        UbiMultiChannel<Float> is2(env, CL_MEM_READ_WRITE, 1, 2, False);

        UbiMultiChannel<Float> icb(env, CL_MEM_WRITE_ONLY, 2, 2, False);
        UbiMultiChannel<Float> isb(env, CL_MEM_READ_ONLY, 2, 2, False);

        fff_TEST_TRY_NOTHROW(
            Transformation<Float> ts(c, vc, vs, True);
            Transformation<Float> tc(c, vc, vs, False);
        );

        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, ic1, vs, True);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, ic1, vs, False);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, ic2, vs, True);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, ic2, vs, False);
            ,
            InvalidRelationException
        );


        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, vc, is1, True);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, vc, is2, False);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, vc, is1, True);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, vc, is2, False);
            ,
            InvalidRelationException
        );


        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, vc, isb);
            ,
            WrongBoolException
        );
        fff_TEST_TRY_CATCH(
            Transformation<Float> t(c, icb, vs);
            ,
            WrongBoolException
        );


    fff_TEST_END_SECTION()
    
    fff_TEST_BEGIN_SECTION("test fft")

        UbiMultiChannel<Float> cont(env, CL_MEM_READ_WRITE, 2, 4, False);
        UbiMultiChannel<Float> spec(env, CL_MEM_READ_WRITE, 2, 4, False);

        const Float zero[] = {0.f, 0.f, 0.f, 0.f};

        const Float x0[] = {1.f, 0.f, 0.f, 0.f};
        const Float X0[] = {0.25f, 0.25f, 0.25f, 0.25f};

        const Float x1[] = {1.f, 1.f, 1.f, 1.f};
        const Float X1[] = {1.f, 0.f, 0.f, 0.f};

        copySamples(cont.getHostBuffer().getRawReal(0), x0, 4);
        copySamples(cont.getHostBuffer().getRawReal(1), x1, 4);

        Transformation<Float> tra(
            c,
            cont,
            spec,
            True);

        tra.invokeAndWait();

        

        fff_TEST_TRUE(variance(X0, spec.getHostBuffer().getRawReal(0), 4) == 0.f);
        fff_TEST_TRUE(variance(zero, spec.getHostBuffer().getRawImag(0), 4) == 0.f);

        fff_TEST_TRUE(variance(X1, spec.getHostBuffer().getRawReal(1), 4) == 0.f);
        fff_TEST_TRUE(variance(zero, spec.getHostBuffer().getRawImag(1), 4) == 0.f);

        cont.getHostBuffer().init();
        fff_TEST_TRUE(variance(zero, cont.getHostBuffer().getRawReal(0), 4) == 0.f);
        fff_TEST_TRUE(variance(zero, cont.getHostBuffer().getRawImag(0), 4) == 0.f);
        fff_TEST_TRUE(variance(zero, cont.getHostBuffer().getRawReal(1), 4) == 0.f);
        fff_TEST_TRUE(variance(zero, cont.getHostBuffer().getRawImag(1), 4) == 0.f);

        Transformation<Float> itra(
            c,
            cont,
            spec,
            False);

        itra.invokeAndWait();

        fff_TEST_TRUE(variance(x0, cont.getHostBuffer().getRawReal(0), 4) == 0.f);
        fff_TEST_TRUE(variance(zero, cont.getHostBuffer().getRawImag(0), 4) == 0.f);
        fff_TEST_TRUE(variance(x1, cont.getHostBuffer().getRawReal(1), 4) == 0.f);
        fff_TEST_TRUE(variance(zero, cont.getHostBuffer().getRawImag(1), 4) == 0.f);

    fff_TEST_END_SECTION()

fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, TransferFunction)
    OpenCLEnvironment env(CL_DEVICE_TYPE_DEFAULT);
    Compiler c(env);
    c.build();

    fff_TEST_OK(env);
    fff_TEST_OK(c);

    fff_TEST_BEGIN_SECTION("invalid transfer function input")
        
        UbiMultiChannel<Float> vb(env, CL_MEM_READ_ONLY, 2, 2, False);
        UbiMultiChannel<Float> va(env, CL_MEM_READ_ONLY, 2, 2, False);
        UbiMultiChannel<Float> vH(env, CL_MEM_READ_WRITE, 2, 2, False);

        UbiMultiChannel<Float> ib1(env, CL_MEM_READ_ONLY, 1, 2, False);
        UbiMultiChannel<Float> ib2(env, CL_MEM_READ_ONLY, 2, 1, False);
        UbiMultiChannel<Float> ia1(env, CL_MEM_READ_ONLY, 1, 2, False);
        UbiMultiChannel<Float> ia2(env, CL_MEM_READ_ONLY, 2, 1, False);
        UbiMultiChannel<Float> iH1(env, CL_MEM_READ_WRITE, 1, 2, False);
        UbiMultiChannel<Float> iH2(env, CL_MEM_READ_WRITE, 2, 1, False);

        fff_TEST_TRY_NOTHROW(
            TransferFunction<Float> t(
                c,
                vb,
                va,
                vH);

            fff_TEST_OK(t);
        );

        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, ib1, va, vH);
            fff_TEST_OK(t);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, ib2, va, vH);
            fff_TEST_OK(t);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, vb, ia1, vH);
            fff_TEST_OK(t);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, vb, ia2, vH);
            fff_TEST_OK(t);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, vb, va, iH1);
            fff_TEST_OK(t);
            ,
            InvalidRelationException
        );
        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, vb, va, iH2);
            fff_TEST_OK(t);
            ,
            InvalidRelationException
        );


        UbiMultiChannel<Float> ibb(env, CL_MEM_WRITE_ONLY, 2, 2, False);
        UbiMultiChannel<Float> iab(env, CL_MEM_WRITE_ONLY, 2, 2, False);
        UbiMultiChannel<Float> iHb1(env, CL_MEM_READ_ONLY, 2, 2, False);
        UbiMultiChannel<Float> iHb2(env, CL_MEM_WRITE_ONLY, 2, 2, False);

        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, ibb, va, vH);
            fff_TEST_OK(t);
            ,
            WrongBoolException
        );

        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, vb, iab, vH);
            fff_TEST_OK(t);
            ,
            WrongBoolException
        );

        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, vb, va, iHb1);
            fff_TEST_OK(t);
            ,
            WrongBoolException
        );

        fff_TEST_TRY_CATCH(
            TransferFunction<Float> t(
                c, vb, va, iHb2);
            fff_TEST_OK(t);
            ,
            WrongBoolException
        );


    fff_TEST_END_SECTION()

    fff_TEST_BEGIN_SECTION("transfer function values 1")
        UbiMultiChannel<Float> b(env, CL_MEM_READ_ONLY, 1, 4, False);
        UbiMultiChannel<Float> a(env, CL_MEM_READ_ONLY, 1, 4, False);
        UbiMultiChannel<Float> H(env, CL_MEM_READ_WRITE, 1, 4, False);

        TransferFunction<Float> tra(c, b, a, H);

        const Float b1[] = {1, 0, 0, 0};
        const Float a1[] = {1, 0, 0, 0};
        const Float H1[] = {1, 1, 1, 1};

        copySamples(b.getHostBuffer().getRawReal(0), b1, 4);
        copySamples(a.getHostBuffer().getRawReal(0), a1, 4);

        tra.invokeAndWait();

        fff_EXPECT(
            variance(H1, H.getHostBuffer().getRawReal(0), 4),
            ==,
            0.f);

    fff_TEST_END_SECTION()

    fff_TEST_BEGIN_SECTION("transfer function values 2")
        UbiMultiChannel<Float> b(env, CL_MEM_READ_ONLY, 1, 8, False);
        UbiMultiChannel<Float> a(env, CL_MEM_READ_ONLY, 1, 8, False);
        UbiMultiChannel<Float> H(env, CL_MEM_READ_WRITE, 1, 8, False);

        //                  1
        // H = ---------------------------
        //      (1 - pz[-1])*(1 + pz[-1])
        //
        //                  1
        // H = ------------------------------
        //      1 + pz[-1] - pz[-1] - p*p*z[-2]
        //
        //                 1
        // H = -----------------------------
        //      1 - p*p * z[-2]
        //
        // for p = 0.5 =>
        // a0=1, a1=0, a2=0.25
        
        Float b1[] = {1.f, 0.f, 0.f,   0.f, 0.f, 0.f, 0.f, 0.f};
        Float a1[] = {1.f, 0.f, 0.25f, 0.f, 0.f, 0.f, 0.f, 0.f};


        copySamples(b.getHostBuffer().getRawReal(0), b1, 8);
        copySamples(a.getHostBuffer().getRawReal(0), a1, 8);

        TransferFunction<Float> trans(c, b, a, H);
        fff_TEST_OK(trans);
        trans.invokeAndWait();

    fff_TEST_END_SECTION()
fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(classes, FastConvolution)
    OpenCLEnvironment env(CL_DEVICE_TYPE_GPU);
    Compiler c(env);
    c.build();
    
    fff_TEST_OK(env);
    fff_TEST_OK(c);

    fff_TEST_BEGIN_SECTION("invalid Fast Convolution input")
        UbiMultiChannel<Float> vx(env, CL_MEM_READ_ONLY, 2, 15, False);
        UbiMultiChannel<Float> ix1(env, CL_MEM_READ_ONLY, 1, 15, False);
        UbiMultiChannel<Float> ix2(env, CL_MEM_READ_ONLY, 2, 16, False);

        UbiMultiChannel<Float> vy(env, CL_MEM_READ_WRITE, 2, 15, False);
        UbiMultiChannel<Float> iy1(env, CL_MEM_READ_WRITE, 1, 15, False);
        UbiMultiChannel<Float> iy2(env, CL_MEM_READ_WRITE, 2, 14, False);

        UbiMultiChannel<Float> vs(env, CL_MEM_READ_WRITE, 2, 7, False);
        UbiMultiChannel<Float> is1(env, CL_MEM_READ_WRITE, 1, 7, False);
        UbiMultiChannel<Float> is2(env, CL_MEM_READ_WRITE, 2, 6, False);

        UbiMultiChannel<Float> vH(env, CL_MEM_READ_ONLY, 2, 32, False);
        UbiMultiChannel<Float> iH1(env, CL_MEM_READ_ONLY, 1, 32, False);
        UbiMultiChannel<Float> iH2(env, CL_MEM_READ_ONLY, 2, 31, False);

        fff_TEST_TRY_NOTHROW(
            FastConvolution<Float> fconv(c, vs, vx, vH, vy, 1);
            fff_TEST_OK(fconv);
        );
        

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, is1, vx, vH, vy, 1);
            ,
            WrongBoolException
        );

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, is2, vx, vH, vy, 1);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, vs, ix1, vH, vy, 1);
            ,
            WrongBoolException
        );

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, vs, ix2, vH, vy, 1);
            ,
            WrongBoolException
        );

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, vs, vx, iH1, vy, 1);
            ,
            WrongBoolException
        );

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, vs, vx, iH2, vy, 1);
            ,
            InvalidRelationException
        );

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, vs, vx, vH, iy1, 1);
            ,
            WrongBoolException
        );

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, vs, vx, vH, iy2, 1);
            ,
            WrongBoolException
        );

        // only H has to be readable!
        // other buffers are copied before.
        UbiMultiChannel<Float> iHb(env, CL_MEM_WRITE_ONLY, 2, 32, False);

        fff_TEST_TRY_CATCH(
            FastConvolution<Float> fconv(c, vs, vx, iHb, vy, 1);
            ,
            WrongBoolException
        );
    fff_TEST_END_SECTION()

    fff_TEST_BEGIN_SECTION("fast convolution values")
        UbiMultiChannel<Float> x(env, CL_MEM_READ_ONLY, 1, 10, False);
        UbiMultiChannel<Float> H(env, CL_MEM_READ_ONLY, 1, 8, False);
        UbiMultiChannel<Float> s(env, CL_MEM_READ_ONLY, 1, 3, False);
        UbiMultiChannel<Float> y(env, CL_MEM_READ_WRITE, 1, 10, False);

        FastConvolution<Float> fconv(c, s, x, H, y, 0);
        fff_TEST_OK(fconv);

        const Float zero[] = {0,0,0,0,0,0,0,0,0,0};

        const Float s1[] = {1,2,3}; // state
        const Float x1[] = {4,5,6,7,8,9,10,11,12,13}; // input
        const Float H1[] = {1,1,1,1,1,1,1,1}; // neutral kernel
        
        const Float t1[] = {11,12,13}; // new state
        const Float y1[] = {4,5,6,7,8,9,10,11,12,13}; // output

        // => [s1,x1] ** h1 = iFFT(FFT([s1,x1]) * H1)
 
        copySamples(s.getHostBuffer().getRawReal(0), s1, 3);
        copySamples(x.getHostBuffer().getRawReal(0), x1, 10);
        copySamples(H.getHostBuffer().getRawReal(0), H1, 8);

        fconv.invokeAndWait();

        fff_TEST_EQ(
            variance(t1, s.getHostBuffer().getRawReal(0), 3),
            0.f);

        //printarray(std::cout, s.getHostBuffer().getRawReal(0), 3);

        // 32bit float aliasing!
        fff_TEST_LT(
            variance(y1, y.getHostBuffer().getRawReal(0), 10),
            1.0e-12);

    fff_TEST_END_SECTION()

    fff_TEST_BEGIN_SECTION("Transfer Function + fast convolution integration test")

        UbiMultiChannel<Float> b(env, CL_MEM_READ_ONLY, 1, 8, False);
        UbiMultiChannel<Float> a(env, CL_MEM_READ_ONLY, 1, 8, False);
        UbiMultiChannel<Float> H(env, CL_MEM_READ_WRITE, 1, 8, False);

        // M = 3

        const Float zeros[] = {1, 0, 0, 0, 0, 0, 0, 0 };
        const Float poles[] = {1, 0,   0,     0, 0, 0, 0, 0 };

        copySamples(b.getHostBuffer().getRawReal(0), zeros, 8);
        copySamples(a.getHostBuffer().getRawReal(0), poles, 8);

        TransferFunction<Float> tra(c, b, a, H);
        fff_TEST_OK(tra);
        tra.invokeAndWait();

        UbiMultiChannel<Float> x(env, CL_MEM_READ_ONLY, 1, 10, False);
        UbiMultiChannel<Float> s(env, CL_MEM_READ_ONLY, 1, 2, False);
        UbiMultiChannel<Float> y(env, CL_MEM_READ_WRITE, 1, 10, False);

        FastConvolution<Float> fconv(c, s, x, H, y, 1);
        fff_TEST_OK(fconv);

        const Float zero[] = {0,0,0,0,0,0,0,0,0,0};

        const Float s1[] = {0, 4}; // state
        const Float x1[] = {4,0,0,16,8,16,0,0,16,8}; // input
        
        const Float t1[] = {16, 8}; // new state
 
        copySamples(s.getHostBuffer().getRawReal(0), s1, 2);
        copySamples(x.getHostBuffer().getRawReal(0), x1, 10);

        fconv.invokeAndWait();

        // input is output, since h is the dirac impulse
        fff_TEST_LT(
            variance(y.getHostBuffer().getRawReal(0), x1, 10),
            1.0e-12f);


    fff_TEST_END_SECTION()
    
    fff_TEST_BEGIN_SECTION("huge fast convolution")
        
        const UInt f = 1;
        const UInt M = 100000;
        const UInt N = 1000000;

        
        ComputingData<Float> cd(M);
        DeviceProperties devprops(env.getDevice());
        Mapper map(cd, devprops, f);

        UbiMultiChannel<Float> b(env, CL_MEM_READ_ONLY, 1, fff_POW2(map.getLb2N()));
        UbiMultiChannel<Float> a(env, CL_MEM_READ_ONLY, 1, fff_POW2(map.getLb2N()));
        UbiMultiChannel<Float> H(env, CL_MEM_READ_WRITE, 1, fff_POW2(map.getLb2N()));

        a.getHostBuffer().getRawReal(0)[0] = 1.f;
        b.getHostBuffer().getRawReal(0)[0] = 1.f;

        TransferFunction<Float> tra(c, b, a, H);

        tra.invokeAndWait();

        UbiMultiChannel<Float> s(env, CL_MEM_READ_ONLY, 1, M-1);
        UbiMultiChannel<Float> x(env, CL_MEM_READ_ONLY, 1, N);
        UbiMultiChannel<Float> y(env, CL_MEM_READ_WRITE, 1, N);

        Float *xv = new Float[N];
        for(UInt i = 0; i < N; ++i)
            xv[i] = 1.f/(Float)(i+1);

        copySamples(x.getHostBuffer().getRawReal(0), xv, N);

        FastConvolution<Float> fconv(
            c,
            s,
            x,
            H,
            y,
            f);
        fconv.invokeAndWait();
        /*
        for(UInt i = 0; i < N; ++i)
            fff_EXPECT(
                fabs(y.getHostBuffer().getRawReal(0)[i]-xv[i]),
                <,
                0.02f);
        */

        fff_TEST_LT(
            variance(y.getHostBuffer().getRawReal(0), xv, N),
            1.0e-12f);

        delete [] xv;

    fff_TEST_END_SECTION()
    
    /*
    fff_TEST_BEGIN_SECTION("testing huge ir")
        
        const UInt f = 1;

        WaveReader<Float> fh("h.wav");
        WaveReader<Float> fx("x.wav");
        WaveWriter<Float> fy("y.wav", fx.getSampleRate(), fx.getChannelCount());
        
        ComputingData<Float> cd(fh.getSampleCount());
        DeviceProperties devprops(env.getDevice());
        Mapper map(cd, devprops, f);

        UbiMultiChannel<Float> b(env, CL_MEM_READ_ONLY, fh.getChannelCount(), fff_POW2(map.getLb2N()));
        UbiMultiChannel<Float> a(env, CL_MEM_READ_ONLY, fh.getChannelCount(), fff_POW2(map.getLb2N()));
        UbiMultiChannel<Float> H(env, CL_MEM_READ_WRITE, fh.getChannelCount(), fff_POW2(map.getLb2N()));

        for(UInt ch = 0; ch < a.getHostBuffer().getChannelCount(); ++ch)
            a.getHostBuffer().getRawReal(ch)[0] = 1.f;

        fh.readFile(b.getHostBuffer());
        Float *bsl = b.getHostBuffer().getRawReal(0);
        Float *bsr = b.getHostBuffer().getRawReal(1);

        TransferFunction<Float> tra(c, b, a, H);

        tra.invokeAndWait();

        UbiMultiChannel<Float> s(env, CL_MEM_READ_ONLY, fh.getChannelCount(), fh.getSampleCount()-1);
        UbiMultiChannel<Float> x(env, CL_MEM_READ_ONLY, fh.getChannelCount(), fx.getSampleCount() + fh.getSampleCount());
        UbiMultiChannel<Float> y(env, CL_MEM_READ_WRITE, fh.getChannelCount(), fx.getSampleCount() + fh.getSampleCount());

        fx.readFile(x.getHostBuffer());

        x.getMultiBuffer() *= 0.5;

        FastConvolution<Float> fconv(
            c,
            s,
            x,
            H,
            y,
            f);
        fconv.invokeAndWait();

        y.getMultiBuffer() *= 0.05f;
        y.getMultiBuffer() += x.getMultiBuffer();

        fstream file("out.txt", ios::out);
        file<<"channels: " << y.getHostBuffer().getChannelCount() << std::endl;
        for(UInt ch = 0; ch < y.getHostBuffer().getChannelCount(); ++ch)
        {
            for(UInt s = 0; s < y.getHostBuffer().getSampleCount(); ++s)
            {
                Float smp = y.getHostBuffer().getRawReal(ch)[s];
                if(smp > 1.f)
                {
                    file << "Sample[" << ch << "][" << s << "] = " << smp << std::endl;
                }
            }
        }
        file.close();

        

        fy.writeFile(y.getHostBuffer());

    fff_TEST_END_SECTION()
    */
    
    fff_TEST_BEGIN_SECTION("preallocated test")

        
        UbiMultiChannel<Float> b(env, CL_MEM_READ_ONLY, 1, 8);
        UbiMultiChannel<Float> a(env, CL_MEM_READ_ONLY, 1, 8);
        UbiMultiChannel<Float> H(env, CL_MEM_READ_WRITE, 1, 8);

        // M = 3

        const Float zeros[] = {1, 0, 0, 0, 0, 0, 0, 0 };
        const Float poles[] = {1, 0, 0, 0, 0, 0, 0, 0 };

        copySamples(b.getHostBuffer().getRawReal(0), zeros, 8);
        copySamples(a.getHostBuffer().getRawReal(0), poles, 8);

        TransferFunction<Float> tra(c, b, a, H);
        fff_TEST_OK(tra);
        tra.invokeAndWait();
        
        UbiMultiChannelPreallocated<Float> x(env, CL_MEM_READ_ONLY);
        UbiMultiChannel<Float> s(env, CL_MEM_READ_ONLY, 1, 2, False);
        UbiMultiChannelPreallocated<Float> y(env, CL_MEM_READ_WRITE);

        // input samples
        Float x1[] = {1,2,3,4,5,6,7,8,9,10};
        Float x2[] = {11, 12, 13, 14, 15};

        Float t1[] = {9, 10};
        Float t2[] = {14, 15};

        // output samples
        Float y1[10];
        Float y2[5];
        
        Float **xp1 = new Float*[1];
        Float **xp2 = new Float*[1];
        Float **yp1 = new Float*[1];
        Float **yp2 = new Float*[1];

        xp1[0] = x1;
        xp2[0] = x2;
        yp1[0] = y1;
        yp2[0] = y2;

        // set pseudo max size to
        // allow initialization of the input buffer
        // INPUT = [STATE; X]
        x.allocMax(1, 100);
        y.allocMax(1, 100);

        FastConvolution<Float> fconv(c, s, x, H, y, 1);
        fff_TEST_OK(fconv);

        // VST starts computation

        // VST gives us 10 samples in xp1
        // and wants the fconv in yp1
        
        // first process call
        x.setTarget(xp1, 10);
        y.setTarget(yp1, 10);
        fconv.invoke();

        // second process call
        x.setTarget(xp2, 5);
        y.setTarget(yp2, 5);
        fconv.invoke();

        fconv.wait();

        fff_TEST_LT(
            variance(yp1[0], x1, 10),
            1.0e-12f);

        fff_TEST_LT(
            variance(yp2[0], x2, 5),
            1.0e-12f);

        fff_TEST_EQ(
            variance(s.getHostBuffer().getRawReal(0), t2, 2),
            0.f);

        // ...

        delete [] xp1;
        delete [] xp2;
        delete [] yp1;
        delete [] yp2;

    fff_TEST_END_SECTION()

    fff_TEST_BEGIN_SECTION("N>M N=M N<M tests")
        

        // notation:
        // size of x >= size of s
        //      (current state) ... [next state] ** kernel
        // size of x < size of s
        //       (current[state)new] ** kernel
        //=[old samples][output];

        // M = 3 => b[M+1:..]=0


        const UInt
            in0 = 1,
            in1 = 3,
            in2 = 8,
            in3 = 6;

        const UInt
            state = 2,
            M = 3,
            HSize = cl::multiple2In(M);

        // -------------------------------
        // -------------------------------
        const Float bs0[] = {1, 1, 0};
        // -------------------------------
        Float xs00[] = {0};
        //(0[0)0]** 1 1 0
        //=. . 0 . .
        const Float ss00[] = {0, 0};
        const Float ys00[] = {0};
        // -------------------------------
        Float xs10[] = {1, 1, 0};
        //(0 0)1[1 0]** 1 1 0
        //=. . 1 2 1 . .
        const Float ss10[] = {1, 0};
        const Float ys10[] = {1, 2, 1};
        // -------------------------------
        Float xs20[] = {1, 0, 0, 0, 1, 1, 0, 1};
        //(1 0)1 0 0 0 1 1[0 1]** 1 1 0
        //=. . 1 1 0 0 1 2 1 1 . .
        const Float ss20[] = {0, 1};
        const Float ys20[] = {1, 1, 0, 0, 1, 2, 1, 1};
        // -------------------------------
        Float xs30[] = {1, 1, 0, 1, 0, 0};
        //(0 1)1 1 0 1[0 0]** 1 1 0
        //=. . 2 2 1 1 1 0 . .
        const Float ss30[] = {0, 0};
        const Float ys30[] = {2, 2, 1, 1, 1, 0};
        // -------------------------------
        // -------------------------------

        const Float bs1[] = {1, 0, 1};
        // -------------------------------
        Float xs01[] = {1};
        //(0[0)1]** 1 0 1
        //=. . 1 . .
        const Float ss01[] = {0, 1};
        const Float ys01[] = {1};
        // -------------------------------
        Float xs11[] = {1, 0, 0};
        //(0 1)1[0 0] ** 1 0 1
        //=. . 1 1 1 . .
        const Float ss11[] = {0, 0};
        const Float ys11[] = {1, 1, 1};
        // -------------------------------
        Float xs21[] = {0, 0, 0, 1, 1, 0, 1, 1};
        //(0 0)0 0 0 1 1 0[1 1]** 1 0 1
        //=. . 0 0 0 1 1 1 2 1 . .
        const Float ss21[] = {1, 1};
        const Float ys21[] = {0, 0, 0, 1, 1, 1, 2, 1};
        // -------------------------------
        Float xs31[] = {1, 1, 1, 1, 1, 0};
        //(1 1)1 1 1 1[1 0]** 1 0 1
        //=. . 2 2 2 2 2 1 . .
        const Float ss31[] = {1, 0};
        const Float ys31[] = {2, 2, 2, 2, 2, 1};
        // -------------------------------
        // -------------------------------

        Float **x0 = new Float*[2];
        x0[0] = xs00;
        x0[1] = xs01;

        Float **x1 = new Float*[2];
        x1[0] = xs10;
        x1[1] = xs11;

        Float **x2 = new Float*[2];
        x2[0] = xs20;
        x2[1] = xs21;

        Float **x3 = new Float*[2];
        x3[0] = xs30;
        x3[1] = xs31;

        Float **y0 = new Float*[2];
        y0[0] = new Float[in0];
        y0[1] = new Float[in0];

        Float **y1 = new Float*[2];
        y1[0] = new Float[in1];
        y1[1] = new Float[in1];

        Float **y2 = new Float*[2];
        y2[0] = new Float[in2];
        y2[1] = new Float[in2];

        Float **y3 = new Float*[2];
        y3[0] = new Float[in3];
        y3[1] = new Float[in3];

        const UInt
            pseudoCount = 3,
            pseudoX[pseudoCount] = {11, 1010, 63777},
            pseudoM[pseudoCount] = { 3,  477,  4109},
            pseudoThroughputs[pseudoCount] = { 0, 7, 11 };

        for(UInt i = 0; i < pseudoCount; ++i)
        {
            const UInt
                pX = pseudoX[i],
                pM = pseudoM[i],
                pT = pseudoThroughputs[i];

            const UInt pH = cl::multiple2In(pM);

            // std::cout << std::endl << "  pX=" << pX << ", pM=" << pM << ", pH=" << pH << ", pT=" << pT << std::endl;

            UbiMultiChannel<Float> b(env, CL_MEM_READ_ONLY, 2, pH);
            UbiMultiChannel<Float> a(env, CL_MEM_READ_ONLY, 2, pH);
            UbiMultiChannel<Float> H(env, CL_MEM_READ_WRITE, 2, pH);

            // poles
            a.getHostBuffer().getRawReal(0)[0] = 1.f;
            a.getHostBuffer().getRawReal(1)[0] = 1.f;

            // copy zeros
            copySamples(b.getHostBuffer().getRawReal(0), bs0, M);
            copySamples(b.getHostBuffer().getRawReal(1), bs1, M);

            TransferFunction<Float> tra(c, b, a, H);
            tra.invokeAndWait();

            UbiMultiChannelPreallocated<Float> x(env, CL_MEM_READ_ONLY);
            UbiMultiChannelPreallocated<Float> y(env, CL_MEM_READ_WRITE);
            UbiMultiChannel<Float> s(env, CL_MEM_READ_ONLY, 2, M-1);

            x.allocMax(2, pX);
            y.allocMax(2, pX);

            Vector<Float> zeros(pX);

            FastConvolution<Float> fconv(c, s, x, H, y, pT);

            // round 0x ---------------------------------------------+
            x.setTarget(x0, in0);
            y.setTarget(y0, in0);
            fconv.invokeAndWait();

            // channel 0 state real
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawReal(0), ss00, M-1),
                1.0e-12f);

            // channel 0 state imag (expect zeros)
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawImag(0), zeros.getSamples(), M-1),
                1.0e-12f);

            // channel 1 state real
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawReal(1), ss01, M-1),
                1.0e-12f);

            // channel 1 state imag (expect zeros)
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawImag(1), zeros.getSamples(), M-1),
                1.0e-12f);

            // channel 0 y real
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawReal(0), ys00, in0),
                1.0e-12f);

            // channel 0 y imag (expect zeros)
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawImag(0), zeros.getSamples(), in0),
                1.0e-12f);

            // channel 1 y real
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawReal(1), ys01, in0),
                1.0e-12f);

            // channel 1 y imag (expect zeros)
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawImag(1), zeros.getSamples(), in0),
                1.0e-12f);

            // round 1x ---------------------------------------------+
            x.setTarget(x1, in1);
            y.setTarget(y1, in1);
            fconv.invokeAndWait();

            // channel 0 state real
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawReal(0), ss10, M-1),
                1.0e-12f);

            // channel 0 state imag (expect zeros)
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawImag(0), zeros.getSamples(), M-1),
                1.0e-12f);

            // channel 1 state real
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawReal(1), ss11, M-1),
                1.0e-12f);

            // channel 1 state imag (expect zeros)
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawImag(1), zeros.getSamples(), M-1),
                1.0e-12f);

            // channel 0 y real
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawReal(0), ys10, in1),
                1.0e-12f);

            // channel 0 y imag (expect zeros)
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawImag(0), zeros.getSamples(), in1),
                1.0e-12f);

            // channel 1 y real
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawReal(1), ys11, in1),
                1.0e-12f);

            // channel 1 y imag (expect zeros)
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawImag(1), zeros.getSamples(), in1),
                1.0e-12f);


            // round 2x ---------------------------------------------+
            x.setTarget(x2, in2);
            y.setTarget(y2, in2);
            fconv.invokeAndWait();

            // channel 0 state real
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawReal(0), ss20, M-1),
                1.0e-12f);

            // channel 0 state imag (expect zeros)
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawImag(0), zeros.getSamples(), M-1),
                1.0e-12f);

            // channel 1 state real
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawReal(1), ss21, M-1),
                1.0e-12f);

            // channel 1 state imag (expect zeros)
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawImag(1), zeros.getSamples(), M-1),
                1.0e-12f);

            // channel 0 y real
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawReal(0), ys20, in2),
                1.0e-12f);

            // channel 0 y imag (expect zeros)
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawImag(0), zeros.getSamples(), in2),
                1.0e-12f);

            // channel 1 y real
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawReal(1), ys21, in2),
                1.0e-12f);

            // channel 1 y imag (expect zeros)
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawImag(1), zeros.getSamples(), in2),
                1.0e-12f);

            // round 3x ---------------------------------------------+
            x.setTarget(x3, in3);
            y.setTarget(y3, in3);
            fconv.invokeAndWait();

            // channel 0 state real
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawReal(0), ss30, M-1),
                1.0e-12f);

            // channel 0 state imag (expect zeros)
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawImag(0), zeros.getSamples(), M-1),
                1.0e-12f);

            // channel 1 state real
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawReal(1), ss31, M-1),
                1.0e-12f);

            // channel 1 state imag (expect zeros)
            fff_TEST_LTE(
                variance(s.getHostBuffer().getRawImag(1), zeros.getSamples(), M-1),
                1.0e-12f);

            // channel 0 y real
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawReal(0), ys30, in3),
                1.0e-12f);

            // channel 0 y imag (expect zeros)
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawImag(0), zeros.getSamples(), in3),
                1.0e-12f);

            // channel 1 y real
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawReal(1), ys31, in3),
                1.0e-12f);

            // channel 1 y imag (expect zeros)
            fff_TEST_LTE(
                variance(y.getHostBuffer().getRawImag(1), zeros.getSamples(), in3),
                1.0e-12f);
        
        }

        delete [] x0;
        delete [] x1;
        delete [] x2;
        delete [] x3;

        delete [] y0[0];
        delete [] y0[1];
        delete [] y0;
        delete [] y1[0];
        delete [] y1[1];
        delete [] y1;
        delete [] y2[0];
        delete [] y2[1];
        delete [] y2;
        delete [] y3[0];
        delete [] y3[1];
        delete [] y3;


    fff_TEST_END_SECTION()
    /*
    fff_TEST_BEGIN_SECTION("complex scenary")
        
        // read wav file with 60000 samples
        const UInt M = 60000;
        UbiMultiChannel<Float> b(env, CL_MEM_READ_ONLY, 1, 131072);
        UbiMultiChannel<Float> a(env, CL_MEM_READ_ONLY, 1, 131072);
        UbiMultiChannel<Float> H(env, CL_MEM_READ_WRITE, 1, 131072);

        // poles
        a.getHostBuffer().getRawReal(0)[0] = 1.f;

        // First and last sample of M (our read wav file)
        b.getHostBuffer().getRawReal(0)[0] = 1.f;
        b.getHostBuffer().getRawReal(0)[99999] = 1.f;

        // a = [1 0 0 ... 0]
        // b = [1 0 0 ... 1]

        TransferFunction<Float> tra(c, b, a, H);
        tra.invokeAndWait();

        // state 99999
        UbiMultiChannel<Float> s(env, CL_MEM_READ_ONLY, 1, M-1);
        UbiMultiChannelPreallocated<Float> x(env, CL_MEM_READ_ONLY);
        UbiMultiChannelPreallocated<Float> y(env, CL_MEM_READ_WRITE);

        // expected not more than 477 samples
        x.allocMax(1, 477);
        y.allocMax(1, 477);
        
        FastConvolution<Float> fconv(c, s, x, H, y, 1);

        
        x.getHostBuffer().init();
        for(UInt i = 0; i < x.getHostBuffer().getSampleCount(); ++i)
            x.getHostBuffer().getRawReal(0)[i] = (Float)i;

        fconv.invokeAndWait();

        fff_EXPECT(
            variance(x.getHostBuffer().getRawReal(0),

        

    fff_TEST_END_SECTION()
    */
fff_TEST_END_IMPL()

} } }

#endif