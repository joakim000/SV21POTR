/* Options for sorting exercise run            args    */

// Flags    true overrides missing arg
#define PRT false  // print input tables        -it
#define PT false  // print result tables        -rt
#define CT false // qsort comparison testing    -test
#define T false // timing                       -time

// Printing sanity check
#define TMAX 100   // max table size

// Sorts to run                                 
//                falls back     
#define    BUBBLE false                     //  bub
#define INSERTION false                     //  ins                
#define SELECTION false                     //  sel
#define     SHELL false                     //  she
#define     MERGE true                      //  mer
#define     QUICK false                     //  qui

/* Sort set options */
// Size                                         -size=X
//#define ELEMENTS 100
#define ELEMENTS 5000000                        

// Max random value                             -max={i8, i16, i32}
//#define RND_MAX INT16_MAX
#define RND_MAX INT32_MAX                       

// Run size, alternating linear / random.       -run=X
// 0: all linear, 1: all random
#define RUN_LEN 1                               