/*  Sorting exercise

    Main file: sorting.c

    This is a companion running script with some helper functions,
    prettyprint tables, functionality test and basic timing.
    stdlib qsort used as reference.
*/

#include "run.h"
#include "options.h"
#include "sorts.h"

uint32_t elements;

int main(int argc, char* argv[] )
{
    /* Defaults from options.h */
    // Flags
    bool prt = PRT; 
    bool pt = PT;  
    bool ct = CT; 
    bool t = T;
    uint32_t tmax = TMAX;

    // Sort ascending or descending
    bool descend = DESCEND;
    sort* sorts = descend ? sorts_d : sorts_a;
    
    // Sort set
    elements = ELEMENTS;
    uint32_t rnd_max = RND_MAX; 
    int32_t run_len = RUN_LEN;

     
      
    /* Process args */

    // printf("%d args:\n", argc);
    // for (int i = 0; i < argc; i++)
    //         printf("%d  %s\n", i, argv[i]);
    
    if (argc >= 2) {
        // Print help
        if (checkArg(argc, argv, "help") || (checkArg(argc, argv, "-h")) || (checkArg(argc, argv, "--help"))) {
            printf(HELPTEXT1);
            printf("[ ");
            for (int i = 0; i < NUMBER_OF_SORTS; i++) 
                if (sorts[i].default_run) 
                    printf("%s ", sorts[i].name);  
            printf("]\n");
            for (int i = 0; i < NUMBER_OF_SORTS; i++) 
                printf("%20s: %3s\n", sorts[i].print_name, sorts[i].name);        
            printf(HELPTEXT2);
            exit(0);
        }

        // Flags
        if (checkArg(argc, argv, "-it")) prt = true;
        if (checkArg(argc, argv, "-rt")) pt = true;
        if (checkArg(argc, argv, "-notest")) ct = false;
        if (checkArg(argc, argv, "-time")) t = true;
        if (checkArg(argc, argv, "-d")) descend = true;
        sorts = descend ? sorts_d : sorts_a;

        // Sorts to run
        for (int i = 0; i < NUMBER_OF_SORTS; i++) 
            sorts[i].run = (bool)checkArg(argc, argv, sorts[i].name); 

        // Numerical value args
        int arg_tmax = checkArg(argc, argv, "-tmax");
        tmax = arg_tmax ? strtol(argv[arg_tmax + 1], NULL, 10) : TMAX;
        int arg_size = checkArg(argc, argv, "-size");
        elements = arg_size ? strtol(argv[arg_size + 1], NULL, 10) : ELEMENTS;
        int arg_run = checkArg(argc, argv, "-run");
        run_len = arg_run ? strtol(argv[arg_run + 1], NULL, 10) : RUN_LEN;
        
        int arg_max = checkArg(argc, argv, "-max");
        if (arg_max) {
            // char s[10] = argv[arg_run + 1];
            char* s = argv[arg_max + 1];
            if (!strcmp(s, "i8"))
                rnd_max = INT8_MAX;
            else if (!strcmp(s, "i16"))
                rnd_max = INT16_MAX;
            else if (!strcmp(s, "i32"))
                rnd_max = INT32_MAX;
            else
                rnd_max = strtol(argv[arg_max + 1], NULL, 10);           
        }
       
    }

    // Print some info
    printf("\nSet size: %d    Biggest: %d    Composition: %d", elements, rnd_max, run_len);

    // If no sort args then use defaults
        for (int i = 1; i < NUMBER_OF_SORTS; i++) 
            if (sorts[i].run) 
                break;
            else
                for (int i = 1; i < NUMBER_OF_SORTS; i++) 
                    sorts[i].run = sorts[i].default_run; 

    
   
    // Alloc input, working and testing array
    uint32_t* random = calloc(elements, sizeof(uint32_t));
        assert( ("Memory allocation failed.", random != NULL) );
    uint32_t* numbers = calloc(elements, sizeof(uint32_t));
        assert( ("Memory allocation failed.", numbers != NULL) );
    uint32_t* compare = calloc(elements, sizeof(uint32_t));
        assert( ("Memory allocation failed.", compare != NULL) );

    // Generate input
    srand(time(0));
    generate_array(random, elements, run_len, rnd_max);
    // print_array(random, elements);

    /* Run libsort if needed for testing or timing */
    if (ct || t) {
        copy_array(random, elements, compare, prt, tmax);
        printf("\nLib qsort: \n"); 
        timer_start = clock();
            sorts[0].sort_ptr(compare, elements);
        timer_end = clock();
        if (pt) print_array(compare, elements, tmax);
        if (t) printf("%d elements in %5.3f seconds.\n", elements, TIMING(timer_start, timer_end));
        if (!pt && !t) printf("Done.\n");
    }

    /* Run sorts */
    for (int i = 1; i < NUMBER_OF_SORTS; i++) {
        if (sorts[i].run) {
            copy_array(random, elements, numbers, prt, tmax);
            printf("\n%s: \n", sorts[i].print_name);
            timer_start = clock();
                sorts[i].sort_ptr(numbers, elements);
            timer_end = clock();
            if (pt) print_array(numbers, elements, tmax);
            if (t) printf("%d elements in %5.3f seconds.\n", elements, TIMING(timer_start, timer_end));
            if (ct) compare_array(numbers, elements, compare);
            if (!pt && !t && !ct) printf("Done.\n");
        }
    }

    free(random);
    free(numbers);
    free(compare);

    return 0;
}

