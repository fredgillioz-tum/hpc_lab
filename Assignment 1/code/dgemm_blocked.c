/** 
 * matrix matrix multiplication pattern for practical course
 **/

#include "timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define mymax(a, b) ((a) > (b) ? (a) : (b))
#define mymin(a, b) ((a) > (b) ? (b) : (a))

inline int max(int a, int b){
	if(a >= b) return a;
	else return b;
}

inline int min(int a, int b){
	if(a <= b) return a;
	else return b;
}

int main(int argc, char **argv)
{
	int n;
	double *a, *b, *c;
	__declspec(align(32)) double s;

	int mem_size;

	int i, j, k, ii, jj, kk;

	char logfile_name[100];
	FILE *logfile_handle;

	int silent = 0;
	if(argc > 2){
		silent = (strcmp("silent", argv[2]) == 0);
	}

	n = 512;
	if(argc > 1){
		n = atoi(argv[1]);
	}
	if((n % 4) != 0){
		if(!silent) printf("matrix size must be a multiple of 4");
		return 0;
	}

    int blockSize = 16;
    if(argc > 3){
        blockSize = atoi(argv[3]);
    }
	if((blockSize % 4) != 0){
		if(!silent) printf("blocksize must be a multiple of 4");
		return 0;
	}
    int const numBlocks = ((n-1)/blockSize) + 1;
    int const nn = blockSize*numBlocks;

	sprintf(logfile_name, "logfile_dgemm.txt");
	if(!silent){
		logfile_handle = freopen(logfile_name, "a", stdout);
	}

	mem_size = n * n * sizeof(double);
	a = (double*)memalign(32, mem_size);
	b = (double*)memalign(32, mem_size);
	c = (double*)memalign(32, mem_size);
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
	if(!silent) printf("%d\t", blockSize);

	double const flops = 2.0 * n * n * n;
	time_marker_t time = get_time();

	for(ii=0; ii<n; ii+=blockSize){
		for(jj=0; jj<n; jj+=blockSize){
			for(kk=0; kk<n; kk+=blockSize){
				for(i=0; i<blockSize && ii+i<n; ++i){
					for(j=0; j<blockSize && jj+j<n; ++j){
						int const kStop = mymin(blockSize, n-kk);
						s = c[(ii + i)*n + jj + j];
#pragma vector aligned
						for(k=0; k<kStop; ++k){
							s += a[(ii + i)*n + kk + k] * b[(kk + k)*n + jj + j];
						}
						c[(ii + i)*n + jj + j] = s;
					}
				}
			}
		}
	}

	if(!silent) printf("%f\t", (double) flops / get_ticks_diff_time(time));

	//print_flops(flops, time);

	if(!silent) fclose(logfile_handle);
	logfile_handle = 0;

	return(0);
}
