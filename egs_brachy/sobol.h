/*! \file sobol.h
 *
 * \brief Header file for Sobol library functions from http://people.sc.fsu.edu/~jburkardt/cpp_src/sobol/sobol.html
 * \author John Burkardt.
 * \license GNU LGPL
 *
 */
#ifndef AE_SOBOL_
#define AE_SOBOL_

#ifdef WIN32

    #ifdef BUILD_SOBOL_DLL
        #define AE_SOBOL_EXPORT __declspec(dllexport)
    #else
        #define AE_SOBOL_EXPORT __declspec(dllimport)
    #endif
    #define AE_SOBOL_LOCAL

#else

    #ifdef HAVE_VISIBILITY
        #define AE_SOBOL_EXPORT __attribute__ ((visibility ("default")))
        #define AE_SOBOL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define AE_SOBOL_EXPORT
        #define AE_SOBOL_LOCAL
    #endif

#endif

#include "egs_rndm.h"
#include "egs_input.h"
#include "egs_functions.h"

namespace sobol {

int i4_bit_hi1(int n);
int i4_bit_lo0(int n);
int i4_max(int i1, int i2);
int i4_min(int i1, int i2);
void i4_sobol(int dim_num, int *seed, float quasi[ ]);
float *i4_sobol_generate(int m, int n, int skip);
int i4_uniform(int b, int c, int *seed);

int i8_bit_hi1(long long int n);
int i8_bit_lo0(long long int n);
long long int i8_max(long long int i1, long long int i2);
long long int i8_min(long long int i1, long long int i2);
void i8_sobol(int dim_num, long long int *seed, double quasi[ ]);
double *i8_sobol_generate(int m, int n, int skip);
long long int i8_uniform(long long int b, long long int c, int *seed);

float r4_abs(float x);
int r4_nint(float x);
float r4_uniform_01(int *seed);

double r8_abs(double x);
int r8_nint(double x);
double r8_uniform_01(int *seed);

void r8mat_write(string output_filename, int m, int n, double table[]);

int tau_sobol(int dim_num);

}



#endif