/* Helper functions */
void generate_random_array(uint32_t *num, uint32_t size, uint32_t rnd_max)
{
    for (size_t i = 0; i < size; i++) 
        num[i] = (uint32_t)GENRAND(rnd_max);
}

void generate_array(uint32_t *num, uint32_t size, uint32_t run_len, uint32_t rnd_max)
{
    if (run_len <= 0)    // Linear array
        for (size_t i = 0; i < size; i++) 
            num[i] = i;

    else if (run_len == 1)    // Random array
        generate_random_array(num, size, rnd_max);

    else {  // Mixed array
        int runSize = (run_len % 2 == 0) ? run_len + 1 : run_len;  // Must be odd number

        for (size_t i = 0; i < size; ) 
            if (i % 2) {
                uint32_t runStart = num[i - 1];
                for (int runEnd = i + runSize; i < runEnd && i < size; i++)
                    num[i] = runStart + i;
            }
            else {
                for (int runEnd = i + runSize; i < runEnd && i < size; i++) 
                    num[i] = (uint32_t)GENRAND(rnd_max);
            }
    }
}

void print_array(uint32_t *num, uint32_t size, uint32_t tmax)
{
    int orders = (int)log10(RND_MAX) + 1;
    int space = 2;
    char fmt[10];
    sprintf(fmt, "%%%dd", orders+space);

    for (size_t i = 0; i < size && i < tmax; i++)
    {
        printf(fmt, num[i]);
        if (i < size - 1)
        if (i != 0 && (i + 1) % 10 == 0 && i + 1 != size)
            printf("\n");
    }
    printf("\n");
}

void copy_array(uint32_t *num, uint32_t size, uint32_t *out, bool prt, uint32_t tmax) {
    for (int i = 0; i < size; i++) 
        out[i] = num[i];
    if (prt) printf("\nInput array:\n");
    if (prt) print_array(out, elements, tmax);
}

void compare_array(uint32_t *num, uint32_t size, uint32_t *comp)
{
    int errors = 0;
    for (size_t i = 0; i < size; i++)
        if (num[i] != comp[i])
            errors++;
    printf("%d errors in %d elements.\n", errors, size);
}

int checkArg(int argc, char *argv[], char arg[]) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], arg))
            { 
                // printf("\n%s found at index %d\n", argv[i], i); 
                return i;
            }
    }
    return 0;
}

void prep(char* title){

}

// static inline void cleanup() {
//      if (pt) print_array(numbers, elements, tmax);
//      if (t) printf("%d elements in %5.3f seconds.\n", elements, TIMING(timer_start, timer_end));
//      if (ct) compare_array(numbers, elements, compare);
// }