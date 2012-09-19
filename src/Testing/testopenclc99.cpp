//---------------------------------------------------------+
// fff/src/Testing/testopenclc99.cpp
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
//!	\file		testopenclc99.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		Testing OpenCL C99 Code.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

#ifdef _fff_TEST

//---------------------------------------------------------+
//! PROJECT INCLUDES
//!=========================================================
#include "../../include/_intern/_base.h"
#include "../../include/Testing/Base/_testingbase.h"
#include "../../include/Testing/testopenclc99.h"
#include "../../cl/_.h.cl"
//---------------------------------------------------------+

//---------------------------------------------------------+
//! EXTERNAL INCLUDES
//!=========================================================
//---------------------------------------------------------+

//---------------------------------------------------------+
//! NAMESPACES
//!=========================================================
using namespace fff;
using namespace fff::_Intern;
using namespace fff::Testing::OpenCLC99;
using namespace fff::cl;

//---------------------------------------------------------+

namespace fff {
namespace Testing {

//---------------------------------------------------------+
fff_TEST_BEGIN_IMPL(fff, openclc99)

    fff_TEST_CALL(openclc99, POW2);
    fff_TEST_CALL(openclc99, BITMASK);
    fff_TEST_CALL(openclc99, BITSWAPONLY);
    fff_TEST_CALL(openclc99, bit_reversal);
    fff_TEST_CALL(openclc99, factorial);
    fff_TEST_CALL(openclc99, multiple2);
    fff_TEST_CALL(openclc99, calcOverlapSaveSampleOffset);
    fff_TEST_CALL(openclc99, calcOverlapSaveSampleCount);
    fff_TEST_CALL(openclc99, fftButterfly);

    fff_TEST_CALL(openclc99, dconv);

fff_TEST_END_IMPL()
//---------------------------------------------------------+

namespace OpenCLC99 {

fff_TEST_BEGIN_IMPL(openclc99, POW2)

    fff_TEST_EQ(fff_POW2(0), BIN(I));
    fff_TEST_EQ(fff_POW2(1), BIN(IO));
    fff_TEST_EQ(fff_POW2(2), BIN(IOO));
    fff_TEST_EQ(fff_POW2(3), BIN(IOOO));
    fff_TEST_EQ(fff_POW2(4), BIN(IOOOO));
    fff_TEST_EQ(fff_POW2(16),
        BIN(IOOOOOOOOOOOOOOOO));
    fff_TEST_EQ(fff_POW2(24),
        BIN(IOOOOOOOOOOOOOOOOOOOOOOOO));
    fff_TEST_EQ(fff_POW2(31),
        BIN(IOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO));

fff_TEST_END_IMPL()

fff_TEST_BEGIN_IMPL(openclc99, BITMASK)

    fff_TEST_EQ(fff_BITMASK(0), BIN(I));
    fff_TEST_EQ(fff_BITMASK(1), BIN(IO));
    fff_TEST_EQ(fff_BITMASK(2), BIN(IOO));
    fff_TEST_EQ(fff_BITMASK(3), BIN(IOOO));
    fff_TEST_EQ(fff_BITMASK(4), BIN(IOOOO));
    fff_TEST_EQ(fff_BITMASK(16),
        BIN(IOOOOOOOOOOOOOOOO));
    fff_TEST_EQ(fff_BITMASK(24),
        BIN(IOOOOOOOOOOOOOOOOOOOOOOOO));
    fff_TEST_EQ(fff_BITMASK(31),
        BIN(IOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO));

fff_TEST_END_IMPL()


fff_TEST_BEGIN_IMPL(openclc99, BITSWAPONLY)

fff_TEST_END_IMPL()


//---------------------------------------------------------+
fff_TEST_BEGIN_IMPL(openclc99, factorial)
		
	fff_TEST_EQ(factorial(0), 1);
	fff_TEST_EQ(factorial(1), 1);
	fff_TEST_EQ(factorial(2), 2);
	fff_TEST_EQ(factorial(3), 6);
	fff_TEST_EQ(factorial(4), 24);
	fff_TEST_EQ(factorial(5), 120);
	fff_TEST_EQ(factorial(6), 720);
	fff_TEST_EQ(factorial(7), 5040);
	fff_TEST_EQ(factorial(8), 40320);
	fff_TEST_EQ(factorial(9), 362880);
	fff_TEST_EQ(factorial(10), 3628800);
	fff_TEST_EQ(factorial(11), 39916800);
	fff_TEST_EQ(factorial(12), 479001600);
		
