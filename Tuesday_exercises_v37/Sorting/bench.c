/*  Sorting exercise bench

    Main file: sorting.c

    This is a companion running script with some helper functions,
    prettyprint tables, functionality test and basic timing.
    stdlib qsort used as reference.
*/
#include "bench.h"
#include "options.h"
#include "sort_struct.h"
#include "sort_init.h"
#include "cmdargs/cmdargs.h"

int main(int argc, char* argv[] )
{
    sort* sorts = sorts_a;   // Available sort implemenations

    // Timing 
    double libTime,         
           sortTime,
           elem_over_ns,
           lib_elem_over_ns;

    uint32_t errorCount;      // Comparison testing
    char jsonOut[0x400] = ""; // Buffer for json print / write
    FILE* fp;                 // File handle for saving data

    struct benchargs {
            // Flags
            bool prtin,     // print input
                prtout,     // print output
                prterr,     // ?
                noprtref,   // ?
                csv,        // print csv-format
                json,       // print json-format
                perf,       // print timings
                verbose,    // print more details
                notest,     // do not run lib sort
                descend;    // sort descending

            char fname[FILENAME_MAX];  // filename to save output

            uint32_t size,  // sort set size 
                compo,      // sort set composition
                maxNum;     // sort set max number
            
            uint32_t tmax,  // max table size for printing
                maxThreads; // max threads for threaded sorts
        } ba;
      
    /* Process args */
    // char* defaultArgs[] = {"exe", "tme", "-prterr", "-noprtref", 
                        // "-size", "1000", "-run", "0", "-tmax", "1000"};
    // char* defaultArgs[] = {"exe", "tme", "-perf", "-v", "-size", "8"}; 
    char* defaultArgs[] = {"exe", "help"}; 
    if (argc < 2) {
        argv = defaultArgs;
        argc = 2;
    }

    // Print help
    if (argc >= 2) {
        if (checkArg(argc, argv, "help") || (checkArg(argc, argv, "-h")) || (checkArg(argc, argv, "--help"))) {
            printf(HELPTEXT1);
            printf("[ ");
            for (int i = 0; i < NUMBER_OF_SORTS; i++) 
                if (sorts[i].default_run) 
                    printf("%s ", sorts[i].name);  
            printf("]\n");
            for (int i = 0; i < NUMBER_OF_SORTS; i++) 
                printf("%23s: %3s\n", sorts[i].print_name, sorts[i].name);        
            printf(HELPTEXT2);
            exit(0);
        }
    }
    
    // Process cmd line arguments, set defaults
    argdef_t defs[] = {
        { .isFlag = true, .var = (bool*)&ba.prtin, .str = "-prtin" },
        { .isFlag = true, .var = (bool*)&ba.prtout, .str = "-prtout" },
        { .isFlag = true, .var = (bool*)&ba.prterr, .str = "-prterr" },
        { .isFlag = true, .var = (bool*)&ba.prterr, .str = "-prterr" },
        { .isFlag = true, .var = (bool*)&ba.csv, .str = "-csv" },
        { .isFlag = true, .var = (bool*)&ba.json, .str = "-json" },
        { .isFlag = true, .var = (bool*)&ba.perf, .str = "-perf" },
        { .isFlag = true, .var = (bool*)&ba.verbose, .str = "-v" },
        { .isFlag = true, .var = (bool*)&ba.notest, .str = "-notest" },
        { .isFlag = true, .var = (bool*)&ba.descend, .str = "-d" },
        
        { .isInt = true,  .var = (int*)&ba.tmax, .str = "-tmax", .defaultInt = TMAX },
        { .isInt = true,  .var = (int*)&ba.size, .str = "-size", .defaultInt = ELEMENTS },
        { .isInt = true,  .var = (int*)&ba.compo, .str = "-run", .defaultInt = RUN_LEN },
        { .isInt = true,  .var = (int*)&ba.maxThreads, .str = "-thr", .defaultInt = MAXTHREADS },
        
        { .isString = true, .var = (char*)&ba.fname, .str = "-file" }
    };
    processArgs(argv, argc, defs, COUNT_OF(defs));
        
    /* cmdline args that need special handling */
    int ai; // argv index

    // Special argument: Has aliases
    ai = checkArg(argc, argv, "-max");
    if (ai) {
        char* s = argv[ai + 1];
        if (!strcmp(s, "i8"))
            ba.maxNum = INT8_MAX;
        else if (!strcmp(s, "i16"))
            ba.maxNum = INT16_MAX;
        else if (!strcmp(s, "i32"))
            ba.maxNum = INT32_MAX;
        else
            ba.maxNum = strtol(argv[ai + 1], NULL, 10);           
    }
    else
        ba.maxNum = RND_MAX;

    // Special argument: Sorts to run
    sorts = ba.descend ? sorts_d : sorts_a;  // Needs to be set before
    for (int i = 1; i < NUMBER_OF_SORTS; i++) 
        sorts[i].run = (bool)checkArg(argc, argv, sorts[i].name); 

    // If no sort args then use defaults
    bool foundSortArg = false;
    for (int i = 1; i < NUMBER_OF_SORTS; i++) 
        if (sorts[i].run) {
            foundSortArg = true;
            break;
        }
    if (!foundSortArg)
        for (int i = 1; i < NUMBER_OF_SORTS; i++) 
            sorts[i].run = sorts[i].default_run; 

   
    // Alloc arrays for input, working and testing
    uint32_t* random = calloc(ba.size, sizeof(uint32_t));
        assert( ("Memory allocation failed.", random != NULL) );
    uint32_t* numbers = calloc(ba.size, sizeof(uint32_t));
        assert( ("Memory allocation failed.", numbers != NULL) );
    uint32_t* compare = calloc(ba.size, sizeof(uint32_t));
        assert( ("Memory allocation failed.", compare != NULL) );

    // Generate input
    srand(time(0));
    generate_array(random, ba.size, ba.compo, ba.maxNum);
    // print_array(random, size);

    // Print some info
    if (ba.verbose)             
        printf("\nSet size: %d    Biggest: %d    Composition: %d", ba.size, ba.maxNum, ba.compo);

    // Open file for writing timing data
    if (ba.fname != NULL) 
        fp = fopen(ba.fname, "a");
    
    // Prepare args for sort functions
    struct sort_args args = {
        .num = compare,
        .size = ba.size,
        .maxThreads = ba.maxThreads,
        // .shellSeq
    };

    /* Run libsort if needed for testing or timing */
    if (!ba.notest) {
        // Setup
        copy_array(random, ba.size, compare, ba.prtin, ba.tmax, ba.maxNum);
        if (!(ba.csv || ba.json)) 
            printf("\nLib qsort: \n"); 
        // Run
        timer_start = clock();
            sorts[0].sort_ptr((void *)&args);
        timer_end = clock();
        // Process timing        
        libTime = TIMING(timer_start, timer_end);
        lib_elem_over_ns = libTime > 0 ? ba.size / libTime / 1000000 : 0; 
        // Printing
        if (!ba.noprtref && (ba.prtout || ba.prterr)) 
            print_array(compare, ba.size, ba.tmax, ba.maxNum);
        if (ba.perf) 
            printf("%d elements in %5.3f seconds, %5.3f ns/element. \n", ba.size, libTime, lib_elem_over_ns);
        if (!ba.prtout && !ba.prterr && !ba.perf & !ba.csv && !ba.json) 
            printf("Done.\n");
    }

    // Print csv data 
    if (ba.csv) {
        printf("%d,%d,%d\n", ba.size, ba.maxNum, ba.compo);   
        printf("%s,%5.3f\n", sorts[0].name, libTime);
    }
   
    // Prepare json data for print or write
    if (ba.json || (fp != NULL)) {
        char buf[0x400] = "";
        time_t now = time(0);
        struct tm* dt = localtime(&now);
        sprintf(buf,  "{\n\t\"dt\": \"%4d%02d%02d:%02d%02d%02d\",\n", 1900+dt->tm_year, 1+dt->tm_mon, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec); strcat(jsonOut, buf);
        sprintf(buf,  "\t\"size\": %d,\n", ba.size); strcat(jsonOut, buf);
        sprintf(buf,  "\t\"maxnum\": %d,\n", ba.maxNum); strcat(jsonOut, buf);
        sprintf(buf,  "\t\"composition\": %d,\n", ba.compo); strcat(jsonOut, buf);
        sprintf(buf,  "\t\"threads\": %d,\n", args.maxThreads); strcat(jsonOut, buf);
        strcat(jsonOut, "\t\"sorted\": [ {\n");
        sprintf(buf,  "\t\t\"name\": \"%s\",\n", sorts[0].name); strcat(jsonOut, buf);
        sprintf(buf,  "\t\t\"el/us\": %5.3f,\n", elem_over_ns); strcat(jsonOut, buf);
        sprintf(buf,  "\t\t\"time\": %5.3f\n\t}, ", libTime); strcat(jsonOut, buf);
    }
    
    /* Run sorts */
    // For formatting, find index of last sort to run
    int last_to_run = 0; 
    for (int i = 0; i < NUMBER_OF_SORTS; i++)
        if (sorts[i].run) last_to_run = i;

    for (int i = 1; i < NUMBER_OF_SORTS; i++) {
        if (sorts[i].run) {
            // Setup
            args.num = numbers;
            copy_array(random, ba.size, numbers, ba.prtin, ba.tmax, ba.maxNum);
            if (!(ba.csv || ba.json)) printf("\n%s: \n", sorts[i].print_name);
            // Run
            timer_start = clock();
                sorts[i].sort_ptr((void *)&args);
            timer_end = clock();
            // Process timing
            sortTime = TIMING(timer_start, timer_end);
            elem_over_ns = sortTime > 0 ? ba.size / sortTime / 1000000 : 0;
            errorCount = compare_array(numbers, ba.size, compare);
            // Printing
            if (!ba.csv && !ba.json && (ba.verbose || (errorCount > 0)))
                printf("%d errors in %d elements.\n", errorCount, ba.size);
            if (ba.prtout) 
                print_array(numbers, ba.size, ba.tmax, ba.maxNum);
            if (ba.prterr) 
                compare_print_array(numbers, ba.size, compare, ba.tmax, ba.maxNum);
            if (ba.perf) 
                printf("%d elements in %5.3f seconds, %5.3f ns/element. %5.3f %% of libsort performance. \n", ba.size, sortTime, elem_over_ns, timeComp(libTime, sortTime));
            if (!ba.prtout && !ba.prterr && !ba.perf && !ba.csv && !ba.json) 
                printf("Done.\n");
            if (ba.csv) 
                printf("%s,%5.3f,%5.3f,%d\n", sorts[i].name, sortTime, timeComp(libTime, sortTime), errorCount);
            // Send to buffer for later printing / writing            
            if (ba.json || (fp != NULL)) {
                char buf[0x400] = "";
                strcat(jsonOut, "{\n");                
                sprintf(buf, "\t\t\"name\": \"%s\",\n", sorts[i].name); strcat(jsonOut, buf);                
                sprintf(buf, "\t\t\"el/us\": %5.3f,\n", elem_over_ns); strcat(jsonOut, buf);
                sprintf(buf, "\t\t\"time\": %5.3f,\n", sortTime); strcat(jsonOut, buf);
                sprintf(buf, "\t\t\"compare\": %5.3f,\n", timeComp(libTime, sortTime)); strcat(jsonOut, buf);
                if (i == last_to_run) {
                    sprintf(buf, "\t\t\"errors\": %d\n\t}", errorCount); strcat(jsonOut, buf);
                } else {
                    sprintf(buf, "\t\t\"errors\": %d\n\t}, ", errorCount); strcat(jsonOut, buf);
                }
            }
        }
    }

    // Final formatting and print / write
    if (ba.json || (fp != NULL)) 
       strcat(jsonOut, " ]\n}\n");
    if (ba.json)
        puts(jsonOut);
    if (fp != NULL) {
        fprintf(fp, jsonOut);
        fclose(fp);
    }

    // Never forget
    free(random);
    free(numbers);
    free(compare);

    return 0;
}

