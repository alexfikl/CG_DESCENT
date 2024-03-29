/* When the line search routine tries to bracket a local minimizer in
   the search direction, it may expand the initial line search interval.
   The default expansion factor is 5. You can modify this factor using
   the parameter rho.  In the following example, we choose a small initial
   stepsize (initial step is 1.e-5), QuadStep is FALSE, and rho is 1.5.
   The code has to do a number of expansions to reach a suitable
   interval bracketing the minimizer in the initial search direction.

   Termination status: 0
   Convergence tolerance for gradient satisfied
   maximum norm for gradient:  6.283573e-09
   function value:            -6.530787e+02

   cg  iterations:                  31
   function evaluations:            55
   gradient evaluations:            86
   ===================================

   We then restore the default value of rho to obtain:

   Termination status: 0
   Convergence tolerance for gradient satisfied
   maximum norm for gradient:  9.605754e-09
   function value:            -6.530787e+02

   cg  iterations:                  31
   function evaluations:            38
   gradient evaluations:            69 */

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

int main (void)
{
    double *x ;
    INT i, n ;
    cg_parameter Parm ;


    /* allocate space for solution */
    n = 100 ;
    x = (double *) malloc (n*sizeof (double)) ;

    /* starting guess */
    for (i = 0; i < n; i++) x [i] = 1. ;

    /* initialize default parameter values */
    cg_default (&Parm) ;

    /* set parameter values */
    Parm.step = 1.e-5 ;
    Parm.QuadStep = FALSE ;
    Parm.rho = 1.5 ;

    /* solve the problem */
    cg_descent(x, n, NULL, &Parm, 1.e-8, myvalue, mygrad, myvalgrad, NULL, NULL, NULL) ;

    /* starting guess */
    for (i = 0; i < n; i++) x [i] = 1. ;

    /* set rho = 5. */
    Parm.rho = 5. ;

    /* solve the problem */
    cg_descent(x, n, NULL, &Parm, 1.e-8, myvalue, mygrad, myvalgrad, NULL, NULL, NULL) ;

/* free work space */
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