	// end of 32 bit
	// 0xFFFFFFFF = 4294967295
fff_TEST_END_IMPL()

//---------------------------------------------------------+

//---------------------------------------------------------+
fff_TEST_BEGIN_IMPL(openclc99, bit_reversal)

	// test some numbers
	fff_TEST_EQ(bit_reversal(BIN(IOO), 0), BIN(O));
	fff_TEST_EQ(bit_reversal(BIN(I), 0), BIN(O));

	fff_TEST_EQ(bit_reversal(BIN(IIO), 1), BIN(O));
	fff_TEST_EQ(bit_reversal(BIN(IOIIIOIIOI), 1), BIN(I));

	fff_TEST_EQ(bit_reversal(BIN(OO), 2), BIN(OO));
	fff_TEST_EQ(bit_reversal(BIN(OI), 2), BIN(IO));
	fff_TEST_EQ(bit_reversal(BIN(IO), 2), BIN(OI));
	fff_TEST_EQ(bit_reversal(BIN(II), 2), BIN(II));
	fff_TEST_EQ(bit_reversal(BIN(IIO), 2), BIN(OI));
	fff_TEST_EQ(bit_reversal(BIN(III), 2), BIN(II));

	fff_TEST_EQ(bit_reversal(BIN(OOO), 3), BIN(OOO));
	fff_TEST_EQ(bit_reversal(BIN(OOI), 3), BIN(IOO));
	fff_TEST_EQ(bit_reversal(BIN(OIO), 3), BIN(OIO));
	fff_TEST_EQ(bit_reversal(BIN(OII), 3), BIN(IIO));
	fff_TEST_EQ(bit_reversal(BIN(IOO), 3), BIN(OOI));
	fff_TEST_EQ(bit_reversal(BIN(IOI), 3), BIN(IOI));
	fff_TEST_EQ(bit_reversal(BIN(IIO), 3), BIN(OII));
	fff_TEST_EQ(bit_reversal(BIN(III), 3), BIN(III));

	fff_TEST_EQ(bit_reversal(
        BIN(OOOOI), 5),
        BIN(IOOOO));

	fff_TEST_EQ(bit_reversal(
        BIN(OOOIIOOII),9),
        BIN(IIOOIIOOO));

	// test large numbers
	fff_TEST_EQ(bit_reversal(
        BIN(IOOIIOIOOOIIOIOOIOIIIIOOIOOIOIIO), 32),
        BIN(OIIOIOOIOOIIIIOIOOIOIIOOOIOIIOOI));

	fff_TEST_EQ(bit_reversal(
        BIN(OIOOOIOIOIOIIOIIOIOIOIIII), 8),
        BIN(OOOOOOOOOOOOOOOOOIIIIOIOI));

	fff_TEST_EQ(bit_reversal(
        BIN(OOIOOOIIIIIOI), 13),
        BIN(IOIIIIIOOOIOO));


fff_TEST_END_IMPL()
//---------------------------------------------------------+


//---------------------------------------------------------+
fff_TEST_BEGIN_IMPL(openclc99, multiple2)

	

    // test some lb2Multiple2
    fff_TEST_EQ(1<<lb2Multiple2(4), 8);
    fff_TEST_EQ(1<<lb2Multiple2(54), 64);
    fff_TEST_EQ(1<<lb2Multiple2(999), 1024);
    fff_TEST_EQ(1<<lb2Multiple2(1111), 2048);
    fff_TEST_EQ(1<<lb2Multiple2(45666), 65536);


    // special case "too much"
	fff_TEST_EQ(lb2Multiple2(1<<31), (UInt)-1);

	// test some numbers incl. zero
	fff_TEST_EQ(multiple2(0), 1);
	fff_TEST_EQ(multiple2(10), 16);
	fff_TEST_EQ(multiple2(50), 64);

	// test numbers near step numbers
	fff_TEST_EQ(multiple2(3), 4);
	fff_TEST_EQ(multiple2(127), 128);
	fff_TEST_EQ(multiple2(257), 512);
	fff_TEST_EQ(multiple2(65535), 65536);
    fff_TEST_EQ(multiple2(0x7FFFFFFF), 0x80000000);

