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

/*
    OUTPUT INTERPRETATION:

    0: CLEAR
    1: CLIMB LOW SPEED
    2: DESCEND LOW SPEED
    3: CLIMB HIGH SPEED
    4: DESCEND HIGH SPEED
    5: TRAFFIC TRAFFIC
*/

#define TRUE 1
#define FALSE 0
#define M_TO_NMI 1852.0

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define sign(x)  ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define absolute(x) ((x) > 0 ? (x) : -(x))

#include "user.h"
#include "simple_random.h"

#include <math.h>

MEASURE_GLOBAL_VARIABLES();

static double DMOD_l, TAU_l, ZTHR_l, HMD_l, ALIM_l;
static double tcas_result[2];

static void copy(double *a, double *b){
	a[0] = b[0];
	a[1] = b[1];
}

static double mult(double a[], double b[]) {
	return a[0]*b[0] + a[1]*b[1];
}

static void multInt(double a, double b[]) {
	tcas_result[0] = a*b[0];
	tcas_result[1] = a*b[1];
}

static void sum(double a[], double b[]) {
	tcas_result[0] = a[0] + b[0];
	tcas_result[1] = a[1] + b[1];
}

static void diff(double a[], double b[]) {
	tcas_result[0] = a[0] - b[0];
	tcas_result[1] = a[1] - b[1];
}

static void perpendicular(double *a) {
	double temp;
	temp = a[1];
	a[1] = -a[0];
	a[0] = temp;
}

static void initialize(double alt) {
	if(alt <= 2350) { 
		TAU_l = 15;
		DMOD_l = 0.2;
		ZTHR_l = 600;
		ALIM_l = 300;
		HMD_l = DMOD_l;
		//HMD_l = 1215;
		return;
	}
	else if(alt > 2350 && alt <= 5000) { 
		TAU_l = 20;
		DMOD_l = 0.35;
		ZTHR_l = 600;
		ALIM_l =  300;
		HMD_l = DMOD_l;
		//HMD_l = 2126;
		return;
	}
	else if(alt > 5000 && alt <= 10000) { 
		TAU_l = 25;
		DMOD_l = 0.55;
		ZTHR_l = 600;
		ALIM_l = 350;
		HMD_l = DMOD_l;
		//HMD_l =  3342;
		return;
	}
	else if(alt > 10000 && alt <= 20000) { 
		TAU_l = 30;
		DMOD_l = 0.8;
		ZTHR_l = 600;
		ALIM_l = 400;
		HMD_l = DMOD_l;
		//HMD_l = 4861;
		return;
	}
	else if(alt > 20000 && alt <= 42000) { 
		TAU_l = 35;
		DMOD_l = 1.1;
		ZTHR_l = 700;
		ALIM_l = 600;
		HMD_l = DMOD_l;
		//HMD_l = 6683;
		return;
	}
	else { 
		TAU_l = 35;
		DMOD_l = 1.1;
		ZTHR_l = 800;
		ALIM_l = 700;
		HMD_l = DMOD_l;
		//HMD_l = 6683;
		return;
	}
}

void initializeTraffic(double alt) {
	if(alt <= 1000) { 
		TAU_l = 20;
		DMOD_l = 0.3;
		ZTHR_l = 850;
		HMD_l = DMOD_l;
		return;
	}	
	else if(alt > 1000 && alt <= 2350) { 
		TAU_l = 25;
		DMOD_l = 0.33;
		ZTHR_l = 850;
		HMD_l = DMOD_l;
		return;
	}
	else if(alt > 2350 && alt <= 5000) { 
		TAU_l = 30;
		DMOD_l = 0.48;
		ZTHR_l = 850;
		HMD_l = DMOD_l;
		return;
	}
	else if(alt > 5000 && alt <= 10000) { 
		TAU_l = 40;
		DMOD_l = 0.75;
		ZTHR_l = 850;
		HMD_l = DMOD_l;
		return;
	}
	else if(alt > 10000 && alt <= 20000) { 
		TAU_l = 45;
		DMOD_l = 1;
		ZTHR_l = 850;
		HMD_l = DMOD_l;
		return;
	}
	else if(alt > 20000 && alt <= 42000) { 
		TAU_l = 48;
		DMOD_l = 1.3;
		ZTHR_l = 850;
		HMD_l = DMOD_l;
		return;
	}
	else { 
		TAU_l = 48;
		DMOD_l = 1.3;
		ZTHR_l = 1200;
		HMD_l = DMOD_l;
		return;
	}
}

