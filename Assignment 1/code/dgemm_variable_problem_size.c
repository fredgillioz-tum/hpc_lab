/** 
 * matrix matrix multiplication pattern for practical course
 **/

#include "timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int max(int a, int b){
	if(a >= b) return a;
	else return b;
}

int min(int a, int b){
	if(a <= b) return a;
	else return b;
}

int main(int argc, char **argv)
{
	int n;
	double *a, *b, *c;

	int mem_size;

	int i, j, k;

	char logfile_name[100];
	FILE *logfile_handle;

	n = 500;
	if(argc > 1){
		n = atoi(argv[1]);
	}

	int silent = 0;
	if(argc > 2){
		silent = (strcmp("silent", argv[2]) == 0);
	}

	sprintf(logfile_name, "logfile_dgemm.txt");
	if(!silent){
		logfile_handle = freopen(logfile_name, "a", stdout);
	}

	mem_size = n * n * sizeof(double);
	a = (double*)malloc(mem_size);
	b = (double*)malloc(mem_size);
	c = (double*)malloc(mem_size);
	if(0 == a || 0 == b || 0 == c){
		if(!silent) printf("memory allocation failed");
		return 0;
	}

	/* initialisation */
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			*(a + i * n + j) = (double)i + (double)j;
			*(b + i * n + j) = (double)(n - i) + (double)(n - j);
		}
	}
	memset(c, 0, mem_size);

	if(!silent) printf("%d\t", n);

	double const flops = 2.0 * n * n * n;
	time_marker_t time = get_time();

	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			for(k = 0; k < n; k++){
				c[i * n + j] += a[i * n + k] * b[k * n + j];
			}
		}
	}

	if(!silent) printf("%f\t", (double) flops / get_ticks_diff_time(time));

	//print_flops(flops, time);

	if(!silent) fclose(logfile_handle);
	logfile_handle = 0;

	return(0);
}