/* Helper functions */
uint32_t compare_array(uint32_t *num, uint32_t size, uint32_t *comp)
{
    uint32_t errors = 0;
    for (size_t i = 0; i < size; i++)
        if (num[i] != comp[i])
            errors++;

    return errors;
}

void compare_print_array(uint32_t *num, uint32_t size, uint32_t *comp, uint32_t tmax, uint32_t maxNum)
{
    // int orders = (int)log10(ba.maxNum) + 1;  // Build chain troubles with <math.h>
    int orders = 10;
    int space = 2;
    char fmt[10], errfmt[10];
    sprintf(fmt, "%%%dd", orders+space);
    sprintf(errfmt, "\e[1;31m%%%dd\e[m", orders+space); // red text
    int errors = 0;

    for (size_t i = 0; i < size && i < tmax; i++)
    {
        if (num[i] == comp[i])
            printf(fmt, num[i]);
        else {
            printf(errfmt, num[i]);
            errors++;
        }
        if (i != 0 && (i + 1) % 10 == 0 && i + 1 != size)
            printf("\n");
    }
    printf("\n\t%d errors printed.\n", errors);
}

void copy_array(uint32_t *num, uint32_t size, uint32_t *out, bool prtin, uint32_t tmax, uint32_t maxNum) {
    for (size_t i = 0; i < size; i++) 
        out[i] = num[i];
    if (prtin) printf("\nInput array:\n");
    if (prtin) print_array(out, size, tmax, maxNum); 
}