double root(double a, double b, double c, double eps){
	if (a == 0) {
		return 0;
	}
	if (b*b - 4*a*c < 0) {
		return 0;
	}
	double res = (-b + eps*sqrt(b*b - 4*a*c))/2*a;
	return res;
}

double t_cpa(double* s, double* v) {
	return (- mult(s,v)/ mult(v,v));
}

double tau(double* s, double* v) {
	return (- mult(s,s)/mult(s,v));
}

double tau_modl (double* s, double* v) {
	return (DMOD_l*DMOD_l - mult(s,s)) / mult(s,v);
}

int Horizontal_RA_l(double* s, double* v) {
	if(mult(s,s) <= DMOD_l) {
		return TRUE;
	}
	if(mult(s,v) < 0) {
		if(tau_modl(s,v) <= TAU_l) {
			return TRUE;
		}
	}
	return FALSE;
}

double t_coa(double s, double v) {
	return - s/v;
}

double Vertical_RA_l (double s, double v) {
	if(absolute(s) <= ZTHR_l) {
		return TRUE;
	}
	if(s*v < 0) {
		if(t_coa(s,v) <= TAU_l) {
			return TRUE;
		}
	}
	return FALSE;
}

double delta(double* s, double* v, double D) {
	double x,y;
	x = D*D*mult(v,v);
	perpendicular(v);
	y = mult(s,v);
	return x - y*y;
}

double theta(double* s, double* v, double D, double eps) {
	return root(mult(v,v), 2*mult(s,v), mult(s,s)-D*D, eps);
}