	// test step numbers to get
	// the next step number
	fff_TEST_EQ(multiple2(1), 2);
	fff_TEST_EQ(multiple2(2), 4);
	fff_TEST_EQ(multiple2(4), 8);
	fff_TEST_EQ(multiple2(65536), 65536<<1);
    

    // test is multiple
    fff_TEST_TRUE(isMultiple2(BIN(I)));
    fff_TEST_TRUE(isMultiple2(BIN(IO)));
    fff_TEST_TRUE(isMultiple2(BIN(IOOO)));
    fff_TEST_TRUE(isMultiple2(BIN(IOOOOOOOOOOOO)));
    fff_TEST_TRUE(isMultiple2(BIN(IOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO)));


    fff_TEST_FALSE(isMultiple2(BIN(IOOI)));
    fff_TEST_FALSE(isMultiple2(BIN(IOOOOOIOOOOOO)));
    fff_TEST_FALSE(isMultiple2(BIN(IIOOOOOOOOOOO)));
    fff_TEST_FALSE(isMultiple2(BIN(IOOOOOOOOOOIO)));
    fff_TEST_FALSE(isMultiple2(BIN(IOIOOOOOOOO)));

fff_TEST_END_IMPL()
//---------------------------------------------------------+

//---------------------------------------------------------+
fff_TEST_BEGIN_IMPL(openclc99, calcOverlapSaveSampleOffset)

	/*
		scenary 1 (big example)

			Convolution example:
				- input size 18 samples
				- kernel size 5 samples
				- FFT size 2^^3 samples = 8

			Presentation:
				- Number is the fft group
				- letter a is the list of source indices
				- letter b are the fft swapped items
				- letter c is the list of target indices

			Testing:
				- list of b
				- list of c

		x-4,x-3,x-2,x-1,x00,x01,x02,x03,x04,x05,x06,x07,x08,x09,x10,x11,x12,x13    ***    k00,k01,k02,k03,k04

	0,a:    x-4,x-3,x-2,x-1,x00,x01,x02,x03
	0,b:	x-4,x00,x-2,x02,x-3,x01,x-1,x03
	0,c:	 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7
	1,a:	                x00,x01,x02,x03,x04,x05,x06,x07
	1,b:	                x00,x04,x02,x06,x01,x05,x03,x07
	1,c:	                 8 , 9 , 10, 11, 12, 13, 14, 15
	2,a:	                                x04,x05,x06,x07,x08,x09,x10,x11
	2,b:	                                x04,x08,x06,x10,x05,x09,x07,x11
	2,c:	                                 16, 17, 18, 19, 20, 21, 22, 23
	3,a:	                                                x08,x09,x10,x11,x12,x13,x14,x15
	3,b:	                                                x08,x12,x10,x14,x09,x13,x11,x15
	3,c:	                                                 24, 25, 26, 27, 28, 29, 30, 31


	*/

//                          <->                            +
	UInt from, to;

	// abbreviation for the long function name
	void (*tf)(UInt*,UInt*,UInt,UInt,UInt,UInt,UInt) =
		calcOverlapSaveSampleOffset;

	// Call scheme:
	// from pointer | to pointer
	// group id | local id | lb2 fft sample count
	// kernel sample count  |  data sample count
//                          <->                            +
	tf(&from, &to,
		0, 0, 3,
		5, 100);
	fff_TEST_OK(from==0 && to== 0);
	tf(&from, &to,
		0, 3, 3,
		5, 100);
	fff_TEST_OK(from== 6 && to== 3);
	tf(&from, &to,
		0, 6, 3,
		5, 100);
	fff_TEST_OK(from== 3 && to== 6);
	tf(&from, &to,
		0, 7, 3,
		5, 100);
	fff_TEST_OK(from== 7 && to== 7);
	tf(&from, &to,
		1, 0, 3,
		5, 100);
	fff_TEST_OK(from== 4 && to== 8);
	tf(&from, &to,
		1, 5, 3,
		5, 100);
	fff_TEST_OK(from== 9 && to==13);
	tf(&from, &to,
		2, 0, 3,
		5, 100);
	fff_TEST_OK(from== 8 && to==16);
	tf(&from, &to,
		2, 2, 3,
		5, 100);
	fff_TEST_OK(from== 10 && to==18);
	tf(&from, &to,
		3, 3, 3,
		5, 100);
	fff_TEST_OK(from==18 && to==27);
	tf(&from, &to,
		3, 7, 3,
		5, 100);
	fff_TEST_OK(from==19 && to==31);	
	tf(&from, &to,
		1, 12, 5,
		8, 100);
	fff_TEST_OK(from==31 && to==44);
	tf(&from, &to,
		0, 1, 5,
		8, 100);
	fff_TEST_OK(from==16 && to==1);

//                          <->                            +
	// scenary 2 ( x[14] ** h[2], FFT=4 )
	fff_TEST_EQ(
		calcOverlapSaveSampleCount(14, 2, 2), 20);

