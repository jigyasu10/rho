/* ansari.c
   Compute the exact distribution of the Ansari-Bradley test statistic.
   */

#include <R.h>
#include <Rmath.h> /* uses choose() */

#include "ctest.h"


/*
  Removed the non-local variable `double ***w'
  and moved to R_alloc from Calloc. No need for
  w_free() since the .C() calls will clear it.
  The tests for whether the memory was allocated 
  can be discarded as R_alloc will throw an error.
  The .C() will handle the vmaxget() and vmaxset().
 */
static void
errmsg(char *s)
{
    PROBLEM "%s", s RECOVER(NULL_ENTRY);
}

static double ***
w_init(Sint m, Sint n)
{
    Sint i;
    double ***w;
    
    w = (double ***) R_alloc(m + 1, sizeof(double **));
    memset(w, '\0', (m+1)*sizeof(double**));
    for (i = 0; i <= m; i++) {
	w[i] = (double**) R_alloc(n + 1, sizeof(double *));
	memset(w[i], '\0', (n+1)*sizeof(double*));
    }
    return(w);
}


#if 0
/* 
  This is not needed if we use R_alloc() and let R 
  garbage collect.
 */
static void
w_free(Sint m, Sint n, double ***w)
{
    Sint i, j;
    for (i = m; i >= 0; i--) {
	for (j = n; j >= 0; j--) {
	    Free(w[i][j]);
        }
	Free(w[i]);
    }
    Free(w);
    w = 0;
}
#endif 

static double
cansari(int k, int m, int n, double ***w)
{
    int i, l, u;

    l = (m + 1) * (m + 1) / 4;
    u = l + m * n / 2;
    
    if ((k < l) || (k > u))
	return(0);

    if (w[m][n] == 0) {
	w[m][n] = (double *) R_alloc(u + 1, sizeof(double));
	memset(w[m][n], '\0', (u + 1)*sizeof(double));
	for (i = 0; i <= u; i++)
	    w[m][n][i] = -1;
    }

    if (w[m][n][k] < 0) {
	if (m == 0)
	    w[m][n][k] = (k == 0);
	else if (n == 0)
	    w[m][n][k] = (k == l);
	else
	    w[m][n][k] = cansari(k, m, n - 1, w)
		+ cansari(k - (m + n + 1) / 2, m - 1, n, w);
    }

    return(w[m][n][k]);
}


#if 0
/*
  Is this ever called?
 */
void
dansari(Sint *len, double *x, Sint *m, Sint *n)
{
    Sint i;
    double ***w;    

    w = w_init(*m, *n);
    for (i = 0; i < *len; i++)
	if (fabs(x[i] - floor(x[i] + 0.5)) > 1e-7) {
	    x[i] = 0;
	} else {
	    x[i] = cansari((Sint)x[i], (Sint)*m, (Sint)*n, w)
		/ choose(*m + *n, *m);
	}
    /* w_free(*m, *n, w); */
}
#endif


void
pansari(Sint *len, double *x, Sint *m, Sint *n)
{
    Sint i, j, l, u;
    double c, p, q;
    double ***w;

    w = w_init(*m, *n);
    l = (*m + 1) * (*m + 1) / 4;
    u = l + *m * *n / 2;
    c = choose(*m + *n, *m);
    for (i = 0; i < *len; i++) {
	q = floor(x[i] + 1e-7);
	if (q < l)
	    x[i] = 0;
	else if (q > u)
	    x[i] = 1;
	else {
	    p = 0;
	    for (j = l; j <= q; j++) {
		p += cansari((Sint)j, (Sint)*m, (Sint)*n, w);
	    }
	    x[i] = p / c;
	}
    }
    /* w_free(*m, *n, w); */
}

void
qansari(Sint *len, double *x, Sint *m, Sint *n)
{
    Sint i, l, u;
    double c, p, q, xi;
    double ***w;

    w = w_init(*m, *n);
    l = (*m + 1) * (*m + 1) / 4;
    u = l + *m * *n / 2;
    c = choose(*m + *n, *m);    
    for (i = 0; i < *len; i++) {
	xi = x[i];
        if(xi < 0 || xi > 1)
	    errmsg("probabilities outside [0,1] in qansari()");
	if(xi == 0)
	    x[i] = l;
	else if(xi == 1)
	    x[i] = u;
	else {
	    p = 0;
	    q = 0;
	    for(;;) {
		p += cansari(q, (Sint)*m, (Sint)*n, w) / c;
		if (p >= xi)
		    break;
		q++;
	    }
	    x[i] = q;
	}
    }
    /* w_free(*m, *n, w); */
}
