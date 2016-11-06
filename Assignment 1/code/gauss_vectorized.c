/* row wise gaus algorithm
 * pattern for practical course
 * -------------------------
 * autor: Markus Brenk
 * date: 2002-09-25
 * =================================================== */

//#define NGLS 32768
#define NGLS 32800000

#define NUM_LOOPS 4

#include <stdio.h>
#include <math.h>
#include "timer.h"


/* print a 3x3 matrix */
void print_matrix(char* name, double matrix[3][3]);


/* print a 3d vector */
void print_vector(char* name, double vec[3][NGLS], int const g);

/**
 *  initialisation: generates the following LGS:
 *  ( 3  1  1)       (5)                         (1)
 *  ( 1  4  1) * X = (6)        => solution  X = (1)
 *  ( 1  1  5)       (7)                         (1)
 */
void initA(double a[3][3]);
void initVec(double b[3][NGLS], double x[3][NGLS]);

/* LU decomposition, in-place. */
void luDecompose(double a[3][3]);

/* Substitutions */
void forwardSubstitute(double a[3][3], double b[3][NGLS], double x[3][NGLS]);
void backwardSubstitute(double a[3][3], double b[3][NGLS], double x[3][NGLS]);


__declspec(align(32)) double a[3][3];
__declspec(align(32)) double b[3][NGLS];
__declspec(align(32)) double x[3][NGLS];

int main() {

	int i, j;
	int n=3;

	time_marker_t time;

	initA(a);
	initVec(b, x);

	time = get_time();

	luDecompose(a);
	forwardSubstitute(a, b, x);
	backwardSubstitute(a, x, x); // We declared all arguments as "restrict". However we know its safe to overlap x here.

	printf("NAIV: Time elapsed. time: %f   ticks: %f\n", get_ToD_diff_time(time), get_ticks_diff_time(time));

	print_vector("x", x, 0);

	return(0);
}



void initVec(double b[3][NGLS], double x[3][NGLS]) {
	int i, g;
	int n = 3;

	for(i=0; i<n; ++i) {
		for (g=0;g<NGLS;g++)
		{
			b[i][g]=(float)(2*n-2)+(float)(i+1);
			x[i][g]=0.;
		}
	}
}

void initA(double a[3][3]) {
	int i,j;
	int n = 3;

	for (j=0;j<n;j++)
	{
		a[j][j]=(float)(n-1)+(float)(j+1);

		for (i=j+1;i<n;i++) {
			a[i][j]=1.;
		}
		for (i=0;i<j;i++) {
			a[i][j]=1.;
		}
	}
}

void luDecompose(double a[3][3]) {
	int n = 3;
	int i,j,k;

	for(i = 0; i < n-1; ++i) {
		for(k = i+1; k < n; ++k) {
			// Compute L.
			a[k][i] /= a[i][i];

			// Compute R.
			for(j = i+1; j < n; ++j) {
				a[k][j] -= a[k][i] * a[i][j];
			}
		}
	}
}

void forwardSubstitute(double a[restrict][3], double b[restrict][NGLS], double x[restrict][NGLS]) {
	int n = 3;
	int i,j,g;

	for(i = 0; i < n; ++i){
		for(g = 0; g < NGLS; ++g){
			x[i][g] = b[i][g];
		}

		for(j = 0; j < i; ++j){
			for(g = 0; g < NGLS; ++g){
				x[i][g] -= a[i][j] * x[j][g];
			}
		}
	}
}

void backwardSubstitute(double a[restrict][3], double b[restrict][NGLS], double x[restrict][NGLS]) {
	int n = 3;
	int i,j,g;

	for(i = n-1; i >= 0; --i){
		for(g = 0; g < NGLS; ++g) {
			x[i][g] = b[i][g];
		}

		for(j = i+1; j < n; ++j) {
			for(g = 0; g < NGLS; ++g) {
				x[i][g] -= a[i][j] * x[j][g];
			}
		}
		for(g = 0; g < NGLS; ++g) {
			x[i][g] /= a[i][i];
		}
	}
}

void print_matrix(char* name, double matrix[3][3]) {
	int i, j;
	printf("Matrix %s: \n", name);
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf(" %f ", matrix[i][j]);
		}
		printf(" ;\n");
	}
}

void print_vector(char* name, double vec[3][NGLS], int const g) {
	int i;
	printf("vector %s: \n", name);
	for (i=0;i<3;i++)
	{
		printf(" %f \n",vec[i][g]);
	}
}

