/*----------------------------------------------------------
 *    The Fast Filtering Framework implements an LTI filter
 *    with Khronos Group's OpenCL.
 *    Copyright (C) 2012  Philipp Renoth
 *----------------------------------------------------------
 *    This program is free software: you can redistribute
 *    it and/or modify it under the terms of the
 *    GNU General Public License as published by the
 *    Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will
 *    be useful, but WITHOUT ANY WARRANTY; without even the
 *    implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the
 *    GNU General Public License along with this program.
 *    If not, see <http://www.gnu.org/licenses/>.
 *--------------------------------------------------------*/

#include "../include/fffTesting.h"

_fff_USE_NAMESPACE

void _fff_NAMESPACE_PREFIX fff_test()
{
	/*
		Bit swap testing. Only with respect to the
		size parameter. E.g. a number with a width
		higher than the size (in bits) will be
		truncated left of the "size"-bits.
	*/
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

	assert(_bit_reversal(_bit_reversal(99,13),13) == 99);
	assert(_bit_reversal(_bit_reversal(9999,13),13) == 1807);
	assert(_bit_reversal(_bit_reversal(999999,13),13) == 575);
	assert(_bit_reversal(_bit_reversal(99999999,13),13) == 255);

	assert(_bit_reversal(_bit_reversal(99,32),32) == 99);
	assert(_bit_reversal(_bit_reversal(9999,32),32) == 9999);
	assert(_bit_reversal(_bit_reversal(999999,32),32) == 999999);
	assert(_bit_reversal(_bit_reversal(99999999,32),32) == 99999999);

	
	
	/*
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


	UInt from, to;

	//								            group id | local id | lb2 fftsize | kernel size | data size	 | ASSERTION
	_calcOverlapSaveOffset(&from, &to,         0,         0,            3,            5,		100		); assert(from==0 && to== 0);
	_calcOverlapSaveOffset(&from, &to,         0,         3,            3,            5,		100		); assert(from== 6 && to== 3);
	_calcOverlapSaveOffset(&from, &to,         0,         6,            3,            5,		100		); assert(from==3 && to== 6);
	_calcOverlapSaveOffset(&from, &to,         0,         7,            3,            5,		100		); assert(from== 7 && to== 7);
	_calcOverlapSaveOffset(&from, &to,         1,         0,            3,            5,		100		); assert(from== 4 && to== 8);
	_calcOverlapSaveOffset(&from, &to,         1,         5,            3,            5,		100		); assert(from== 9 && to==13);

	_calcOverlapSaveOffset(&from, &to,         2,         0,            3,            5,		100		); assert(from== 8 && to==16);
	_calcOverlapSaveOffset(&from, &to,         2,         2,            3,            5,		100		); assert(from== 10 && to==18);

	_calcOverlapSaveOffset(&from, &to,         3,         3,            3,            5,		100		); assert(from==18 && to==27);
	_calcOverlapSaveOffset(&from, &to,         3,         7,            3,            5,		100		); assert(from==19 && to==31);
	



	_calcOverlapSaveOffset(&from, &to,		 1,			12,			5,				8,		100		); assert(from==31 && to==44);
	_calcOverlapSaveOffset(&from, &to,		 0,			1,			5,				8,		100		); assert(from==16 && to==1);


	// scenary 2 ( x[14] ** h[2], FFT=4 )
	assert(_calcOverlapSaveSize(14, 2, 2) == 20);
	_calcOverlapSaveOffset(&from, &to,		 0,			0,			2,				2,		14		); assert(from==0 && to==0);
	_calcOverlapSaveOffset(&from, &to,		 0,			1,			2,				2,		14		); assert(from==2 && to==1);
	_calcOverlapSaveOffset(&from, &to,		 0,			2,			2,				2,		14		); assert(from==1 && to==2);
	_calcOverlapSaveOffset(&from, &to,		 0,			3,			2,				2,		14		); assert(from==3 && to==3);
	_calcOverlapSaveOffset(&from, &to,		 1,			0,			2,				2,		14		); assert(from==3 && to==4);
	_calcOverlapSaveOffset(&from, &to,		 1,			1,			2,				2,		14		); assert(from==5 && to==5);
	_calcOverlapSaveOffset(&from, &to,		 1,			2,			2,				2,		14		); assert(from==4 && to==6);
	_calcOverlapSaveOffset(&from, &to,		 1,			3,			2,				2,		14		); assert(from==6 && to==7);
	_calcOverlapSaveOffset(&from, &to,		 2,			0,			2,				2,		14		); assert(from==6 && to==8);
	_calcOverlapSaveOffset(&from, &to,		 2,			1,			2,				2,		14		); assert(from==8 && to==9);
	_calcOverlapSaveOffset(&from, &to,		 2,			2,			2,				2,		14		); assert(from==7 && to==10);
	_calcOverlapSaveOffset(&from, &to,		 2,			3,			2,				2,		14		); assert(from==9 && to==11);
	_calcOverlapSaveOffset(&from, &to,		 3,			0,			2,				2,		14		); assert(from==9 && to==12);
	_calcOverlapSaveOffset(&from, &to,		 3,			1,			2,				2,		14		); assert(from==11 && to==13);
	_calcOverlapSaveOffset(&from, &to,		 3,			2,			2,				2,		14		); assert(from==10 && to==14);
	_calcOverlapSaveOffset(&from, &to,		 3,			3,			2,				2,		14		); assert(from==12 && to==15);
	_calcOverlapSaveOffset(&from, &to,		 4,			0,			2,				2,		14		); assert(from==12 && to==16);
	_calcOverlapSaveOffset(&from, &to,		 4,			1,			2,				2,		14		); assert(from==14 && to==17);
	_calcOverlapSaveOffset(&from, &to,		 4,			2,			2,				2,		14		); assert(from==13 && to==18);
	_calcOverlapSaveOffset(&from, &to,		 4,			3,			2,				2,		14		); assert(from==15 && to==19);

	// std::cout << "FROM: " << from << ", TO: " << to << std::endl; std::cin.get();
	//assert(from==16 && to==1);

	/*
		check overlap save sizes

	*/

	//std::cout << fff_calcOverlapSaveSize(10, 2, 3) << std::endl << std::endl; std::cin.get();
	assert(_calcOverlapSaveSize(100, 5, 10)==160);
	assert(_calcOverlapSaveSize(1023, 7, 7)==1152);
	assert(_calcOverlapSaveSize(17367, 8, 0)==17408);
	assert(_calcOverlapSaveSize(1025, 10, 1)==2048);
	assert(_calcOverlapSaveSize(44100, 12, 100)==49152);

	// extreme: ~5 secs FIR (44100 samples per minute)
	assert(_calcOverlapSaveSize(1000000, 20, 220500)==2097152);





	/*
		FFT-butterfly testing.


	*/
	UInt first, second;
	Sample piE;

	// N=8
	_fft_butterfly(&first, &second, &piE, 0, 0); assert(first==0 && second==1 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 1); assert(first==2 && second==3 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 2); assert(first==4 && second==5 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 3); assert(first==6 && second==7 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 1, 0); assert(first==0 && second==2 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 1, 1); assert(first==1 && second==3 && piE == (Sample)0.5);
	_fft_butterfly(&first, &second, &piE, 1, 2); assert(first==4 && second==6 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 1, 3); assert(first==5 && second==7 && piE == (Sample)0.5);
	_fft_butterfly(&first, &second, &piE, 2, 0); assert(first==0 && second==4 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 2, 1); assert(first==1 && second==5 && piE == (Sample)0.25);
	_fft_butterfly(&first, &second, &piE, 2, 2); assert(first==2 && second==6 && piE == (Sample)0.5);
	_fft_butterfly(&first, &second, &piE, 2, 3); assert(first==3 && second==7 && piE == (Sample)0.75);

	// N=16
	_fft_butterfly(&first, &second, &piE, 0, 0); assert(first==0 && second==1 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 1); assert(first==2 && second==3 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 2); assert(first==4 && second==5 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 3); assert(first==6 && second==7 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 4); assert(first==8 && second==9 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 5); assert(first==10 && second==11 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 6); assert(first==12 && second==13 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 0, 7); assert(first==14 && second==15 && piE == (Sample)0);

	_fft_butterfly(&first, &second, &piE, 1, 0); assert(first==0 && second==2 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 1, 1); assert(first==1 && second==3 && piE == (Sample)0.5);
	_fft_butterfly(&first, &second, &piE, 1, 2); assert(first==4 && second==6 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 1, 3); assert(first==5 && second==7 && piE == (Sample)0.5);
	_fft_butterfly(&first, &second, &piE, 1, 4); assert(first==8 && second==10 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 1, 5); assert(first==9 && second==11 && piE == (Sample)0.5);
	_fft_butterfly(&first, &second, &piE, 1, 6); assert(first==12 && second==14 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 1, 7); assert(first==13 && second==15 && piE == (Sample)0.5);

	_fft_butterfly(&first, &second, &piE, 2, 0); assert(first==0 && second==4 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 2, 1); assert(first==1 && second==5 && piE == (Sample)0.25);
	_fft_butterfly(&first, &second, &piE, 2, 2); assert(first==2 && second==6 && piE == (Sample)0.5);
	_fft_butterfly(&first, &second, &piE, 2, 3); assert(first==3 && second==7 && piE == (Sample)0.75);
	_fft_butterfly(&first, &second, &piE, 2, 4); assert(first==8 && second==12 && piE == (Sample)0);
	_fft_butterfly(&first, &second, &piE, 2, 5); assert(first==9 && second==13 && piE == (Sample)0.25);
	_fft_butterfly(&first, &second, &piE, 2, 6); assert(first==10 && second==14 && piE == (Sample)0.5);
	_fft_butterfly(&first, &second, &piE, 2, 7); assert(first==11 && second==15 && piE == (Sample)0.75);

	_fft_butterfly(&first, &second, &piE, 3, 0); assert(first==0 && second==8 && piE == (Sample)0.000);
	_fft_butterfly(&first, &second, &piE, 3, 1); assert(first==1 && second==9 && piE == (Sample)0.125);
	_fft_butterfly(&first, &second, &piE, 3, 2); assert(first==2 && second==10 && piE == (Sample)0.250);
	_fft_butterfly(&first, &second, &piE, 3, 3); assert(first==3 && second==11 && piE == (Sample)0.375);
	_fft_butterfly(&first, &second, &piE, 3, 4); assert(first==4 && second==12 && piE == (Sample)0.500);
	_fft_butterfly(&first, &second, &piE, 3, 5); assert(first==5 && second==13 && piE == (Sample)0.625);
	_fft_butterfly(&first, &second, &piE, 3, 6); assert(first==6 && second==14 && piE == (Sample)0.750);
	_fft_butterfly(&first, &second, &piE, 3, 7); assert(first==7 && second==15 && piE == (Sample)0.875);
	

	// test fffSamples
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

	// test fffComplexSamples
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






}