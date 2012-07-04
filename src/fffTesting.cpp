//---------------------------------------------------------+
// fff/src/fffTesting.cpp
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
//!	\file		fffTesting.cpp
//!
//!	\author		Philipp Renoth <fff@aitch.de>
//!	\brief		fff testing functions.
//!	\copyright	GNU General Public License v3 2012.
//---------------------------------------------------------+

//---------------------------------------------------------+
#include "../include/fffTesting.h"
//---------------------------------------------------------+

//---------------------------------------------------------+
_fff_BEGIN_NAMESPACE
//---------------------------------------------------------+

//---------------------------------------------------------+
	void test_bitReversal()
	{
		// test some numbers
		assert(_bit_reversal(4, 0) == 0);
		assert(_bit_reversal(1, 0) == 0);

		assert(_bit_reversal(6, 1)  == 0);
		assert(_bit_reversal(99, 1) == 1);

		assert(_bit_reversal(0, 2) == 0);
		assert(_bit_reversal(1, 2) == 2);
		assert(_bit_reversal(2, 2) == 1);
		assert(_bit_reversal(3, 2) == 3);
		assert(_bit_reversal(6, 2) == 1);
		assert(_bit_reversal(7, 2) == 3);

		assert(_bit_reversal(0, 3) == 0);
		assert(_bit_reversal(1, 3) == 4);
		assert(_bit_reversal(2, 3) == 2);
		assert(_bit_reversal(3, 3) == 6);
		assert(_bit_reversal(4, 3) == 1);
		assert(_bit_reversal(5, 3) == 5);
		assert(_bit_reversal(6, 3) == 3);
		assert(_bit_reversal(7, 3) == 7);

		assert(_bit_reversal(1, 5) == 16);

		assert(_bit_reversal(99,9) == 396);

		assert(_bit_reversal(99,9) == 396);

		// test large numbers
		assert(
			_bit_reversal(
				_bit_reversal(99,32),
				32) == 99);
		assert(
			_bit_reversal(
				_bit_reversal(9999,32),
				32) == 9999);
		assert(
			_bit_reversal(
				_bit_reversal(999999,32),
				32) == 999999);
		assert(
			_bit_reversal(
				_bit_reversal(99999999,32),
				32) == 99999999);

		// test some numbers larger than the given
		// bit-width to test the truncation (2^13 = 8192)
		assert(
			_bit_reversal(
				_bit_reversal(99,13),
				13) == 99);
		assert(
			_bit_reversal(
				_bit_reversal(9999,13),13) == 1807);
		assert(
			_bit_reversal(
				_bit_reversal(999999,13),13) == 575);
		assert(
			_bit_reversal(
				_bit_reversal(99999999,13),13) == 255);
	}
//---------------------------------------------------------+


//---------------------------------------------------------+
	void test_nextBinStep()
	{
		// test some numbers incl. zero
		assert(_nextBinStep(0) == 1);
		assert(_nextBinStep(10) == 16);
		assert(_nextBinStep(50) == 64);

		// test numbers near step numbers
		assert(_nextBinStep(3) == 4);
		assert(_nextBinStep(127) == 128);
		assert(_nextBinStep(257) == 512);
		assert(_nextBinStep(65535) == 65536);

		// test step numbers to get
		// the next step number
		assert(_nextBinStep(1) == 2);
		assert(_nextBinStep(2) == 4);
		assert(_nextBinStep(4) == 8);
		assert(_nextBinStep(65536) == 65536<<1);
	}
//---------------------------------------------------------+

