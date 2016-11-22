// /*
//   Copyright (c) 2010-2011, Intel Corporation
//   All rights reserved.

//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are
//   met:

//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.

//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.

//     * Neither the name of Intel Corporation nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.


//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
//    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
//    OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
// */

// #ifdef _MSC_VER
// #define _CRT_SECURE_NO_WARNINGS
// #define NOMINMAX
// #pragma warning (disable: 4244)
// #pragma warning (disable: 4305)
// #endif

#include <stdio.h>
#include <algorithm>
#include "../timing.h"
// #include "mandelbrot_ispc.h"
#include "sqrt_ispc.h"
// #include "sqrt_ispc_avx.h"
#include <string.h>
#include <cstdlib>
using namespace ispc;

#include <math.h>
#include <iostream>
using namespace std;

double mysqrt(double n)
{
    if(n == 0 || n == 1) return n ; 
    double pres = 0.0001 ;
    double x_0 = n/4 ; 
    double x_1 = x_0-   (((x_0*x_0)-n)/(2*x_0)) ; 
    while((x_1 - x_0 > pres) || (x_1 - x_0 < -pres)){
        x_0 = x_1 ;
    x_1 = x_0-   (((x_0*x_0)-n)/(2*x_0)) ; 
    }
    return x_1 ; 
}
int main(int argc, char** argv) {
  // Prepare data
  // long int N = 1000000000000;
	int N = 0;
  int num_task = 1;
	if (argc != 3){
		cout<<"Usage ./squareroot [number of elements] [number of tasks] \n";
		return 0;
	}
	if (atoi(argv[1]) && atoi(argv[2])){
		N = atoi(argv[1]);
    num_task = atoi(argv[2]);
	}
	else{
		cout<<"please only input a number \n";
		return 0;
	}

  srand(10);
  float *x = new float[N];

  // Populate the array with random elements
  for(int i =  0; i < N ; i+=1){
      // float z = i*i ; 
      x[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5));;
  }

  // Preallocate memory for output matrix
  float *out = new float[N];

  // Timing for square root in ISPC
  // double minISPC = 1e30;
  reset_and_start_timer();
  sqrt_ispc(N, x, out);
  double dt = get_elapsed_mcycles();
  // minISPC = std::min(minISPC, dt);
  cout<<"ISPC time with "<<N<<" elements is "<<dt<<" million cycles\n";

  // Resetting the buffer
  for (int i = 0; i < N; i++){
    out[i] = 0.0;
  }

  // Timing for square root from math.h
  reset_and_start_timer();
  for (int i = 0; i < N; i++){
    out[i] = mysqrt(x[i]);
  }
  dt = get_elapsed_mcycles();
  cout<<"Sequential time with "<<N<<" elements is "<<dt<< " million cylcles\n";


  // Resetting the buffer
  for (int i = 0; i < N; i++){
    out[i] = 0.0;
  }

  // Timing for tasked square root
  reset_and_start_timer();
  sqrt_ispc_task(N, x, out, num_task);
  dt = get_elapsed_mcycles();
  cout<<"ISPC with taks time with "<<N<<" elements is "<<dt<<" million cycles\n";


  delete[] x;
  delete[] out;
  return 0;
}