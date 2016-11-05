/* vectorisation quiz
 * for practical course
 * ----------------------
 * autor: Markus Brenk
 * date: 2003-10-21
 * author: Sebastian Rettenberger
 * date: 2015-10-09
 * ================================================== */


#include <stdio.h>
#include <math.h>

#define dim1 100
#define dim2 200

static int bsp1_(double *, double *, double *, int);
static int bsp2_(double *, double *, double *, int);
static int bsp3_(double *, int, int);
static int bsp4_(double *, int , int );
static int bsp5_(double *, double *, int, int, int, int);
static int bsp6_(double *, double *, double *, int);
static int bsp7_(double *, double *, double *, int *, int);
static int bsp8_(double *, double *, double *, double *, int *, int, int);
static int bsp9_(double *, double *, double *, int *, int *, int);
static int bsp10_(double *, double *, int);
static int bsp11_(double a[][dim1], int);
static int bsp12_(double a[][dim1], double b[][dim1], int);
static int bsp13_(double*,double*,double*,double a[][dim1],double b[][dim1],double*,double*,double*,int);
static int bsp14_(double*,double*,double*,double a[][dim1],double b[][dim1],double*,double*,double*,int);
static int bsp15_(double a[][dim1], double b[][dim1], double c[][dim1], int);
static int bsp16_(double *, double *, double *, int *, int);
static int bsp17_(double *, double *, int);
static int bsp18_(double *, double *, int);
static int bsp19_(double *, double *, int);

static inline double f_(double);

/* ---------------------------------------------------------------------- *\
 *  The functions bsp1_() up to bsp19_() contain loops.                   *
 *  Is it possible to optimize the different loops with SSE2 pipelining?  *
 *  What do you expect?                                                   *
 *  What does the compiler do?                                            *
 *  Compile on an intel architecture with the intel compiler              *
 *  Use the following commands:                                           *
 *  > icc -std=c99 -restrict -O3 -ip -ip_no_inlining\                     *
 *        -qopt-report=3 -qopt-report-phase=vec\                          *
 *        -opt_report_filevector.opt -opt_report_phaseall\                *
 *        vector.c                                                        *
 *  What is the difference?                                               *
 *  Can you help the compiler with pragmas or restructuring the code?     *
\* ---------------------------------------------------------------------- */


