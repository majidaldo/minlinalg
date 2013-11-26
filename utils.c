#include "utils.h"
#include <stdio.h>
#include <assert.h>



int indexof(char *aword, char *arrayofwords[]){
  int i; unsigned int ct=10;
//unsigned int ct=COUNT_OF(arrayofwords); //DOES NOT COUNT!!! TODO!
  for (i=0 ; i<ct ;i++){
    if (strcmp(aword,arrayofwords[i])==0){return i;}}
  return -1;//not found
}
 
/*iterator utilities*/
structpiterator initpiterator(void* pstart,size_t objsize,unsigned int nobj){
  structpiterator pit;
  pit.start=pstart;
  pit.objsize=objsize;
  pit.nobj=nobj;
  pit.current=pstart-pit.objsize;
  pit.last=pit.current+pit.nobj*pit.objsize;
  return pit;
}
void *getnextp(structpiterator *pit){
  if(pit->current<pit->last){
    pit->current=pit->current+pit->objsize;
    return pit->current;}
  else{assert(pit->current>=pit->last);
    return NULL;}
}
void *reinitpiterator(structpiterator *pit){pit->current=pit->start-pit->objsize;}
//how piterator works
/* int ints[]={1,2,3,4}; */
/* structpiterator p2ints=initpiterator(ints,sizeof(int),4); */
/* while(getnextp(&p2ints)!=NULL){printf("%d ",*(int*) p2ints.current);}  */
/* reinitpiterator(&p2ints); */
/* while(getnextp(&p2ints)!=NULL){printf("%d ",*(int*) p2ints.current);}  */
