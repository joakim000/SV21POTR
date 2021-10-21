/*  Tests for cmdargs

   gcc cmdargs_test.c cmdargs.c -o ca
*/

#include "cmdargs.h"

void tcp(char title[], char str1[], char str2[]);

/* int processArgs(char* argv[], int argc, argdef defs[], int defcount) {
    if (argc >= 2) {
        // printf("count of defs: %d\n", defcount);
                
        // puts("argv inside processArgs:");
        // for (int i = 0; i < argc; i++)
        //     puts(argv[i]);
        // puts("");

        // puts("Compare-strings from defs[i]->str :");
        // for (int i = 0; i < defcount; i++) {
        //     // puts(defs[i]->str);
        //     puts(defs[i].str);
        // }
        // puts("adresses from defs[i]->var :");
        // for (int i = 0; i < defcount; i++) {
        //     // puts(defs[i]->str);
        //     printf("%p\n", defs[i].var);
        // }
        
       
        for (int i = 0; i < defcount; i++) {            
            if (defs[i].isFlag) {
                bool* p = (bool*)defs[i].var;
                *p = checkArg(argc, argv, defs[i].str) ? true : false;               
                // defs[i].val = checkArg(argc, argv, defs[i].str) ? (bool*)true : (bool*)false;   
                // defs[i].valBool = checkArg(argc, argv, defs[i].str) ? true : false;   
            } 
            if (defs[i].isInt) {
                int index = checkArg(argc, argv, defs[i].str);
                int* p = (int*)defs[i].var;
                *p = index ? strtol(argv[index + 1], NULL, 10) : defs[i].defaultInt;
                // defs[i].val = index ? (int)strtol(argv[index + 1], NULL, 10) : (int)defs[i].defaultInt;
                // defs[i].valInt = index ? strtol(argv[index + 1], NULL, 10) : defs[i].defaultInt;
                // printf("defs[%d].valInt set to %d\n", i, defs[i].valInt);
            } 

            if (defs[i].isString) {
                char* p = (char*)defs[i].var;
                int index = checkArg(argc, argv, defs[i].str);
                strcpy(p, index ? (char*)argv[index + 1] : (char*)"default string");


                // defs[i].val = index ? (char*)argv[index + 1] : (char*)"default string";
                // strcpy(defs[i].valString, index ? (char*)argv[index + 1] : (char*)"default string");
                // strcpy(ca.aString, defs[2].val );
                // printf("defs[%d].valString set to \"%s\"\n", i, defs[i].valString);
            }

        }
    }
    

    return 0;
} */

