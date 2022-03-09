/******************************************************************************
*   Copyright 2021 Politecnico di Milano
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*******************************************************************************/
#include "user.h"
#include "simple_random.h"

#include <math.h>

MEASURE_GLOBAL_VARIABLES()

typedef struct{
    double re;
    double im;
} complex;

static complex roots[2];

/**
 * @brief It performs sum between two complex numbers
 * 
 * @param a first complex number
 * @param b second complex number
 * @return complex sum of a and b
 */
static complex complex_sum(complex a, complex b){
    complex res;
    res.re=a.re + b.re;
    res.im=a.im + b.im;
    return res;
}

/**
 * @brief It performs multiplication between two complex numbers
 * 
 * @param a first complex number
 * @param b second complex number
 * @return complex mult of a and b
 */
static complex complex_mult(complex a, complex b){
    complex res;
    res.re=(a.re * b.re) - (a.im*b.im);
    res.im=(a.im*b.re) + (a.re*b.im);
    return res;
}


/**
 * @brief Actual quadratic equation solver implementation
 * 
 * @param a 
 * @param b 
 * @param c 
 */
static int eq_root_routine(double a, double b, double c){

    double delta;

	if (a==0){
		if(b==0){
			return c!=0?-1:0;
		}
		roots[0].re=c/b;
		roots[0].im=0;
		return 1;
	}

	delta=b*b-4*a*c;

	if(delta>=0){
		delta=sqrt(delta);

		roots[0].re=(-b+delta)/(2*a);
		roots[0].im=0;

		roots[1].re=(-b-delta)/(2*a);
		roots[1].im=0;
	}
	else{
		delta=sqrt(-delta);
		roots[0].re=-b/(2*a);
		roots[0].im=delta/(2*a);

		roots[1].re=-b/(2*a);
		roots[1].im=-delta/(2*a);
	}
	return 2;
}

/**
 * @brief It computes complex roots of a random quadratic equation. The execution is repeated as many times
 * as the value of ITERATIONS costant. The execution time is measured through user defined MEASURE_START()/MEASURE_STOP() macros. 
 */
void eq_root(){

    double a,b,c;
    complex eq,a_cmp,b_cmp,c_cmp;
    int i,res;

    
    
    a=random_get();
    b=random_get()*2;
    c=random_get();

  
    MEASURE_START();
    for (i=0;i<ITERATIONS;i++){
        res=eq_root_routine(a,b,c);
    }
    MEASURE_STOP();

    
    if(res==0){
        CHECK_RESULT(a==0 && b==0 && c==0);
    }
    else if(res==1){
        double x=roots[0].re;
        CHECK_RESULT(a==0.0 && fabs(b*x+c)<CLASS_PRECISION);
    }
    else if(res==-1){
        CHECK_RESULT(a==0 && b==0 && c!=0);
        
    }
    else{
        a_cmp.re=a;
        a_cmp.im=0;

        b_cmp.re=b;
        b_cmp.im=0;
        
        c_cmp.re=c;
        c_cmp.im=0;
        /* eq=a*x1*x1 */
        eq=complex_mult(a_cmp,roots[0]);
        eq=complex_mult(eq,roots[0]);
        

        b_cmp=complex_mult(b_cmp,roots[0]);

        eq=complex_sum(eq,b_cmp);
        eq=complex_sum(eq,c_cmp);
        
        CHECK_RESULT(fabs(eq.re)<CLASS_PRECISION && fabs(eq.im)<CLASS_PRECISION);

        a_cmp.re=a;
        a_cmp.im=0;

        b_cmp.re=b;
        b_cmp.im=0;
        
        c_cmp.re=c;
        c_cmp.im=0;

        /* eq=a*x1*x1 */
        eq=complex_mult(a_cmp,roots[1]);
        eq=complex_mult(eq,roots[1]);

        b_cmp=complex_mult(b_cmp,roots[1]);

        eq=complex_sum(eq,b_cmp);
        eq=complex_sum(eq,c_cmp);

        CHECK_RESULT(fabs(eq.re)<1e-10 && fabs(eq.im)<CLASS_PRECISION);
    }
    

    
}