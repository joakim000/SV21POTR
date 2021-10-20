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

uint32_t elements;

int main(int argc, char* argv[] )
{
    /* Defaults from options.h */
    // Flags
    bool prtin = false;
    bool prtout = false;
    bool prterr = false;
    bool noprtref = false;
    bool csv = false;
    bool json = false;
    bool perf = false;
    bool verbose = false;
    bool notest = false;
    bool write = false;
    uint32_t tmax = TMAX;
    uint16_t threads = 12;

    // Sort ascending or descending
    bool descend = DESCEND;
    sort* sorts = descend ? sorts_d : sorts_a;
    
    // Sort set
    elements = ELEMENTS;
    uint32_t rnd_max = RND_MAX; 
    int32_t run_len = RUN_LEN;

    /* Test / Timing */
    double libTime, sortTime, elem_over_ns, lib_elem_over_ns; 
    uint32_t errorCount;
    char* fname[FILENAME_MAX];
      
    /* Process args */

    // printf("%d args:\n", argc);
    // for (int i = 0; i < argc; i++)
    //         printf("%d  %s\n", i, argv[i]);
    
    // char* fakeargs[] = {"run.exe", "tme", "-prterr", "-noprtref", 
                        // "-size", "1000", "-run", "0", "-tmax", "1000"};
    // char* fakeargs[] = {"run.exe", "tme", "-perf", "-v", "-size", "8"}; 
    char* fakeargs[] = {"run.exe", "help"}; 
    if (argc < 2) {
        argv = fakeargs;
        argc = 2;
    }

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
                printf("%23s: %3s\n", sorts[i].print_name, sorts[i].name);        
            printf(HELPTEXT2);
            exit(0);
        }

        if (checkArg(argc, argv, "-prtin")) prtin = true;
        if (checkArg(argc, argv, "-prtout")) prtout = true;
        if (checkArg(argc, argv, "-prterr")) prterr = true;
        if (checkArg(argc, argv, "-prterr")) noprtref = true;
        if (checkArg(argc, argv, "-csv")) csv = true;
        if (checkArg(argc, argv, "-json")) json = true;
        if (checkArg(argc, argv, "-perf")) perf = true;
        if (checkArg(argc, argv, "-v")) verbose = true;
        if (checkArg(argc, argv, "-notest")) notest = true;
        
        if (checkArg(argc, argv, "-d")) descend = true;
        sorts = descend ? sorts_d : sorts_a;

        // Sorts to run
        for (int i = 1; i < NUMBER_OF_SORTS; i++) 
            sorts[i].run = (bool)checkArg(argc, argv, sorts[i].name); 

        // Save file  
        int arg_fname = checkArg(argc, argv, "-file");
        *fname = arg_fname ? argv[arg_fname + 1] : "";
        // write = 
        
        // Numerical value args
        int arg_tmax = checkArg(argc, argv, "-tmax");
        tmax = arg_tmax ? strtol(argv[arg_tmax + 1], NULL, 10) : TMAX;
        int arg_size = checkArg(argc, argv, "-size");
        elements = arg_size ? strtol(argv[arg_size + 1], NULL, 10) : ELEMENTS;
        int arg_run = checkArg(argc, argv, "-run");
        run_len = arg_run ? strtol(argv[arg_run + 1], NULL, 10) : RUN_LEN;
        int arg_thr = checkArg(argc, argv, "-thr");
        threads = arg_thr ? strtol(argv[arg_thr + 1], NULL, 10) : 12;  // Default 12 threads
        
        int arg_max = checkArg(argc, argv, "-max");
        if (arg_max) {
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

    if (verbose)             
        printf("\nSet size: %d    Biggest: %d    Composition: %d", elements, rnd_max, run_len);

    // Open file for writing timing data
    FILE* fp; 
    if (*fname != NULL) 
        fp = fopen(*fname, "a");
    char jsonOut[0x400] = "";

    // Prepare args
    struct sort_args args = {
        .num = compare,
        .size = elements,
        .maxThreads = threads,
        // .shellSeq
    };

    /* Run libsort if needed for testing or timing */
    if (!notest) {
        copy_array(random, elements, compare, prtin, tmax);
        if (!(csv || json)) 
            printf("\nLib qsort: \n"); 
        timer_start = clock();
            sorts[0].sort_ptr((void *)&args);
        timer_end = clock();
        libTime = TIMING(timer_start, timer_end);
        lib_elem_over_ns = libTime > 0 ? elements / libTime / 1000000 : 0; 
        if (!noprtref && (prtout || prterr)) 
            print_array(compare, elements, tmax, rnd_max);
        if (perf) 
            printf("%d elements in %5.3f seconds, %5.3f ns/element. \n", elements, libTime, lib_elem_over_ns);
        if (!prtout && !prterr && !perf & !csv && !json) 
            printf("Done.\n");
    }

   

    // Print some info
    if (csv) {
        printf("%d,%d,%d\n", elements, rnd_max, run_len);   
        printf("%s,%5.3f\n", sorts[0].name, libTime);
    }
    // else if (json) {
    //     time_t now = time(0);
    //     struct tm* dt = localtime(&now);
    //     printf("{\n\t\"dt\": \"%4d%02d%02d:%02d%02d%02d\",\n", 1900+dt->tm_year, 1+dt->tm_mon, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);
    //     printf("\t\"size\": %d,\n", elements);
    //     printf("\t\"maxnum\": %d,\n", rnd_max);
    //     printf("\t\"composition\": %d,\n", run_len);
    //     printf("\t\"threads\": %d,\n", args.maxThreads);
    //     printf("\t\"sorted\": [ {\n");
    //     printf("\t\t\"name\": \"%s\",\n", sorts[0].name);
    //     printf("\t\t\"el/us\": %5.3f,\n", elem_over_ns);
    //     printf("\t\t\"time\": %5.3f\n\t}, ", libTime);
    // }
    // if (fp != NULL) {
    //     time_t now = time(0);
    //     struct tm* dt = localtime(&now);
    //     fprintf(fp, "{\n\t\"dt\": \"%4d%02d%02d:%02d%02d%02d\",\n", 1900+dt->tm_year, 1+dt->tm_mon, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);
    //     fprintf(fp, "\t\"size\": %d,\n", elements);
    //     fprintf(fp, "\t\"maxnum\": %d,\n", rnd_max);
    //     fprintf(fp, "\t\"composition\": %d,\n", run_len);
    //     fprintf(fp, "\t\"threads\": %d,\n", args.maxThreads);
    //     fprintf(fp, "\t\"sorted\": [ {\n");
    //     fprintf(fp, "\t\t\"name\": \"%s\",\n", sorts[0].name);
    //     fprintf(fp, "\t\t\"el/us\": %5.3f,\n", elem_over_ns);
    //     fprintf(fp, "\t\t\"time\": %5.3f\n\t}, ", libTime);

    // }
    if (json || (fp != NULL)) {
        char buf[0x400] = "";
        time_t now = time(0);
        struct tm* dt = localtime(&now);
        sprintf(buf,  "{\n\t\"dt\": \"%4d%02d%02d:%02d%02d%02d\",\n", 1900+dt->tm_year, 1+dt->tm_mon, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec); strcat(jsonOut, buf);
        sprintf(buf,  "\t\"size\": %d,\n", elements); strcat(jsonOut, buf);
        sprintf(buf,  "\t\"maxnum\": %d,\n", rnd_max); strcat(jsonOut, buf);
        sprintf(buf,  "\t\"composition\": %d,\n", run_len); strcat(jsonOut, buf);
        sprintf(buf,  "\t\"threads\": %d,\n", args.maxThreads); strcat(jsonOut, buf);
        strcat(jsonOut, "\t\"sorted\": [ {\n");
        sprintf(buf,  "\t\t\"name\": \"%s\",\n", sorts[0].name); strcat(jsonOut, buf);
        sprintf(buf,  "\t\t\"el/us\": %5.3f,\n", elem_over_ns); strcat(jsonOut, buf);
        sprintf(buf,  "\t\t\"time\": %5.3f\n\t}, ", libTime); strcat(jsonOut, buf);
    }
    
    /* Run sorts */
    args.num = numbers;
    int last_to_run = 0;
    for (int i = 0; i < NUMBER_OF_SORTS; i++)
        if (sorts[i].run) last_to_run = i;

    for (int i = 1; i < NUMBER_OF_SORTS; i++) {
        if (sorts[i].run) {
            copy_array(random, elements, numbers, prtin, tmax);
            if (!(csv || json)) printf("\n%s: \n", sorts[i].print_name);
            timer_start = clock();
                sorts[i].sort_ptr((void *)&args);
            timer_end = clock();
            sortTime = TIMING(timer_start, timer_end);
            elem_over_ns = sortTime > 0 ? elements / sortTime / 1000000 : 0;
            errorCount = compare_array(numbers, elements, compare);
            if (!csv && !json && (verbose || (errorCount > 0)))
                printf("%d errors in %d elements.\n", errorCount, elements);
            if (prtout) 
                print_array(numbers, elements, tmax, rnd_max);
            if (prterr) 
                compare_print_array(numbers, elements, compare, tmax, rnd_max);
            if (perf) 
                printf("%d elements in %5.3f seconds, %5.3f ns/element. %5.3f %% of libsort performance. \n", elements, sortTime, elem_over_ns, timeComp(libTime, sortTime));
            if (!prtout && !prterr && !perf && !csv && !json) 
                printf("Done.\n");
            if (csv) 
                printf("%s,%5.3f,%5.3f,%d\n", sorts[i].name, sortTime, timeComp(libTime, sortTime), errorCount);
            
            if (json || (fp != NULL)) {
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
            
            // if (json) {
            //     printf("{\n");
            //     printf("\t\t\"name\": \"%s\",\n", sorts[i].name);
            //     printf("\t\t\"el/us\": %5.3f,\n", elem_over_ns);
            //     printf("\t\t\"time\": %5.3f,\n", sortTime);
            //     printf("\t\t\"compare\": %5.3f,\n", timeComp(libTime, sortTime));
            //     if (i == NUMBER_OF_SORTS - 2)
            //         printf("\t\t\"errors\": %d\n\t}", errorCount);
            //     else
            //         printf("\t\t\"errors\": %d\n\t}, ", errorCount);
            // }
            // if (fp != NULL) {
            //     fprintf(fp, "{\n");
            //     fprintf(fp, "\t\t\"name\": \"%s\",\n", sorts[i].name);
            //     fprintf(fp, "\t\t\"el/us\": %5.3f,\n", elem_over_ns);
            //     fprintf(fp, "\t\t\"time\": %5.3f,\n", sortTime);
            //     fprintf(fp, "\t\t\"compare\": %5.3f,\n", timeComp(libTime, sortTime));
            //     if (i == NUMBER_OF_SORTS - 2)
            //         fprintf(fp, "\t\t\"errors\": %d\n\t}", errorCount);
            //     else
            //         fprintf(fp, "\t\t\"errors\": %d\n\t}, ", errorCount);
            // }
            


        }
    }

    // if (json || (fp != NULL)) 
    //     strcat(jsonOut, "{\n");  

    if (json || (fp != NULL)) 
       strcat(jsonOut, " ]\n}\n");

    if (json)
        puts(jsonOut);

    if (fp != NULL) {
        fprintf(fp, jsonOut);
        fclose(fp);
    }

    

    free(random);
    free(numbers);
    free(compare);

    return 0;
}

/* Helper functions */
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

uint32_t compare_array(uint32_t *num, uint32_t size, uint32_t *comp)
{
    uint32_t errors = 0;
    for (size_t i = 0; i < size; i++)
        if (num[i] != comp[i])
            errors++;

    return errors;
}

void compare_print_array(uint32_t *num, uint32_t size, uint32_t *comp, uint32_t tmax, uint32_t rnd_max)
{
    // int orders = (int)log10(rnd_max) + 1;
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

void copy_array(uint32_t *num, uint32_t size, uint32_t *out, bool prtin, uint32_t tmax) {
    for (int i = 0; i < size; i++) 
        out[i] = num[i];
    if (prtin) printf("\nInput array:\n");
    if (prtin) print_array(out, elements, tmax, RND_MAX); // TODO don't use macro
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

void generate_random_array(uint32_t *num, uint32_t size, uint32_t rnd_max)
{
    for (size_t i = 0; i < size; i++) 
        num[i] = (uint32_t)GENRAND(rnd_max);
}

void print_array(uint32_t *num, uint32_t size, uint32_t tmax, uint32_t rnd_max)
{
    // int orders = (int)log10(rnd_max) + 1;
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