//---------------------------------------------------------+
	void test_calcOverlapSaveSampleOffset()
	{
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
			_calcOverlapSaveSampleOffset;

		// Call scheme:
		// from pointer | to pointer
		// group id | local id | lb2 fft sample count
		// kernel sample count  |  data sample count
//                          <->                            +
		tf(&from, &to,
			0, 0, 3,
			5, 100);
		assert(from==0 && to== 0);
		tf(&from, &to,
			0, 3, 3,
			5, 100);
		assert(from== 6 && to== 3);
		tf(&from, &to,
			0, 6, 3,
			5, 100);
		assert(from== 3 && to== 6);
		tf(&from, &to,
			0, 7, 3,
			5, 100);
		assert(from== 7 && to== 7);
		tf(&from, &to,
			1, 0, 3,
			5, 100);
		assert(from== 4 && to== 8);
		tf(&from, &to,
			1, 5, 3,
			5, 100);
		assert(from== 9 && to==13);
		tf(&from, &to,
			2, 0, 3,
			5, 100);
		assert(from== 8 && to==16);
		tf(&from, &to,
			2, 2, 3,
			5, 100);
		assert(from== 10 && to==18);
		tf(&from, &to,
			3, 3, 3,
			5, 100);
		assert(from==18 && to==27);
		tf(&from, &to,
			3, 7, 3,
			5, 100);
		assert(from==19 && to==31);	
		tf(&from, &to,
			1, 12, 5,
			8, 100);
		assert(from==31 && to==44);
		tf(&from, &to,
			0, 1, 5,
			8, 100);
		assert(from==16 && to==1);

//                          <->                            +
		// scenary 2 ( x[14] ** h[2], FFT=4 )
		assert(
			_calcOverlapSaveSampleCount(14, 2, 2) == 20);

		tf(&from, &to,
			0, 0, 2,
			2, 14);
		assert(from==0 && to==0);
		tf(&from, &to,
			0, 1, 2,
			2, 14);
		assert(from==2 && to==1);
		tf(&from, &to,
			0, 2, 2,
			2, 14);
		assert(from==1 && to==2);
		tf(&from, &to,
			0, 3, 2,
			2, 14);
		assert(from==3 && to==3);
		tf(&from, &to,
			1, 0, 2,
			2, 14);
		assert(from==3 && to==4);
		tf(&from, &to,
			1, 1, 2,
			2, 14);
		assert(from==5 && to==5);
		tf(&from, &to,
			1, 2, 2,
			2, 14);
		assert(from==4 && to==6);
		tf(&from, &to,
			1, 3, 2,
			2, 14);
		assert(from==6 && to==7);
		tf(&from, &to,
			2, 0, 2,
			2, 14);
		assert(from==6 && to==8);
		tf(&from, &to,
			2, 1, 2,
			2, 14);
		assert(from==8 && to==9);
		tf(&from, &to,
			2, 2, 2,
			2, 14);
		assert(from==7 && to==10);
		tf(&from, &to,
			2, 3, 2,
			2, 14);
		assert(from==9 && to==11);
		tf(&from, &to,
			3, 0, 2,
			2, 14);
		assert(from==9 && to==12);
		tf(&from, &to,
			3, 1, 2,
			2, 14);
		assert(from==11 && to==13);
		tf(&from, &to,
			3, 2, 2,
			2, 14);
		assert(from==10 && to==14);
		tf(&from, &to,
			3, 3, 2,
			2, 14);
		assert(from==12 && to==15);
		tf(&from, &to,
			4, 0, 2,
			2, 14);
		assert(from==12 && to==16);
		tf(&from, &to,
			4, 1, 2,
			2, 14);
		assert(from==14 && to==17);
		tf(&from, &to,
			4, 2, 2,
			2, 14);
		assert(from==13 && to==18);
		tf(&from, &to,
			4, 3, 2,
			2, 14);
		assert(from==15 && to==19);

	}
//---------------------------------------------------------+

//---------------------------------------------------------+
	void test_calcOverlapSaveSampleCount()
	{
		
		assert(_calcOverlapSaveSampleCount(
			100, 5, 10)==160);
		assert(_calcOverlapSaveSampleCount(
			1023, 7, 7)==1152);
		assert(_calcOverlapSaveSampleCount(
			17367, 8, 0)==17408);
		assert(_calcOverlapSaveSampleCount(
			1025, 10, 1)==2048);
		assert(_calcOverlapSaveSampleCount(
			44100, 12, 100)==49152);

		// long impulse response
		assert(_calcOverlapSaveSampleCount(
			1000000, 20, 220500)==2097152);
	}
//---------------------------------------------------------+