	tf(&from, &to,
		0, 0, 2,
		2, 14);
	fff_TEST_OK(from==0 && to==0);
	tf(&from, &to,
		0, 1, 2,
		2, 14);
	fff_TEST_OK(from==2 && to==1);
	tf(&from, &to,
		0, 2, 2,
		2, 14);
	fff_TEST_OK(from==1 && to==2);
	tf(&from, &to,
		0, 3, 2,
		2, 14);
	fff_TEST_OK(from==3 && to==3);
	tf(&from, &to,
		1, 0, 2,
		2, 14);
	fff_TEST_OK(from==3 && to==4);
	tf(&from, &to,
		1, 1, 2,
		2, 14);
	fff_TEST_OK(from==5 && to==5);
	tf(&from, &to,
		1, 2, 2,
		2, 14);
	fff_TEST_OK(from==4 && to==6);
	tf(&from, &to,
		1, 3, 2,
		2, 14);
	fff_TEST_OK(from==6 && to==7);
	tf(&from, &to,
		2, 0, 2,
		2, 14);
	fff_TEST_OK(from==6 && to==8);
	tf(&from, &to,
		2, 1, 2,
		2, 14);
	fff_TEST_OK(from==8 && to==9);
	tf(&from, &to,
		2, 2, 2,
		2, 14);
	fff_TEST_OK(from==7 && to==10);
	tf(&from, &to,
		2, 3, 2,
		2, 14);
	fff_TEST_OK(from==9 && to==11);
	tf(&from, &to,
		3, 0, 2,
		2, 14);
	fff_TEST_OK(from==9 && to==12);
	tf(&from, &to,
		3, 1, 2,
		2, 14);
	fff_TEST_OK(from==11 && to==13);
	tf(&from, &to,
		3, 2, 2,
		2, 14);
	fff_TEST_OK(from==10 && to==14);
	tf(&from, &to,
		3, 3, 2,
		2, 14);
	fff_TEST_OK(from==12 && to==15);
	tf(&from, &to,
		4, 0, 2,
		2, 14);
	fff_TEST_OK(from==12 && to==16);
	tf(&from, &to,
		4, 1, 2,
		2, 14);
	fff_TEST_OK(from==14 && to==17);
	tf(&from, &to,
		4, 2, 2,
		2, 14);
	fff_TEST_OK(from==13 && to==18);
	tf(&from, &to,
		4, 3, 2,
		2, 14);
	fff_TEST_OK(from==15 && to==19);

fff_TEST_END_IMPL()
//---------------------------------------------------------+

//---------------------------------------------------------+
fff_TEST_BEGIN_IMPL(openclc99, calcOverlapSaveSampleCount)
		
	fff_TEST_EQ(calcOverlapSaveSampleCount(
		100, 5, 10), 160);
	fff_TEST_EQ(calcOverlapSaveSampleCount(
		1023, 7, 7), 1152);
	fff_TEST_EQ(calcOverlapSaveSampleCount(
		17367, 8, 0), 17408);
	fff_TEST_EQ(calcOverlapSaveSampleCount(
		1025, 10, 1), 2048);
	fff_TEST_EQ(calcOverlapSaveSampleCount(
		44100, 12, 100), 49152);

	// long impulse response
	fff_TEST_EQ(calcOverlapSaveSampleCount(
		1000000, 20, 220500), 2097152);

fff_TEST_END_IMPL()
//---------------------------------------------------------+

//---------------------------------------------------------+
fff_TEST_BEGIN_IMPL(openclc99, fftButterfly)

	UInt first, second;
	Sample piE;

