#ifndef MMTYPES_H
#define MMTYPES_H

#include <stdbool.h> //seriously C, i have in include bool??
#include <complex.h> 

/*keywords*/
//object values
#define VECTOR "vector"
#define MATRIX "matrix"
//format
#define COORDINATE "coordinate"
#define ARRAY "array"
//field
#define REAL "real"
#define DOUBLE "double"
#define COMPLEX "complex"
#define INTEGER "integer"
#define PATTERN "pattern"
//symmetry
#define GENERAL "general"
#define SYMMETRIC "symmetric"
#define SKEWSYMMETRIC "skew-symmetric"
#define HERMITIAN "hermitian"

/* data types of numerical values in the file */
//#define TMMINDEX unsigned int
#define tmmindex unsigned int //bc i want it as a string
#define stmmindex "unsigned int"
//typedef  TMMINDEX tmmindex;
#define tmmreal  float  //should rename to cmmreal for "C"
#define tmmdouble double
#define tmmcomplex double complex //double complex or complex?
#define tmmint int

//why does const fail?
  char *object[]={VECTOR, MATRIX};
  char *format[]={COORDINATE, ARRAY};
  char *field[]={REAL, DOUBLE, COMPLEX, INTEGER,PATTERN };
char *ctypes[]={"float* ","double* ","float* ","int* "};//should match field
  char *symmetry[]={GENERAL, SYMMETRIC, SKEWSYMMETRIC, HERMITIAN};




#endif