double CD2D_inf(double* s, double* v, double D, double B) {
	if(mult(v,v) == 0) {
		if(mult(s,s) <= D) {
			return TRUE;
		}
	}
	if(mult(v,v) > 0) {
		if(delta(s,v,D) >= 0) {
			if(theta(s, v, D, 1) >= B) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

int TCASII_RA(double* so, double soz, double* vo, double voz, double* si, double siz, double* vi, double viz) {
	double s[2], v[2], sz, vz;
	diff(so, si);
	copy(s, tcas_result);
	diff(vo, vi);
	copy(v, tcas_result);
	sz = soz - siz;
	vz = voz - viz;
	if(Horizontal_RA_l(s,v)) {
		if(Vertical_RA_l(sz,vz)) {
			if(CD2D_inf(s,v,HMD_l,0)) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

int TCASII_RA_at(double* so, double soz, double* vo, double voz, double* si, double siz, double* vi, double viz, double t) {
	double s[2], v[2], sz, vz;
	diff(so, si);
	copy(s, tcas_result);
	diff(vo, vi);
	copy(v, tcas_result);
	sz = soz - siz;
	vz = voz - viz;
	double temp[2];
	multInt(t,v);
	copy(temp, tcas_result);
	sum(s, temp);
	copy(temp, tcas_result);
	if (Horizontal_RA_l(temp, v)){
		if(Vertical_RA_l(sz + t*vz, vz)) {
			if(CD2D_inf(s,v,HMD_l,t)){
				return TRUE;
			}
		}
	}
	return FALSE;
}

double tau_min(double* s, double* v){
	return 2*sqrt(-delta(s, v, DMOD_l))/mult(v,v);
}

double tstar(double t, double* s, double* v, double T, double B){
	double temp = t_cpa(s,v) - t/2;
	if (T < temp) {
		temp = T;
	}
	if(B > temp) {
		temp = B;
	}
	return temp;
}

double Time_Min_TAUmod_l(double* s, double* v, double B, double T){
	double temp[2];
	multInt(B,v);
	copy(temp, tcas_result);
	sum(s, temp);
	copy(temp, tcas_result);
	
	if(mult(temp, v) >= 0) {
		return B;
	}
	if(delta(s,v,DMOD_l) < 0) {
		return tstar(tau_min(s,v), s, v, T, B);
	}
	multInt(T, v);
	copy(temp, tcas_result);
	sum(s, temp);
	copy(temp, tcas_result);
	if(mult(temp, v) < 0) {
		return T;
	}
	return tstar(0, s, v, T, B);
}

int RA2D_l (double *s, double *v, double B, double T) {
	double temp[2];
	if(delta(s, v, DMOD_l) >= 0){
		multInt(B,v);
		copy(temp, tcas_result);
		sum(s, temp);
		copy(temp, tcas_result);
		if(mult(temp, v) < 0){
			multInt(T, v);
			copy(temp, tcas_result);
			sum(s, temp);
			copy(temp, tcas_result);
			if(mult(temp, v) >= 0) {
				return TRUE;
			}
		}
	}
	double t = Time_Min_TAUmod_l(s,v,B,T);
	multInt(t,v);
	copy(temp, tcas_result);
	sum(s, temp);
	copy(temp, tcas_result);
	if(Horizontal_RA_l(temp, v)) {
		return TRUE;
	}
	return FALSE;
}

double* RA2DTimeInterval_l (double *s, double *v, double B, double T){
	double a, b, c;
	a = mult(v,v);
	b = 2*mult(s,v) + TAU_l*mult(v,v);
	c = mult(s,s) + TAU_l*mult(s,v) - DMOD_l*DMOD_l;
	
	if(a==0) {
		if(mult(s,s) <= DMOD_l) {
			tcas_result[0] = B;
			tcas_result[1] = T;
			return tcas_result;
		}
	}
	double o = theta(s,v,DMOD_l,1);
	if(mult(s,s) <= DMOD_l) {
		tcas_result[0] = B;
		tcas_result[1] = o;
		return tcas_result;
	}
	if((mult(s,v) >= 0) && (b*b-4*a*c < 0)) {
		tcas_result[0] = T+1;
		tcas_result[1] = 0;
		return tcas_result;
	}
	tcas_result[0] = root(a,b,c,-1);
	if(delta(s,v,DMOD_l) >= 0) {
		tcas_result[1] = o;
	} else {
		tcas_result[1] = root(a,b,c,1);
	}
	return tcas_result;
}

double H(double vz) {
	return max(ZTHR_l, TAU_l*absolute(vz));
}

double* RAZTimeInterval_l(double sz, double vz, double B, double T) {
	if(vz == 0){
		tcas_result[0] = B;
		tcas_result[1] = T;
		return tcas_result;
	}
	tcas_result[0] = (-sign(vz)*H(vz)-sz)/vz;
	tcas_result[1] = (sign(vz)*ZTHR_l - sz)/vz;
	return tcas_result;
}

double* RA3DTimeInterval_l(double* so, double soz, double* vo, double voz, double* si, double siz, double* vi, double viz, double B, double T, int hmdf) {
	double temp[2];
	double s[2], v[2], sz, vz;
	diff(so, si);
	copy(s, tcas_result);
	diff(vo, vi);
	copy(v, tcas_result);
	sz = soz - siz;
	vz = voz - viz;
	if(hmdf) {
		if(!(CD2D_inf(s,v,HMD_l,B))) {
			tcas_result[0] = T;
			tcas_result[1] = B;
			return tcas_result;
		}
	}
	if (vz == 0) {
		if(absolute(sz) > ZTHR_l) {
			tcas_result[0] = T;
			tcas_result[1] = B;
			return tcas_result;
		}
	}
	double *t = RAZTimeInterval_l(sz, vz, B, T);
	if((t[1] < B) || (T < t[0])) {
		tcas_result[0] = T;
		tcas_result[1] = B;
		return tcas_result;
	}
	double t2[2];
	if(B > t[0]) {
		t2[0] = B;
	} else {
		t2[0] = t[0];
	}
	if(T < t[1]) {
		t2[1] = T;
	} else {
		t2[1] = t[1];
	}
	double *t3 = RA2DTimeInterval_l(s,v,B,T);
	double tnum[2];
	tnum[0] = max(t2[0], min(t2[1], t3[0]));
	tnum[1] = max(t2[0], min(t2[1], t3[1]));
	if((t3[0] > t3[1]) || (t3[1] < t2[0]) || t3[0] > t2[1]) {
		tcas_result[0] = T;
		tcas_result[1] = B;
		return tcas_result;
	}
	if(hmdf){
		if(HMD_l < DMOD_l) {
			multInt(t[0], v);
			copy(temp, tcas_result);
			sum(s,temp);
			copy(temp, tcas_result);
			if(mult(temp,v) >= 0) {
				multInt(t[0],v);
				copy(temp, tcas_result);
				sum(s, temp);
				copy(temp, tcas_result);
				if(mult(temp,temp) > HMD_l) {
					tcas_result[0] = T;
					tcas_result[1] = B;
					return tcas_result;
				}
			}
		}
	}
	double tnum3, tnum4;
	if(hmdf){
		if(HMD_l < DMOD_l) {
			double o = theta(s,v,HMD_l,1);
			if(mult(v,v) == 0) {
				tnum3 = T;
			} else {
				tnum3 = max(B, min(o, T));
			}
			tnum4 = min(tnum[1], tnum3);
			tcas_result[0] = tnum[0];
			tcas_result[1] = tnum4;
			return tcas_result;
		}
	}
	tcas_result[0] = tnum[0];
	tcas_result[1] = tnum[1];
	return tcas_result;
}

int RA3D(double* so, double soz, double* vo, double voz, double* si, double siz, double* vi, double viz, double B, double T) {
	double *t = RA3DTimeInterval_l(so, soz, vo, voz, si, siz, vi, viz, B, T, TRUE);
	if(t[0 <= t[1]]) {
		return TRUE;
	}
	return FALSE;
}

double stop_accel(double voz, double v, double a, double eps, double t){
	if((t <= 0) || eps*voz >= v) {
		return 0;
	}
	return (eps*v - voz) / (eps*a);
}

double own_alt_at(double soz, double voz, double v, double a, double eps, double t){
	double s = stop_accel(voz,v,a,eps,t);
	double q = min(t,s);
	double l = max(0, t-s);
	return eps*q*q*a/2 + q*voz + soz + eps*l*v;
}

double sep_at(double soz, double voz, double siz, double viz, double v, double a, double eps, double t) {
	double o = own_alt_at(soz, voz, absolute(v), a, eps*sign(v), t);
	double i = siz + t*viz;
	return eps*(o-i);
}

double RA_sense(double soz, double voz, double siz, double viz, double v, double a, double t) {
	double ou, od, i, u, d;
	ou = own_alt_at(soz, voz, v, a, 1, t);
	od = own_alt_at(soz, voz, v, a, -1, t);
	i = siz + t*viz;
	u = ou - i;
	d = i - od;
	if(sign(soz-siz) == 1) {
		if(u >= ALIM_l) {
			return 1;
		}
	}
	if(sign(soz-siz) == -1) {
		if(d >= ALIM_l) {
			return -1;
		}
	}
	if(u >= d) {
		return 1;
	}
	return -1;
}

int corrective(double *so, double soz, double *vo, double voz, double *si, double siz, double *vi, double viz, double v, double a) {
	double s[2], V[2], sz, vz, t, eps;
	diff(so,si);
	copy(s, tcas_result);
	diff(vo,vi);
	copy(V, tcas_result);
	sz = soz - siz;
	vz = voz - viz;
	t = tau_modl(s,V);
	eps = RA_sense(soz,voz,siz,viz,v,a,t);
	if(mult(s,s) < DMOD_l) {
		return TRUE;
	}
	if(mult(s,V) < 0) {
		if(eps*(sz + t*vz) < ALIM_l){
			return TRUE;
		}
	}
	return FALSE;
}

int TA3D(double *so, double soz, double *vo, double voz, double *si, double siz, double *vi, double viz, double B, double T) {
	double *t = RA3DTimeInterval_l(so, soz, vo, voz, si, siz, vi, viz, B, T, FALSE);
	if(t[0] < t[1]) {
		return TRUE;
	}
	return FALSE;
}

int tcas_bench(double *so, double soz, double *vo, double voz, double *si, double siz, double *vi, double viz) {
	initialize(soz);
	if(RA3D(so, soz, vo, voz, si, siz, vi, viz, 0, 1)) {
		double v = 1500; //fpm
		double a = 0.25 * 1930.44; // g = 1930.44 fpm/s		
		int corrLow = corrective (so, soz, vo, voz, si, siz, vi, viz, v, a);
		if(corrLow) {
			if(RA_sense(soz, voz, siz, viz, v, a, 0) == 1) {
				return 1; //CLIMB LOW SPEED
			} else {
				return 2; //DESCEND LOW SPEED
			}
		} else {
			double v = 2500; //fpm
			double a = 0.35 * 1930.44; // g = 1930.44 fpm/s
			if(RA_sense(soz, voz, siz, viz, v, a, 0) == 1) {
				return 3; //CLIMB HIGH SPEED
			} else {
				return 4; //DESCEND HIGH SPEED
			}				
		}
	} else {
		initializeTraffic(soz);
		if(TA3D(so, soz, vo, voz, si, siz, vi, viz, 0, 1)) {
			return 5; //TRAFFIC TRAFFIC
		}
	}
	return 0; //CLEAR
}

void tcas(int seed){

    random_set_seed(seed);
    

	double so[2], vo[2], si[2], vi[2];
	so[0] = random_get()*10;
	so[1] = random_get()*10;
	vo[0] = random_get();
	vo[1] = random_get();
	si[0] = random_get()*10;
	si[1] = random_get()*10;
	vi[0] = random_get();
	vi[1] = random_get();

    double my_alt = random_get()*10000;
    double my_vs  = (random_get()-0.5)*3000;
    double int_alt = random_get()*10000;
    double int_vs  = (random_get()-0.5)*3000;

    MEASURE_START();
    for(int i=0; i<ITERATIONS;i++){
        tcas_bench(so, my_alt, vo, my_vs, si, int_alt, vi, int_vs);
    }
    MEASURE_STOP();
}

/*
int tcas_bench(double my_lat, double my_lon, double my_alt, double my_vs, double my_vx, double my_vy, double int_lat, double int_lon, double int_alt, double int_vs, double int_vx, double int_vy, void (*start)(), void (*stop)()) {
	//FILE *fp = fopen("tcas_result.txt", "w");
	my_lat = my_lat / M_TO_NMI;
	my_lon = my_lon / M_TO_NMI;
	my_vx = my_vx / M_TO_NMI;
	my_vy = my_vy / M_TO_NMI;
	int_lat = int_lat / M_TO_NMI;
	int_lon = int_lon / M_TO_NMI;
	int_vx = int_vx / M_TO_NMI;
	int_vy = int_vy / M_TO_NMI;

	double so[2], vo[2], si[2], vi[2];
	so[0] = my_lat;
	so[1] = my_lon;
	vo[0] = my_vx;
	vo[1] = my_vy;
	si[0] = int_lat;
	si[1] = int_lon;
	vi[0] = int_vx;
	vi[1] = int_vy;

	start();
	int output = tcas(so, my_alt, vo, my_vs, si, int_alt, vi, int_vs);
	stop();

	return output;
}*/