/* Main program */
int main() {
  int n1 = dim1;
  int n2 = dim2;
  int i, j, l, m;
  __declspec(align(16)) int ix[dim1], iy[dim1];
  __declspec(align(16)) double a[dim1][dim1], b[dim1][dim1],c[dim1][dim1],e[dim1];
  __declspec(align(16)) double x[dim2], y[dim2], z[dim2], s1, s2, s3;
  double result = 0; // Sum up everything

  /* initialisation */

  for (i = 0; i < n2; i++) {
    x[i] = 1.;
    y[i] = 2.;
    z[i] = 3.;
  }

  for (i=0; i < n1; i++) {
    ix[i] = n1 - i;
    iy[i] = (int)(i - i * .9);
  }

  for (i=0; i < n1; i++) {
    for (j=0; j < n1; j++) {
      a[i][j] = 1.;
      b[i][j] = 2.;
      c[i][j] = 3.;
    }
  }
  s1 = 1.;
  s2 = 2.;
  s3 = 3.;



  bsp1_(x, y, e, n1);
  for (i = 0; i < n1; i++) {
    result += e[i];
  }
  bsp2_(x, y, e, n1);
  for (i = 0; i < n1; i++) {
    result += e[i];
  }
  bsp3_(x, n1, n2);
  for (i = 0; i < n1; i++) {
    result += x[i];
  }
  bsp4_(x, n1, n2);
  for (i = 0; i < n2; i++) {
    result += x[i];
  }
  l = 1;
  m = 2;
  /* -- or -- */
  /* l = 2;
     m = 1; */
  bsp5_(y, z, n1, n2, l, m);
  for (i = 0; i < n1; i++) {
    result += z[i];
  }

  for (i=0; i < n2; i++) {
    z[i] = 3.;
  }
  bsp6_(x, y, z, n1);
  for (i = 0; i < n1; i++) {
    result += x[i] + y[i];
  }

  for (i=0; i<n2; i++) {
    x[i] = 1.;
    y[i] = 2.;
  }
  bsp7_(x, y, e, ix, n1);
  for (i = 0; i < n1; i++) {
    result += e[i];
  }
  bsp8_(x, y, z, e, iy, n1, n2);
  for (i = 0; i < n1; i++) {
    result += x[i] + e[i];
  }

  for (i=0; i<n2; i++) {
    x[i] = 1.;
  }
  bsp9_(x, y, z, ix, iy, n1);
  for (i = 0; i < n1; i++) {
    result += x[i] + y[i] + z[i];
  }

  for (i=0; i<n2; i++) {
    x[i] = 1.;
    y[i] = 2.;
    z[i] = 2.;
  }
  bsp10_(x, e, n1);
  for (i = 0; i < n1; i++) {
    result += e[i];
  }
  bsp11_(a, n1);
  for (i = 0; i < n1; i++)
    for (j = 0; j < n1; j++) {
      result += a[i][j];
    }

  for (i=0; i<n1; i++) {
    for (j=0; j<n1; j++) {
      a[i][j] = 1.;
    }
  }
  bsp12_(a, b, n1);
  for (i = 0; i < n1; i++)
    for (j = 0; j < n1; j++) {
      result += a[i][j];
    }
  for (i = 0; i < n1; i++) {
    result += x[i];
  }

  for (i=0; i<n1; i++) {
    for (j=0; j<n1; j++) {
      a[i][j] = 1.;
    }
  }
  bsp13_(x, y, z, a, b, &s1, &s2, &s3, n1);
  for (i = 0; i < n1; i++)
    for (j = 0; j < n1; j++) {
      result += a[i][j];
    }

  for (i=0; i <n1; i++) {
    x[i] = 1.;
    z[i] = 3.;
    for (j=0; j<n1; j++) {
      a[i][j] = 1.;
    }
  }
  bsp14_(x, y, z, a, b, &s1, &s2, &s3, n1);
  for (i = 0; i < n1; i++)
    for (j = 0; j < n1; j++) {
      result += a[i][j];
    }

  for (i=0; i<n1; i++) {
    x[i] = 1.;
    z[i] = 3.;
    for (j=0; j <n1; j++) {
      a[i][j] = 1.;
    }
  }
  bsp15_(a, b, c, n1);
  for (i = 0; i < n1; i++)
    for (j = 0; j < n1; j++) {
      result += a[i][j];
    }
  bsp16_(x, y, z, ix, n1);
  for (i = 0; i < n1; i++) {
    result += x[i];
  }
  bsp17_(x, &s1, n1);
  result += s1;

  for (i=0; i<50; i++) {
    x[i] = 1.;
  }
  bsp18_(x, y, n1);
  for (i = 0; i < n1; i++) {
    result += y[i];
  }
  bsp19_(x, y, n1);
  for (i = 0; i < n1; i++) {
    result += x[i];
  }


  return result;
} /* end of main */



int bsp1_(double * restrict x, double * restrict y, double* restrict e, int n) {
  int i;

  for (i=0; i< n; i++) {
    if (x[i] > 0.) {
      e[i] = x[i] + y[i];
    } else {
      e[i] = x[i] - y[i];
    }
  }
  return 0;
} /* bsp1_ */



int bsp2_(double * restrict x, double * restrict y, double * restrict e, int n) {
  int i;
  double r1;

  i = 0;
L10:
  /* Computing 3rd power */
  r1 = x[i] - y[i];
  e[i] = r1 * (r1 * r1);
  i++;
  if (i < n) {
    goto L10;
  }
  return 0;
} /* bsp2_ */



int bsp3_(double * restrict x, int n, int nn) {
  int i, l;

  l = 65;
  for (i=0; i < n; i++) {
    x[i] = x[i + l];
  }
  return 0;
} /* bsp3_ */



int bsp4_(double * restrict x, int n, int nn) {
  int i, l;

  l = -100;
  for (i = n ; i < nn; i++) {
    x[i] = x[i + l];
  }
  return 0;
} /* bsp4_ */



int bsp5_(double * restrict y, double * restrict z, int n, int nn, int l, int m) {
  int i;

  for (i=0; i< n; i++) {
    z[i + l] = z[i + m] + y[i];
  }
  return 0;
} /* bsp5_ */



int bsp6_(double * restrict x, double * restrict y, double * restrict z, int n) {
  int i;

  for (i = 1; i < n; i++) {
    x[i] = y[i - 1];
    y[i] = z[i];
  }
  return 0;
} /* bsp6_ */



int bsp7_(double * restrict x, double * restrict y, double * restrict e, int * restrict ix, int n) {
  int i;

  for (i=0; i < n; i++) {

    e[i] = x[i] * y[i];
  }
  return 0;
} /* bsp7_ */



int bsp8_(double * restrict x, double * restrict y, double * restrict z,
          double * restrict e, int * restrict iy, int n, int nn) {
  int i;

  for (i=0; i < n; i++) {
    x[i] = x[i * 2] + z[i + i];
    e[i] = y[i] + y[iy[i] * iy[i]];
  }
  return 0;
} /* bsp8_ */



