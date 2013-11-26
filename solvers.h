#ifndef SOLVERS_H
#define SOLVERS_H

static void *swaprows(unsigned int m1, unsigned int m2, structmatrix *m);
//...used in gauss


 structmatrix backsolve(structmatrix *echform);
static structmatrix vecT(structmatrix *vec);
static void *mulmatbyconst(double *constv, structmatrix *m);

void *gaussop(structmatrix *AM);structmatrix gauss(structmatrix *A,structmatrix *b);

structmatrix jacobi(structmatrix *A,structmatrix *b);

structmatrix gaussseidel(structmatrix *A,structmatrix *b);

structmatrix soverrelaxation(structmatrix *A,structmatrix *b);

structmatrix conjugategradient(structmatrix *A,structmatrix *b);

#endif
