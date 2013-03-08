#include <stdbool.h> //seriously C, i have in include bool??
#include <complex.h> 
#include "mmtypes.def"
 
#define nMAXLINE  256 //why should i need to declare a limit??
const char *strmmid="%%MatrixMarket";
#define WLEN 30
/*the above line makes WLEN compile-time constant
  const int wlen=123 is not compile-time constant which
  generates an error originating in the struct definition
  why? idk*/
//const int WLEN=30;//"word length"



typedef struct {char object[WLEN],format[WLEN],field[WLEN],symmetry[WLEN];}
  structheader;
typedef struct {unsigned long int nrows, ncols , nonzeros;}
  structsize;
typedef struct {structheader header; structsize size;}
  structmmfileattribs ;

 
/* data types of numerical values in the file */
//#define TMMINDEX unsigned int
#define tmmindex unsigned int //bc i want it as a string
//typedef  TMMINDEX tmmindex;
#define tmmint int
#define tmmfloat  float
#define tmmdouble double
#define tmmcomplex double complex//double complex or complex?

 //typedef struct {ptrtofunc,ptrtodatatype}
//since i can't have a list of data types OR even refer
//to 


//malloc in general way like this: 
//write func that always gives 4 pts. but nulls empty spot

/*
i'm trying to generically read any type of data line encountered in
a matrix format file. i have to assign a case, for every valid combo
 (object)(format)(field)(symmetry) the string object_format_field_symmetery
 will be an identifier

sets
valid combos of OFFS->Function
->combos of (m n|m|-None-)(value(float,int,double)|value(real) value(real))

*/

//write a macro funtion that takes 4 args and generates all combos 
//like matrix anyformat real any.
//need to count the number of combos to check repeats when all 
//valid combos are enumerated.

//data container for each data line type


//for vector format
//vector, array
//typedef struct {tmmint value;} structvectorarrayint;
// typedef struct {MMFLOATt value;} structvectorarrayfloat;
/* typedef struct {MMDOUBLEt value;} structvectorarraydouble; */
/* typedef struct {MMCOMPLEXt value;} structvectorarraycomplex; */
/* //for array format */



//i tried to generate all combos but then i'd have to invent
//my own c-preprocessor! so this is the best i can do
//very difficult to implement generic programming with C!

/*
keeping these lists in an array is the most useful
and elegant instead of #defines
object={"matrix","vector"}  //ALWAYS use these entitity to refer!
format={"coord","array"}
...etc
headerwords={obj,format,....} ..has a corresponding
typedef int array lenof object,format..
...integrate in below
typedef int array[lenofobj][lenofformat][...etc] typeheaderarray
headerarray={0} //might have to use memset(array,0,sizeof array)

define likea allformats: returns

i could make a table of values at compile time if 
struct headerindex {int int int int}
headerindex headerword2index(*char aword)
#define h2i
loop thru header array
headerstruct2index(*headerstruct)
iitem2headerarray(index,headerarray)//usually 0ed
returns arrayofarrays //the operation is +1 on elem
headerword2headerarray: headerword2index iitem2headerarray

a cool way to encode cases is to implement an array mask
acase=[1,3,0,4] where each num represents some (tabled) item
usenumbers from enum or #define or array of array of
chars so that you don't misindex 
rangeofcases=[[1,1],[0,1,1,1],[0,0,1,1],[0,1]]
assert lenof each is lenof object format..etc
len of each elem is num of possible values
def isacaseinrange(acase,rangeofcases):
for each elem 
if elem > 0 then it's there
def func
*/ 


//3/1
//define all data stucts here
//then refer to them via a huge case/if block
// dataytype: (m n|m|-None-)(value(float,int,double)|value(real) value(real))
//header info: object,format,field,symmetry
/*
cases: (no check for legality though)
1. mat|vec, array, real|int|double|complex(*),*: {field type}
2. mat, coord, *,*: {int int, field type}
3. vec, coord, *: {int, field type}
4. mat,*, 
*/