void generate_array(uint32_t *num, uint32_t size, uint32_t compo, uint32_t maxNum)
{
    if (compo <= 0)    // Linear array
        for (size_t i = 0; i < size; i++) 
            num[i] = i;

    else if (compo == 1)    // Random array
        generate_random_array(num, size, maxNum);

    else {  // Mixed array
        int runSize = (compo % 2 == 0) ? compo + 1 : compo;  // Must be odd number

        for (size_t i = 0; i < size; ) 
            if (i % 2) {
                uint32_t runStart = num[i - 1];
                for (int runEnd = i + runSize; i < runEnd && i < size; i++)
                    num[i] = runStart + i;
            }
            else {
                for (int runEnd = i + runSize; i < runEnd && i < size; i++) 
                    num[i] = (uint32_t)GENRAND(maxNum);
            }
    }
}

void generate_random_array(uint32_t *num, uint32_t size, uint32_t maxNum)
{
    for (size_t i = 0; i < size; i++) 
        num[i] = (uint32_t)GENRAND(maxNum);
}

void print_array(uint32_t *num, uint32_t size, uint32_t tmax, uint32_t maxNum)
{
    // int orders = (int)log10(maxNum) + 1;     // Build chain troubles with <math.h>
    int orders = 10;
    int space = 2;
    char fmt[10], errfmt[10];
    sprintf(fmt, "%%%dd", orders+space);  

    for (size_t i = 0; i < size && i < tmax; i++)
    {
        printf(fmt, num[i]);
        if (i != 0 && (i + 1) % 10 == 0 && i + 1 != size)
            printf("\n");
    }
    printf("\n");
}

double timeComp(double libTime, double sortTime) {
    if (!(libTime > 0))
        if (sortTime > 0)
            return 0;
        else
            return 100;
    else
        return libTime / sortTime * 100;
}


