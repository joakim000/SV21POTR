#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define GENRAND(x) (((rand() << 16) | rand()) % x)
#elif __linux__
#define GENRAND(x) (rand() % x) 
#else
#error "Unsupported platform"
#endif

// Timing
#define TIMING(y, x) ((double)(x - y) / CLOCKS_PER_SEC)
clock_t timer_start; clock_t timer_end; 

// Helper functions
int checkArg(int argc, char *argv[], char arg[]);
uint32_t compare_array(uint32_t *num, uint32_t size, uint32_t *comp);
void copy_array(uint32_t *num, uint32_t size, uint32_t *out, bool prt, uint32_t tmax);
void generate_array(uint32_t *num, uint32_t size, uint32_t run_len, uint32_t rnd_max);
void print_array(uint32_t *num, uint32_t size, uint32_t tmax);
double timeComp(double libTime, double sortTime);

// Sort holder
typedef struct sort_holder {
    char* name;
    char* print_name;
    bool default_run;
    void (*sort_ptr)(uint32_t*, uint32_t size); // ptr to sort function
    bool run;
} sort;

// Help text
char* HELPTEXT1 = "\
 Flags\n \
  print input tables         -prtin\n \
  print result tables        -prtout\n \
  verbose output             -v\n \
  output csv                 -csv\n \
  output json                -json\n \
  timing                     -perf\n \
  disable testing            -notest\n \
  sort descending            -d\n \
\n \
Sorts to run                  ";

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
Print size (sanity check)      [100]   -tmax value\n";
