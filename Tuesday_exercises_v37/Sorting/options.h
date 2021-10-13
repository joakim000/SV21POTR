/* Options for sorting exercise run            args    */

// Flags    true: disregard arg, always run
#define PRT false  // print input tables        -it
#define PT false  // print result tables        -rt
#define CT true  // qsort comparison testing    -notest
#define T false // timing                       -time

// Printing sanity check
#define TMAX 100   // max table size

// Sorts to run                                 
//    no sort arg falls back to these defs     
#define    BUBBLE false                     //  bub
#define INSERTION false                     //  ins                
#define SELECTION false                     //  sel
#define     SHELL false                     //  she
#define     MERGE true                      //  mer
#define     QUICK false                     //  qui

/* Sort set options */
// Size                                         -size=X
#define ELEMENTS 100
// #define ELEMENTS 5000000                        

// Max random value                             -max={i8, i16, i32}
//#define RND_MAX INT16_MAX
#define RND_MAX INT32_MAX                       

// Run size, alternating linear / random.       -run=X
// 0: all linear, 1: all random
#define RUN_LEN 1                               

// Debugging
#define DEBUG true


// Help text
char* HELPTEXT1 = "\
 Flags\n \
  print input tables         -it\n \
  print result tables        -rt\n \
  disable testing            -notest\n \
  timing                     -time\n \
\n \
Sorts to run\n \
    BUBBLE   bub\n \
 INSERTION   ins\n \
 SELECTION   sel\n \
     SHELL   she\n \
     MERGE   mer\n \
     QUICK   qui\n \
Default: \
";

char* HELPTEXT2 = "\
\n\n \
Sort set\n \
  Size                         [100]   -size value\n \
  Biggest random value         [i32]   -max {i8, i16, i32, value}\n \
  Composition                  [1]     -run value\n \
    0: all linear\n \
    1: all random\n \
    >1: alternating runs\n \
        of value length.\n \
\n \
Print size (sanity check)      [100]   -tmax value\n \
";
