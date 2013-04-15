#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
int indexof(char *aword, char *arrayofwords[]);

//iter util
struct piterator {void *start;void *current;
  size_t objsize;unsigned int nobj;
void *last;
// struct piterator* myaddr;
};
typedef  struct piterator structpiterator;
structpiterator initpiterator(void* pstart,size_t objsize,unsigned int nobj);
void *reinitpiterator(structpiterator *pit);
void *getnextp(structpiterator *pit);



#endif
