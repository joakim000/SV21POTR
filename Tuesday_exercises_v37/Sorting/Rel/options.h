/* Options for sorting exercise run            args    */

// Flags    true: disregard arg, always run
#define PRT false  // print input tables        -it
#define PT false  // print result tables        -rt
#define CT true  // qsort comparison testing    -notest
#define T false // timing                       -time
#define DESCEND false    // sort descending     -desc

// Printing sanity check
#define TMAX 100   // max table size

// Sorts to run                                 
//    no sort arg falls back to these defs     
#define    BUBBLE false                     //  bub
#define INSERTION false                     //  ins                
#define SELECTION false                     //  sel
#define     SHELL false                     //  she
#define     MERGE false                     //  mer
#define     QUICK false                     //  qui
#define    TMERGE false                     //  tme
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

// Debugging
#define DEBUG true