int bsp9_(double * restrict x, double * restrict y, double * restrict z,
          int * restrict ix, int * restrict iy, int n) {
  int i;

  for (i=0; i < n; i++) {
    x[i] = y[iy[i]] * 2.5;
    z[i] = y[ix[i]] * 3.;
    y[ix[i]] = 0.;
  }
  return 0;
} /* bsp9_ */



int bsp10_(double * restrict x, double * restrict e, int n) {
  int i;

  for (i=0; i < n; i++) {
    e[i] = f_(x[i]);
  }
  return 0;
} /* bsp10_ */

inline double f_(double x) {
  double ret_val, r1;

  r1 = sin(x);
  ret_val = x - r1 * r1;
  return ret_val;
} /* f_ */



int bsp11_(double a[restrict][dim1], int n) {
  int i, j, ip1;

  for (i=0; i < n; i++) {
    ip1 = i + 1;
    for (j = ip1; j < n; j++) {
      a[i][j] /= a[i][i];
    }
  }

  /* for (j=0; j < n; j++) {
     jp1 = j + 1;
     for (i = jp1; i < n; i++) {
        a[i][j] /= a[j][j];
     }
     }*/
  return 0;
} /* bsp11_ */



int bsp12_(double a[restrict][dim1], double b[restrict][dim1], int n) {
  int i, j;

  for (i=0; i < n-1; i++) {
    for (j=0; j < n; j++) {
      a[i+1][j] = a[i][j] * b[i][j];
    }
  }
  return 0;
} /* bsp12_ */




int bsp13_(double * restrict x, double * restrict y, double * restrict z,
           double a[restrict dim1][dim1], double b[restrict][dim1],
           double * restrict s1, double * restrict s2, double * restrict s3, int n) {
  int i, j, k;

  for (i=0; i < n; i++) {
    z[i] = (x[i] - y[i]) / *s1;
    x[i] = z[i] * *s2;
    for (k=0; k < n; k++) {
      a[i][k] = b[i][k] + x[i];
    }
    for (j=0; j < n; j++) {
      a[i][j] = a[i][j] * x[i] - *s3;
    }
  }
  return 0;
} /* bsp13_ */



int bsp14_(double * restrict x, double * restrict y, double * restrict z,
           double a[restrict][dim1], double b[restrict][dim1],
           double * restrict s1, double * restrict s2, double * restrict s3, int n) {
  int i, j, k;

  for (i=0; i < n; i++) {
    z[i] = (x[i] - y[i]) / *s1;
    x[i] = z[i] * *s2;
    for (k=0; k < n; k++) {
      a[i][k] = b[i][k] + x[i];
    }
    for (j=0; j < n; j++) {
      a[i][j] = a[i][j] * x[j] - *s3;
    }
  }
  return 0;
} /* bsp14_ */



int bsp15_(double a[restrict][dim1], double b[restrict][dim1],
           double c[restrict][dim1], int n) {
  int i, j;
  int l=3, m=2;

  /* ------------------------------------------ *
   * In which case is LOOP COLLAPSING possible? *
   * case 1: l=i and m=j   case 2: l=j and m=i  *           I
   * ------------------------------------------ */

  for (i=0; i < n; i++) {
    /* m = j; */
    /* m = i; */
    for (j=0; j < n; j++) {
      /* l = i; */
      /* l = j; */
      a[i][j] = b[i][j] + c[i][j] * b[l][m];
    }
  }
  return 0;
} /* bsp15_ */



int bsp16_(double * restrict x, double * restrict y, double * restrict z,
           int * restrict ix, int n) {

  int i;

  for (i = 0; i < n; i++) {
    x[i] = y[i] + z[i];
    x[ix[i]] = 1.;
  }
  return 0;
} /* bsp16_ */



int bsp17_(double * restrict x, double * restrict s, int n) {

  int i;


  *s = 0.;
  for (i = 0; i < n; i++) {
    *s += x[i];
  }
  return 0;
} /* bsp17_ */



int bsp18_(double * restrict x, double * restrict y, int n) {
  int i;

  for (i = 0; i < n; i++) {
    y[i] = 1.;
    if (x[i] < 0.) {
      break;
    }
  }
  return 0;
} /* bsp18_ */


int bsp19_(double * restrict x, double * restrict y, int n) {
  int i;

  for (i = 0; i < n; i++) {
    if (x[i] < 0.) {
      break;
    }
    x[i+1] -=  y[i];
  }
  return 0;
} /* bsp19_ */

