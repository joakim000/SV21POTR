/* Options for sorting exercise run            args    */

// Flags    true: disregard arg, always run
#define PRT false  // print input tables        
#define PT false  // print result tables        
#define CT true  // qsort comparison testing    
#define T false // timing                       
#define DESCEND false    // sort descending     

// Printing sanity check
#define TMAX 100   // max table size

// Sorts to run                                 
//    no sort arg falls back to these defs     
#define    BUBBLE false                     //  bub
#define INSERTION false                     //  ins                
#define SELECTION false                     //  sel
#define     SHELL false                     //  she
#define     MERGE false                     //  mer
#define    TMERGE false                     //  tme
#define     QUICK false                     //  qui
#define     RADIX false                     //  rad

/* Sort set options */
// Size                                         -size X
#define ELEMENTS 100
// #define ELEMENTS 5000000                        

// Max random value                             -max {i8, i16, i32, value}
//#define RND_MAX INT16_MAX
#define RND_MAX INT32_MAX                       

// Run size, alternating linear / random.       -run X
// 0: all linear, 1: all random
#define RUN_LEN 1                               

/* Other */
// Debugging
#define DEBUG false

// Threading
#define MAXTHREADS 12      