//---------------------------------------------------------+
	void test_fftButterfly()
	{

		UInt first, second;
		Sample piE;

		void (*bf)(UInt*,UInt*, Sample*, UInt, UInt) =
			_fftButterfly;

//                          <->                            +
		// N=8
		bf(&first, &second, &piE, 0, 0);
		assert(first==0 && second==1 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 1);
		assert(first==2 && second==3 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 2); 
		assert(first==4 && second==5 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 3); 
		assert(first==6 && second==7 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 1, 0); 
		assert(first==0 && second==2 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 1, 1); 
		assert(first==1 && second==3 &&
			piE == (Sample)0.5);
		bf(&first, &second, &piE, 1, 2); 
		assert(first==4 && second==6 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 1, 3); 
		assert(first==5 && second==7 &&
			piE == (Sample)0.5);
		bf(&first, &second, &piE, 2, 0); 
		assert(first==0 && second==4 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 2, 1); 
		assert(first==1 && second==5 &&
			piE == (Sample)0.25);
		bf(&first, &second, &piE, 2, 2); 
		assert(first==2 && second==6 &&
			piE == (Sample)0.5);
		bf(&first, &second, &piE, 2, 3); 
		assert(first==3 && second==7 &&
			piE == (Sample)0.75);

//                          <->                            +
		// N=16
		bf(&first, &second, &piE, 0, 0);
		assert(first==0 && second==1 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 1);
		assert(first==2 && second==3 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 2);
		assert(first==4 && second==5 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 3);
		assert(first==6 && second==7 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 4);
		assert(first==8 && second==9 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 5);
		assert(first==10 && second==11 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 6);
		assert(first==12 && second==13 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 0, 7);
		assert(first==14 && second==15 &&
			piE == (Sample)0);

		bf(&first, &second, &piE, 1, 0);
		assert(first==0 && second==2 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 1, 1);
		assert(first==1 && second==3 &&
			piE == (Sample)0.5);
		bf(&first, &second, &piE, 1, 2);
		assert(first==4 && second==6 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 1, 3);
		assert(first==5 && second==7 &&
			piE == (Sample)0.5);
		bf(&first, &second, &piE, 1, 4);
		assert(first==8 && second==10 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 1, 5);
		assert(first==9 && second==11 &&
			piE == (Sample)0.5);
		bf(&first, &second, &piE, 1, 6);
		assert(first==12 && second==14 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 1, 7);
		assert(first==13 && second==15 &&
			piE == (Sample)0.5);

		bf(&first, &second, &piE, 2, 0);
		assert(first==0 && second==4 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 2, 1);
		assert(first==1 && second==5 &&
			piE == (Sample)0.25);
		bf(&first, &second, &piE, 2, 2);
		assert(first==2 && second==6 &&
			piE == (Sample)0.5);
		bf(&first, &second, &piE, 2, 3);
		assert(first==3 && second==7 &&
			piE == (Sample)0.75);
		bf(&first, &second, &piE, 2, 4);
		assert(first==8 && second==12 &&
			piE == (Sample)0);
		bf(&first, &second, &piE, 2, 5);
		assert(first==9 && second==13 &&
			piE == (Sample)0.25);
		bf(&first, &second, &piE, 2, 6);
		assert(first==10 && second==14 &&
			piE == (Sample)0.5);
		bf(&first, &second, &piE, 2, 7);
		assert(first==11 && second==15 &&
			piE == (Sample)0.75);

		bf(&first, &second, &piE, 3, 0);
		assert(first==0 && second==8 &&
			piE == (Sample)0.000);
		bf(&first, &second, &piE, 3, 1);
		assert(first==1 && second==9 &&
			piE == (Sample)0.125);
		bf(&first, &second, &piE, 3, 2);
		assert(first==2 && second==10 &&
			piE == (Sample)0.250);
		bf(&first, &second, &piE, 3, 3);
		assert(first==3 && second==11 &&
			piE == (Sample)0.375);
		bf(&first, &second, &piE, 3, 4);
		assert(first==4 && second==12 &&
			piE == (Sample)0.500);
		bf(&first, &second, &piE, 3, 5);
		assert(first==5 && second==13 &&
			piE == (Sample)0.625);
		bf(&first, &second, &piE, 3, 6);
		assert(first==6 && second==14 &&
			piE == (Sample)0.750);
		bf(&first, &second, &piE, 3, 7);
		assert(first==7 && second==15 &&
			piE == (Sample)0.875);
	}
//---------------------------------------------------------+

//---------------------------------------------------------+
	void test_classV()
	{
		V<Sample> smp;
		
		assert(!smp.isAllocated());
		smp.alloc(4);
		smp[0] = 1.f;
		smp[1] = 2.f;
		smp[2] = 3.f;
		smp[3] = 4.f;

		assert(
			smp[0] == 1.f &&
			smp[1] == 2.f &&
			smp[2] == 3.f &&
			smp[3] == 4.f);

		V<Sample> smp2;
		assert(!smp2.isAllocated());
		smp2 = smp;
		smp.dealloc();

		assert(
			smp2[0] == 1.f &&
			smp2[1] == 2.f &&
			smp2[2] == 3.f &&
			smp2[3] == 4.f);
	}
//---------------------------------------------------------+

//---------------------------------------------------------+
	void test_classCv()
	{
		Cv<Sample> cs;
		assert(!cs.isAllocated());
		
		cs.alloc(4);
		assert(cs.isAllocated());

		cs.init();
		assert(cs.getReal()[0] == 0.f);
		assert(cs.getReal()[3] == 0.f);
		assert(cs.getImag()[0] == 0.f);
		assert(cs.getImag()[3] == 0.f);

		cs.dealloc();
		assert(!cs.isAllocated());

		cs.alloc(2);
		cs.init();
		cs.getReal()[0] = 1.f;
		cs.getImag()[0] = 2.f;
		cs.getReal()[1] = 3.f;
		cs.getImag()[1] = 4.f;

		assert(
			cs.getReal()[0] == 1.f &&
			cs.getReal()[1] == 3.f &&
			cs.getImag()[0] == 2.f &&
			cs.getImag()[1] == 4.f);

		Cv<Sample> cs2;
		assert(!cs2.isAllocated());
		cs2 = cs;
		cs.dealloc();
		assert(cs2.isAllocated());
		assert(cs2.getImag()[0] == 2.f);

	}
//---------------------------------------------------------+


//---------------------------------------------------------+
_fff_END_NAMESPACE
//---------------------------------------------------------+