	void (*bf)(UInt*,UInt*, Sample*, UInt, UInt) =
		_fftButterfly;

//                          <->                            +
	// N=8
	bf(&first, &second, &piE, 0, 0);
	fff_TEST_OK(first==0 && second==1 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 1);
	fff_TEST_OK(first==2 && second==3 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 2); 
	fff_TEST_OK(first==4 && second==5 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 3); 
	fff_TEST_OK(first==6 && second==7 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 1, 0); 
	fff_TEST_OK(first==0 && second==2 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 1, 1); 
	fff_TEST_OK(first==1 && second==3 &&
		piE == (Sample)0.5);
	bf(&first, &second, &piE, 1, 2); 
	fff_TEST_OK(first==4 && second==6 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 1, 3); 
	fff_TEST_OK(first==5 && second==7 &&
		piE == (Sample)0.5);
	bf(&first, &second, &piE, 2, 0); 
	fff_TEST_OK(first==0 && second==4 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 2, 1); 
	fff_TEST_OK(first==1 && second==5 &&
		piE == (Sample)0.25);
	bf(&first, &second, &piE, 2, 2); 
	fff_TEST_OK(first==2 && second==6 &&
		piE == (Sample)0.5);
	bf(&first, &second, &piE, 2, 3); 
	fff_TEST_OK(first==3 && second==7 &&
		piE == (Sample)0.75);

//                          <->                            +
	// N=16
	bf(&first, &second, &piE, 0, 0);
	fff_TEST_OK(first==0 && second==1 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 1);
	fff_TEST_OK(first==2 && second==3 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 2);
	fff_TEST_OK(first==4 && second==5 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 3);
	fff_TEST_OK(first==6 && second==7 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 4);
	fff_TEST_OK(first==8 && second==9 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 5);
	fff_TEST_OK(first==10 && second==11 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 6);
	fff_TEST_OK(first==12 && second==13 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 0, 7);
	fff_TEST_OK(first==14 && second==15 &&
		piE == (Sample)0);

	bf(&first, &second, &piE, 1, 0);
	fff_TEST_OK(first==0 && second==2 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 1, 1);
	fff_TEST_OK(first==1 && second==3 &&
		piE == (Sample)0.5);
	bf(&first, &second, &piE, 1, 2);
	fff_TEST_OK(first==4 && second==6 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 1, 3);
	fff_TEST_OK(first==5 && second==7 &&
		piE == (Sample)0.5);
	bf(&first, &second, &piE, 1, 4);
	fff_TEST_OK(first==8 && second==10 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 1, 5);
	fff_TEST_OK(first==9 && second==11 &&
		piE == (Sample)0.5);
	bf(&first, &second, &piE, 1, 6);
	fff_TEST_OK(first==12 && second==14 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 1, 7);
	fff_TEST_OK(first==13 && second==15 &&
		piE == (Sample)0.5);

	bf(&first, &second, &piE, 2, 0);
	fff_TEST_OK(first==0 && second==4 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 2, 1);
	fff_TEST_OK(first==1 && second==5 &&
		piE == (Sample)0.25);
	bf(&first, &second, &piE, 2, 2);
	fff_TEST_OK(first==2 && second==6 &&
		piE == (Sample)0.5);
	bf(&first, &second, &piE, 2, 3);
	fff_TEST_OK(first==3 && second==7 &&
		piE == (Sample)0.75);
	bf(&first, &second, &piE, 2, 4);
	fff_TEST_OK(first==8 && second==12 &&
		piE == (Sample)0);
	bf(&first, &second, &piE, 2, 5);
	fff_TEST_OK(first==9 && second==13 &&
		piE == (Sample)0.25);
	bf(&first, &second, &piE, 2, 6);
	fff_TEST_OK(first==10 && second==14 &&
		piE == (Sample)0.5);
	bf(&first, &second, &piE, 2, 7);
	fff_TEST_OK(first==11 && second==15 &&
		piE == (Sample)0.75);

	bf(&first, &second, &piE, 3, 0);
	fff_TEST_OK(first==0 && second==8 &&
		piE == (Sample)0.000);
	bf(&first, &second, &piE, 3, 1);
	fff_TEST_OK(first==1 && second==9 &&
		piE == (Sample)0.125);
	bf(&first, &second, &piE, 3, 2);
	fff_TEST_OK(first==2 && second==10 &&
		piE == (Sample)0.250);
	bf(&first, &second, &piE, 3, 3);
	fff_TEST_OK(first==3 && second==11 &&
		piE == (Sample)0.375);
	bf(&first, &second, &piE, 3, 4);
	fff_TEST_OK(first==4 && second==12 &&
		piE == (Sample)0.500);
	bf(&first, &second, &piE, 3, 5);
	fff_TEST_OK(first==5 && second==13 &&
		piE == (Sample)0.625);
	bf(&first, &second, &piE, 3, 6);
	fff_TEST_OK(first==6 && second==14 &&
		piE == (Sample)0.750);
	bf(&first, &second, &piE, 3, 7);
	fff_TEST_OK(first==7 && second==15 &&
		piE == (Sample)0.875);

fff_TEST_END_IMPL()
//---------------------------------------------------------+


fff_TEST_BEGIN_IMPL(openclc99, dconv)

