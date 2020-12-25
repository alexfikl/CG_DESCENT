/* This is the example appearing in the User's Guide with default
   parameter values.  The output on a linux workstation was the following:



   Termination status: 0
   Convergence tolerance for gradient satisfied
   maximum norm for gradient:  6.269565e-09
   function value:            -6.530787e+02

   cg  iterations:                  32
   function evaluations:            54
   gradient evaluations:            46 */

#include <math.h>
#include "cg_user.h"

double myvalue
(
    double   *x,
    INT       n,
    void     *User
) ;

void mygrad
(
    double    *g,
    double    *x,
    INT        n,
    void      *User
) ;

double myvalgrad
(
    double    *g,
    double    *x,
    INT        n,
    void      *User
) ;

int mycallback
(
    cg_iter_stats *IterStats,
    void               *User
) ;

int main (void)
{
    double *x ;
    INT i, n ;
    cg_parameter Parm ;

    /* allocate space for solution */
    n = 100 ;
    x = (double *) malloc (n*sizeof (double)) ;

    /* set starting guess */
    for (i = 0; i < n; i++) x [i] = 1. ;

    /* set default parameter values */
    cg_default (&Parm) ;
    Parm.PrintLevel = 1 ;

    /* run the code */
    cg_descent (x, n, NULL, &Parm, 1.e-8, myvalue,
            mygrad, myvalgrad, mycallback, NULL, NULL) ;

    /* with some loss of efficiency, you could omit the valgrad routine */
    for (i = 0; i < n; i++) x [i] = 1. ; /* starting guess */
    cg_descent (x, n, NULL, NULL, 1.e-8, myvalue, mygrad, NULL, NULL, NULL, NULL) ;

    free (x) ;
}

double myvalue
(
    double   *x,
    INT       n,
    void     *User
)
{
    double f, t ;
    INT i ;
    f = 0. ;
    for (i = 0; i < n; i++)
    {
        t = i+1 ;
        t = sqrt (t) ;
        f += exp (x [i]) - t*x [i] ;
    }
    return (f) ;
}

void mygrad
(
    double    *g,
    double    *x,
    INT        n,
    void      *User
)
{
    double t ;
    INT i ;
    for (i = 0; i < n; i++)
    {
        t = i + 1 ;
        t = sqrt (t) ;
        g [i] = exp (x [i]) -  t ;
    }
    return ;
}

double myvalgrad
(
    double    *g,
    double    *x,
    INT        n,
    void      *User
)
{
    double ex, f, t ;
    INT i ;
    f = (double) 0 ;
    for (i = 0; i < n; i++)
    {
        t = i + 1 ;
        t = sqrt (t) ;
        ex = exp (x [i]) ;
        f += ex - t*x [i] ;
        g [i] = ex -  t ;
    }
    return (f) ;
}

int mycallback
(
    cg_iter_stats *IterStats,
    void               *User
)
{
    double *g = IterStats->g;
    double *d = IterStats->d;

    double gnorm = fabs(g[0]);
    double dnorm = fabs(d[0]);

    for (INT i = 1; i < IterStats->n; ++i) {
        gnorm = g[i] > gnorm ? g[i] : gnorm;
        dnorm = d[i] > dnorm ? d[i] : dnorm;
    }

    printf("iter %ld alpha %.15e func %.15e gnorm %.15e dnorm %.15e\n",
            IterStats->iter, IterStats->alpha, IterStats->f, gnorm, dnorm);

    return 1;
}
