
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "readmmx.h"


int main(){
FILE *pfile=fopen("test.mm","r");
structmmdata  mydata;
mydata=readmmfile(pfile);

 unsigned int tr=0,tc=0;
 //uintptr_t
 structpiterator ps2vsit=getptrs2mmvec(&tr,&tc,&mydata);
 printf("\n start printing vector\n");
 while(getnextp(&ps2vsit)!=NULL){
     printmmvalat((void*)  *(uintptr_t*) (sizeof(uintptr_t)*0+ps2vsit.current)
		  ,&(mydata.mmfileattribs.header.field));
     printf(" ");}
 printf("\n end printing\n");

 /* double *da=copyasdouble(&tr,&tc,&mydata); */
 /* printf("valat %lf",da[3]); */

 //all arrays were dynamically created.
 //free malloced mem!
 freemmdata(&mydata);//free(da);
 return 0;}