    {
    const UInt
        xsize = 3,
        hsize = 2,
        ysize = xsize+hsize-1;

    Sample
        x[xsize]={1,2,3},
        h[hsize]={2,3},
        y[ysize];

        for(UInt i = 0; i < ysize; ++i)
        {
            y[i] = dconv(
                x, xsize,
                h, hsize,
                i);
        }

        fff_TEST_EQ(y[0], 2);
        fff_TEST_EQ(y[1], 7);
        fff_TEST_EQ(y[2], 12);
        fff_TEST_EQ(y[3], 9);
    }

    {
    const UInt
        xsize = 8,
        hsize = 5,
        ysize = xsize+hsize-1;
    
    Sample
        x[xsize]={0,1,2,2,0,1,2,4},
        h[hsize]={0,1,2,1,2},
        y[ysize],
        y2[ysize];

    // x ** h
    for(UInt i = 0; i < ysize; ++i)
    {
        y[i] = dconv(
            x, xsize,
            h, hsize,
            i);
    }
    // h ** x
    for(UInt i = 0; i < ysize; ++i)
    {
        y2[i] = dconv(
            h, hsize,
            x, xsize,
            i);
    }

    // See the (stupid) algorithm (dont calculate).
    // Right factor of the pairs is the value of h
    // and same per column
    //                ?*0   ?*1   ?*2   ?*1   ?*2
    // Left factor is a x value start at the first
    // item. Every line shift right (max hsize items).

    fff_TEST_EQ(y[0], 0*0);
    //                a
    fff_TEST_EQ(y[1], 1*0 + 0*1);
    //                b     a
    fff_TEST_EQ(y[2], 2*0 + 1*1 + 0*2);
    //                c     b     a
    fff_TEST_EQ(y[3], 2*0 + 2*1 + 1*2 + 0*1);
    //                d     c     b     a
    fff_TEST_EQ(y[4], 0*0 + 2*1 + 2*2 + 1*1 + 0*2);
    //                e     d     c     b     a
    fff_TEST_EQ(y[5], 1*0 + 0*1 + 2*2 + 2*1 + 1*2);
    //                f     e     d     c     b
    fff_TEST_EQ(y[6], 2*0 + 1*1 + 0*2 + 2*1 + 2*2);
    //                g     f     e     d     c
    fff_TEST_EQ(y[7], 4*0 + 2*1 + 1*2 + 0*1 + 2*2);
    //                      g     f     e     d
    fff_TEST_EQ(y[8],       4*1 + 2*2 + 1*1 + 0*2);
    //                            g     f     e
    fff_TEST_EQ(y[9],             4*2 + 2*1 + 1*2);
    //                                  g     f
    fff_TEST_EQ(y[10],                  4*1 + 2*2);
    //                                        g
    fff_TEST_EQ(y[11],                        4*2);
    //                                         

    fff_TEST_EQ(
        variance(y, y2, ysize), 0.f);

    }


    {
    const UInt
        xsize = 3,
        hsize = 6,
        ysize = xsize+hsize-1;

    Sample
        x[xsize]={1,2,3},
        h[hsize]={0,0,1,1,2,0},
        y[ysize];

        for(UInt i = 0; i < ysize; ++i)
        {
            y[i] = dconv(
                x, xsize,
                h, hsize,
                i);
        }

        fff_TEST_EQ(y[0], 1*0);
        fff_TEST_EQ(y[1], 2*0 + 1*0);
        fff_TEST_EQ(y[2], 3*0 + 2*0 + 1*1);
        fff_TEST_EQ(y[3],       3*0 + 2*1 + 1*1);
        fff_TEST_EQ(y[4],             3*1 + 2*1 + 1*2);
        fff_TEST_EQ(y[5],                   3*1 + 2*2 + 1*0);
        fff_TEST_EQ(y[6],                         3*2 + 2*0);
        fff_TEST_EQ(y[7],                               3*0);

    }

fff_TEST_END_IMPL()

}
}
}

#endif