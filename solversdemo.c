#include <stdio.h>
#include <stdbool.h>
#include <complex.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include "matops.h"
#include "solvers.h"

int main(){

  //a resource for checking http://math.fullerton.edu/mathews/numerical.html


  {//test gaussian
  double rowmjrarrayam[]={  2, 1,-1, 8
 			  ,-3,-1, 2,-11
 			  ,-2, 1, 2,-3};
  structmatrix am=creatematrix(rowmjrarrayam
 			      ,3,4
 			      ,endt(MATDOUBLE),rowmjr);

 //test gauss
  gaussop(&am);
  structmatrix solns=backsolve(&am);
  //printmat(&am);
  printf("gaussian elimination solution\n");
  printmat(&solns);
  }



  {//test gauss-seidel and successive overrelaxation
    double rowmjrarraya[]={   7, -2,1,2
 			      ,2,8, 3,1
 			      ,-1, 0, 5,2
 			      ,0,2,-1,4};
    structmatrix a=creatematrix(rowmjrarraya
 				,4,4
 				,endt(MATDOUBLE),rowmjr);
    double rowmjrarrayb[]={ 3
 			    ,-2
 			    ,5
 			    ,4};
    structmatrix b=creatematrix(rowmjrarrayb
 				,4,1
 				,endt(MATDOUBLE),rowmjr);

    structmatrix XJ=jacobi(&a,&b);
    printf("jacobian solution\n");
    printmat(&XJ);
    structmatrix XG=gaussseidel(&a,&b);
    printf("gaussian-seidel solution\n");
    printmat(&XG);
    printf("successive overrelaxation solution\n");
    structmatrix XS=soverrelaxation(&a,&b);
    printmat(&XS);
  }

  {//test conjugate-gradient
  double acga[]={   4,-1, 1
  		  ,-1, 4,-2
  		  , 1,-2, 4};
  double bcga[]={ 12
  		 ,-1
  		 , 5};
  structmatrix acg=creatematrix(acga,3,3,endt(MATDOUBLE),rowmjr);
  structmatrix bcg=creatematrix(bcga,3,1,endt(MATDOUBLE),rowmjr);
  structmatrix XCG=conjugategradient(&acg,&bcg);
  printf("Conjugate gradient solution\n");
  printmat(&XCG);//exact answer is 3,1,1 */
  }

  return 0;
}
