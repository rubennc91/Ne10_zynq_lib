/******************************************************************************
 *
 * Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Use of the Software is limited solely to applications:
 * (a) running on a Xilinx device, or
 * (b) that interact with a Xilinx device through a bus or interconnect.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 *
 ******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
// NEON libraries
#include "Ne10.h"

// Timer libraries
#include "xtime_l.h"

#define NFFT 128

#define C2C 	1	// COMPLEX INPUT AND COMPLEX OUTPUT
#define NEON	0	// USING NEON BLOCK
#define FFT		0	// FFT = 0; iFFT = 1;

#define SHOW_RESULTS 	0
#define TEST_SAMPLES	1000

int main(void)
{
	printf("*****************************************************\n");
	printf("***  Launching the FFT test program (%d) ***\n", NFFT);
	printf("******************************************************\n");
	// Variables for  time
	XTime tStart, tEnd;
	float elapsed_time[TEST_SAMPLES];
	float time_avg = 0;

	// Size of FFT
	ne10_int32_t fftSize = NFFT;

#if C2C == 1
	// Pointer of src_input FFT
	ne10_fft_cpx_float32_t *psrc_ddr; // = XPAR_DDR_MEM_BASEADDR;
	// Pointer of dst_output FFT
	ne10_fft_cpx_float32_t *pdst_ddr; // = XPAR_DDR_MEM_BASEADDR + 0x8000;
	ne10_float32_t random_real;
	ne10_fft_cpx_float32_t ramdom_complex;
	// Configuration variable FFT
	ne10_fft_cfg_float32_t cfg;
#else
#if FFT == 1
	// Pointer of src_input FFT
	ne10_fft_cpx_float32_t *psrc_ddr; // = XPAR_DDR_MEM_BASEADDR;
	// Pointer of dst_output FFT
	ne10_float32_t *pdst_ddr; // = XPAR_DDR_MEM_BASEADDR + 0x8000;
	ne10_float32_t random_real;
	ne10_fft_cpx_float32_t ramdom_complex;
	// Configuration variable FFT
	ne10_fft_r2c_cfg_float32_t cfg;
#else
	// Pointer of src_input FFT
	ne10_float32_t *psrc_ddr; // = XPAR_DDR_MEM_BASEADDR;
	// Pointer of dst_output FFT
	ne10_fft_cpx_float32_t *pdst_ddr; // = XPAR_DDR_MEM_BASEADDR + 0x8000;
	ne10_float32_t random_real;
	// Configuration variable FFT
	ne10_fft_r2c_cfg_float32_t cfg;
#endif
#endif

	ne10_float32_t rand_num;
	// Variables to generate input and read output
	uint32_t i;

	// Initialization of platform
	init_platform();


	// Checking if NEON is avilable
	if (ne10_init() != NE10_OK)
	{
		fprintf(stderr, "Failed to initialise Ne10.\n");
		return 1;
	}

	// printf("***  Alloc memory to FFT test program  ***\n");
	// printf("***********************************************\n");
	// Configuration of pointers and coefficients of FFT.
#if C2C == 1
#if NEON == 1
	cfg = ne10_fft_alloc_c2c_float32_neon(fftSize);
	// cfg = ne10_fft_alloc_c2c_float32_c_DDR(fftSize);
#else
	cfg = ne10_fft_alloc_c2c_float32_c(fftSize);
#endif
#else
	cfg = ne10_fft_alloc_r2c_float32(fftSize);
	// cfg = ne10_fft_alloc_r2c_float32_DDR(fftSize);
	// cfg = ne10_fft_alloc_r2c_float32_heap(fftSize);
#endif

	// Initialization of pointers src and dst
#if FFT == 1
	psrc_ddr = (ne10_fft_cpx_float32_t *) (cfg->buffer + (sizeof (ne10_fft_cpx_float32_t) * (fftSize * NE10_FFT_BYTE_ALIGNMENT)));
	pdst_ddr = (ne10_float32_t *) ((ne10_float32_t *) psrc_ddr + (sizeof (ne10_float32_t) * (fftSize * NE10_FFT_BYTE_ALIGNMENT)));
#else
	psrc_ddr = (ne10_float32_t *) (cfg->buffer + (sizeof (ne10_float32_t) * (fftSize * NE10_FFT_BYTE_ALIGNMENT)));
	pdst_ddr = (ne10_fft_cpx_float32_t *) (psrc_ddr + (sizeof (ne10_fft_cpx_float32_t) * (fftSize * NE10_FFT_BYTE_ALIGNMENT)));
#endif

	// Check if we can remove this part
	if (cfg == NULL)
	{
		printf("ERROR! Alloc failed.\n");
		return -1;
	}

	// printf("***  Generating the input to FFT test program  ***\n");
	// printf("*******************************************************\n");
	// Write the input to the test
	for (i = 0; i < NFFT; i++)
	{
#if C2C == 1
		rand_num = rand();
		random_real = rand();
		ramdom_complex.r = ((random_real / RAND_MAX) * 5);
		ramdom_complex.i = ((rand_num / RAND_MAX) * 5);
		psrc_ddr[i] = (ne10_fft_cpx_float32_t) (ramdom_complex);
		// printf( "%10.4f + %10.4fi\n", i,  *((ne10_float32_t*)&psrc_ddr[i].r),  *((ne10_float32_t*)&psrc_ddr[i].i));
#else
#if FFT == 1
		rand_num = rand();
		random_real = rand();
		ramdom_complex.r = ((random_real / RAND_MAX) * 5);
		ramdom_complex.i = ((rand_num / RAND_MAX) * 5);
		psrc_ddr[i] = ramdom_complex;
		// printf( "%10.4f + %10.4fi\n", i,  *((ne10_float32_t*)&psrc_ddr[i].r),  *((ne10_float32_t*)&psrc_ddr[i].i));
#else
		rand_num = rand();
		random_real = ((rand_num / RAND_MAX) * 5);
		psrc_ddr[i] = (ne10_float32_t) (random_real);
		// printf("%.4f\n", psrc_ddr[i]);
#endif
#endif
	}

	// printf("***  Launch %d iters of FFT test program  ***\n", TEST_SAMPLES);
	// printf("********************************************************\n");
	// Launch the test of fft
	for(int m = 0; m < TEST_SAMPLES; m++)
	{
		XTime_GetTime(&tStart);
#if C2C == 1
#if NEON == 1
		ne10_fft_c2c_1d_float32_neon(pdst_ddr, psrc_ddr, cfg, FFT);
#else
		ne10_fft_c2c_1d_float32_c(pdst_ddr, psrc_ddr, cfg, FFT);
#endif
#else
#if NEON == 1
#if FFT == 1
		ne10_fft_c2r_1d_float32_neon(pdst_ddr, psrc_ddr, cfg);
#else
		ne10_fft_r2c_1d_float32_neon(pdst_ddr, psrc_ddr, cfg);
#endif
#else
#if FFT == 1
		ne10_fft_c2r_1d_float32_c(pdst_ddr, psrc_ddr, cfg);
#else
		ne10_fft_r2c_1d_float32_c(pdst_ddr, psrc_ddr, cfg);
#endif
#endif
#endif
		XTime_GetTime(&tEnd);
		elapsed_time[m] = 1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND/1000000);
	}

	// printf("***  Time of execution of FFT test program  ***\n");
	// printf("********************************************************\n");
	// Get average of execution
	for(int m = 0; m < TEST_SAMPLES; m++)
		time_avg += elapsed_time[m];

	printf("Time average of execution: %.4f us.\n", time_avg / TEST_SAMPLES);

	// printf("***  Show the result of FFT test program  ***\n");
	// printf("********************************************************\n");
	// Display the results
#if SHOW_RESULTS == 1
	for (i = 0; i < NFFT; i++)
	{
#if C2C == 1
		printf( "IN[%2d]: %10.4f + %10.4fi\t", i,  *((ne10_float32_t*)&psrc_ddr[i].r),  *((ne10_float32_t*)&psrc_ddr[i].i));
#else
#if FFT == 1
		printf( "IN[%2d]: %10.4f + %10.4fi\t", i,  *((ne10_float32_t*)&psrc_ddr[i].r),  *((ne10_float32_t*)&psrc_ddr[i].i));
#else
		printf( "IN[%2d]: %10.4f\t", i,  *((ne10_float32_t*)&psrc_ddr[i]));
#endif
#endif
		// printf( "IN: %10.4f\t", i,  *((ne10_float32_t*)&psrc_ddr[i]));
		if (i <= fftSize / 2)
		{
#if FFT == 1
			printf( "OUT[%2d]: %10.4f\t", i,  *((ne10_float32_t*)&pdst_ddr[i]));
#else
			printf("OUT[%2d]: %10.4f + %10.4fi", i, *((ne10_float32_t*)&pdst_ddr[i].r), *((ne10_float32_t*)&pdst_ddr[i].i));
#endif
		}
		printf("\n");
	}
#endif



	printf("***********************************************\n");
	printf("***  End of FFT test program  ***\n");
	printf("***********************************************\n");
	// Cleanup platform
	cleanup_platform();
	return 0;
}