int main(int argc, char* argv[] )
{
    /* Bunch of vars */
    bool prtin = false;
    bool prtout = false;
    // bool prterr = false;
    // bool noprtref = false;
    // bool csv = false;
    // bool json = false;
    // bool perf = false;
    // bool verbose = false;
    // bool notest = false;
    // bool write = false;
    // uint16_t threads = 12;
    // int run_len, tmax, 
    int elements;
    // char* fname[FILENAME_MAX];
    // int rnd_max;
    char aString[0x400];

    /* Automated input */


    // char* fakeargs[] = {"run.exe", "tme", "-prterr", "-noprtref", 
                        // "-size", "1000", "-run", "0", "-tmax", "1000"};
    // char* fakeargs[] = {"run.exe", "tme", "-perf", "-v", "-size", "8"}; 
    // char* fakeargs[] = {"exe", "-prtin", "-size", "66", "-file", "file.txt"}; 
    char* fakeargs[] = {"exe", "-prtin", "-size", "66", "-as", "nice str"}; 
    if (argc < 4) {
    // if (true) {
        argv = fakeargs;
        argc = 6;
    }

    /* Print argv */
    printf("%d args:\n", argc);
    for (int i = 0; i < argc; i++)
            printf("%d  %s\n", i, argv[i]);
    puts("");
    

    /* setup for cmdargs */ 
    struct cmdargs {
        bool prtin,
            prtout,
            // prterr,
            // noprtref,
            // csv,
            // json,
            perf;

        char* fname[FILENAME_MAX];
        char aString[0x400];
        

        int tmax,
            // compo,
            // maxThreads,
            // maxNum,
            size;
    } ca 
    = {
        .prtin = false,
        .size = 400,
        .aString = "boll"
    };

    
    argdef_t defs[] = {
        {
            .isFlag = true,
            .var = &ca.prtin,
            .str = "-prtin" 
        },
        {
            .isInt = true,
            // .var = (int*)&ca.size,
            .var = &ca.size,
            .str = "-size",
            .defaultInt = 200
        },
        {
            .isString = true,
            .var = &ca.aString,
            .str = "-as"
            // .defaultString = ""
        },
        {
            .isString = true,
            .var = &ca.fname,
            .str = "-file"
            // .defaultString = ""
        }
    };

    // argdef** dp = &defs;

    
    /* Call cmdargs */
    if (argc >= 2) {
        // printf("count of defs: %d\n", COUNT_OF(defs));
        processArgs(argv, argc, defs, COUNT_OF(defs));
        // *ca.aString = (char*)defs[2].var; 

        /* basic result printing */
        // if (ca.prtin) puts("prtin true"); else puts("prtin false");
        // printf("ca.prtin       -prtin %s\n", ca.prtin ? "true" : "false");
        // printf("defs[0].val    -prtin %s\n", defs[0].val ? "true" : "false");
        printf("ca.prtin          -prtin %16d\t%p\n", ca.prtin,      &ca.prtin); 
        // printf("defs[0].val       -prtin %16d\t%p\n", defs[0].val,   &defs[0].val);
        printf("defs[0].var       -prtin %16p\t%p\n", defs[0].var,   &defs[0].var);
        // printf("defs[0].valBool   -prtin %16d\t%p\n", defs[0].valBool,   &defs[0].valBool);  

        printf("ca.size           -size  %16d\t%p\n", ca.size,       &ca.size);
        // printf("defs[1].val       -size  %16d\t%p\n", defs[1].val,   &defs[1].val);
        printf("defs[1].var       -size  %16p\t%p\n", defs[1].var,   &defs[1].var);
        // printf("defs[1].valInt    -size  %16d\t%p\n", defs[1].valInt,   &defs[1].valInt);  

        printf("ca.aString        -as    %16s\t%p\n", ca.aString,    &ca.aString);
        // printf("defs[2].val       -as    %16s\t%p\n", defs[2].val,   &defs[2].val);  
        printf("defs[2].var       -as    %16p\t%p\n", defs[2].var,   &defs[2].var);  
        // printf("defs[2].valString -as    %16s\t%p\n", defs[2].valString,   &defs[2].valString);  
       

        // puts("ca.prtin = defs[0].val; ca.size = defs[1].val; strcpy(ca.aString, defs[2].val );");
        // ca.prtin = defs[0].val;
        // ca.size = defs[1].val;
        // strcpy(ca.aString, defs[2].val );

        // printf("ca.prtin          -prtin %16d\t%p\n", ca.prtin,      &ca.prtin); 
        // printf("defs[0].val       -prtin %16d\t%p\n", defs[0].val,   &defs[0].val);
        // printf("defs[0].var       -prtin %16p\t%p\n", defs[0].var,   &defs[0].var);
        // printf("defs[0].valBool   -prtin %16d\t%p\n", defs[0].valBool,   &defs[0].valBool);  

        // printf("ca.size           -size  %16d\t%p\n", ca.size,       &ca.size);
        // printf("defs[1].val       -size  %16d\t%p\n", defs[1].val,   &defs[1].val);
        // printf("defs[1].var       -size  %16p\t%p\n", defs[1].var,   &defs[1].var);
        // printf("defs[1].valInt    -size  %16d\t%p\n", defs[1].valInt,   &defs[1].valInt);  

        // printf("ca.aString        -as    %16s\t%p\n", ca.aString,    &ca.aString);
        // printf("defs[2].val       -as    %16s\t%p\n", defs[2].val,   &defs[2].val);  
        // printf("defs[2].var       -as    %16p\t%p\n", defs[2].var,   &defs[2].var);  
        // printf("defs[2].valString -as    %16s\t%p\n", defs[2].valString,   &defs[2].valString);  
      
      
    }

     /* Boutique solution from another proj, for comparisons */
    if (argc >= 2) {
        // if (checkArg(argc, argv, "-prtin")) prtin = true;
        prtin = checkArg(argc, argv, "-prtin") ? true : false;
        if (checkArg(argc, argv, "-prtout")) prtout = true;
        // if (checkArg(argc, argv, "-prterr")) prterr = true;
        // if (checkArg(argc, argv, "-prterr")) noprtref = true;
        // if (checkArg(argc, argv, "-csv")) csv = true;
        // if (checkArg(argc, argv, "-json")) json = true;
        // if (checkArg(argc, argv, "-perf")) perf = true;
        // if (checkArg(argc, argv, "-v")) verbose = true;
        // if (checkArg(argc, argv, "-notest")) notest = true;

        /* Setting arg in array of structs */
        // for (int i = 1; i < NUMBER_OF_SORTS; i++) 
        //     sorts[i].run = (bool)checkArg(argc, argv, sorts[i].name); 

        /* Filename / string */  
        // int arg_fname = checkArg(argc, argv, "-file");
        // *fname = arg_fname ? argv[arg_fname + 1] : "";
       
        int arg_aString = checkArg(argc, argv, "-as");
        // *aString = arg_aString ? argv[arg_aString + 1] : "";
        arg_aString ? strcpy(aString, argv[arg_aString + 1]) : strcpy(aString, "");

        // Numerical value args
        // int arg_tmax = checkArg(argc, argv, "-tmax");
        // tmax = arg_tmax ? strtol(argv[arg_tmax + 1], NULL, 10) : 100;
        int arg_size = checkArg(argc, argv, "-size");
        elements = arg_size ? strtol(argv[arg_size + 1], NULL, 10) : 200;
        // int arg_run = checkArg(argc, argv, "-run");
        // run_len = arg_run ? strtol(argv[arg_run + 1], NULL, 10) : 300;
        // int arg_thr = checkArg(argc, argv, "-thr");
        // threads = arg_thr ? strtol(argv[arg_thr + 1], NULL, 10) : 12;  // Default 12 threads
        
        /* It's complicated */
        // int arg_max = checkArg(argc, argv, "-max");
        // if (arg_max) {
        //     char* s = argv[arg_max + 1];
        //     if (!strcmp(s, "i8"))
        //         rnd_max = INT8_MAX;
        //     else if (!strcmp(s, "i16"))
        //         rnd_max = INT16_MAX;
        //     else if (!strcmp(s, "i32"))
        //         rnd_max = INT32_MAX;
        //     else
        //         rnd_max = strtol(argv[arg_max + 1], NULL, 10);           
        // }
    }

    // uint32_t* random = calloc(elements, sizeof(uint32_t));
    //     assert( ("Memory allocation failed.", random != NULL) );

    // Printing
    char buf1[0x400]; char buf2[0x400];
    puts("\t\tBoutique\t\tca.");
    tcp("-prtin", prtin ? "true" : "false", ca.prtin ? "true" : "false");
    sprintf(buf1, "%d", elements); sprintf(buf2, "%d", ca.size);  
    tcp("-size", buf1, buf2);
    // tcp("-file", fname, ca.fname);
    tcp("-as", aString, ca.aString);


    // Open file for writing results
    // FILE* fp; 
    // if (*fname != NULL) 
    //     fp = fopen(*fname, "a");
    // char jsonOut[0x400] = "";

}

// Test Color Print
void tcp(char title[], char str1[], char str2[]) {
    char buf[0x400];
    char fmt[] = "%10s   %10s   \e[1;32m%10s\e[m"; // green
    char errfmt[] = "%10s   %10s   \e[1;31m%10s\e[m"; // red
    if (strcmp(str1, str2))
        sprintf(buf, errfmt, title, str1, str2);
    else
        sprintf(buf, fmt, title, str1, str2);
    puts(buf);
    // return *buf;

    /* Terminal colors
            #define KNRM  "\x1B[0m"
            #define KRED  "\x1B[31m"
            #define KGRN  "\x1B[32m"
            #define KYEL  "\x1B[33m"
            #define KBLU  "\x1B[34m"
            #define KMAG  "\x1B[35m"
            #define KCYN  "\x1B[36m"
            #define KWHT  "\x1B[37m"
     */
}
