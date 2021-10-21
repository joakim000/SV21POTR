/*  Tests for cmdargs

   gcc test2.c cmdargs.c -o ca
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdargs.h"



void tcp(char title[], char str1[], char str2[]);

int main(int argc, char* argv[] )
{
    /* Bunch of vars */
    bool aFlag1 = false;
    bool aFlag2 = false;
    int anInt1;
    int anInt2;
    char aString1[0x400];
    char aString2[0x400];

    /* Automated input */
    // char* fakeargs[] = {"exe", "-prtin", "-size", "66", "-file", "file.txt"}; 
    char* fakeargs[] = {"exe", "-flag1", "-int1", "1001", "-as1", "this is string 1"}; 
    if (argc < 2) {
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
        bool aFlag1;
        bool aFlag2; 
        int  anInt1;
        int  anInt2;
        char aString1[0x400];
        char aString2[0x400];
    } ca 
    = {
        // aFlag1 = true,
        // aFlag2 = false, 
        // anInt1 = 1001,
        // anInt2 = 1002,
        // aString1 = "string1",
        // aString2 = "string2"
    };

    
    argdef_t defs[] = {
        {
            .isFlag = true,
            .var = &ca.aFlag1,
            .str = "-flag1" 
        },
        {
            .isInt = true,
            .var = &ca.anInt1,
            .str = "-int1",
            .defaultInt = 101
        },
        {
            .isString = true,
            .var = &ca.aString1,
            .str = "-as1"
            // .defaultString = ""
        },
         {
            .isFlag = true,
            .var = &ca.aFlag2,
            .str = "-flag2" 
        },
        {
            .isInt = true,
            .var = &ca.anInt2,
            .str = "-int2",
            // .defaultInt = 201
        },
        {
            .isString = true,
            .var = &ca.aString2,
            .str = "-as2",
            .defaultString = "kurredutt"
        }
    };

    argdef_t defs2[] = {
        { .isFlag = true, .var = &aFlag1, .str = "-flag1" },
        { .isInt = true,  .var = &anInt1, .str = "-int1", .defaultInt = 101 },
        { .var = &aString1, .str = "-as1",.isString = true  /*, .defaultString = "" */ },
        { .var = &aFlag2, .str = "-flag2", .isFlag = true },
        { .isInt = true, .var = &anInt2, .str = "-int2",
            // .defaultInt = 201
        },
        {
            .isString = true,
            .var = &aString2,
            .str = "-as2"
            // .defaultString = ""
        }
    };

    
    /* Call cmdargs */
    if (argc >= 2) {
        printf("count of defs: %d\n", COUNT_OF(defs));
        processArgs(argv, argc, defs, COUNT_OF(defs));

        processArgs(argv, argc, defs2, COUNT_OF(defs2));

        /* basic result printing */
        // printf("ca.prtin          -prtin %16d\t%p\n", ca.prtin,      &ca.prtin); 
        // printf("defs[0].var       -prtin %16p\t%p\n", defs[0].var,   &defs[0].var);

        // printf("ca.size           -size  %16d\t%p\n", ca.size,       &ca.size);
        // printf("defs[1].var       -size  %16p\t%p\n", defs[1].var,   &defs[1].var);

        // printf("ca.aString        -as    %16s\t%p\n", ca.aString,    &ca.aString);
        // printf("defs[2].var       -as    %16p\t%p\n", defs[2].var,   &defs[2].var);  
      
    }

     /* Boutique solution from another proj, for comparisons */
    if (argc >= 2) {
        int argvIndex; 

        // // Bool args
        // aFlag1 = checkArg(argc, argv, "-flag1") ? true : false;
        // aFlag2 = checkArg(argc, argv, "-flag2") ? true : false;

        // // Numerical value args
        // argvIndex = checkArg(argc, argv, "-int1");
        // if (argvIndex) anInt1 = strtol(argv[argvIndex + 1], NULL, 10); // TODO errorcheck
        // // anInt1 = arg_size ? strtol(argv[arg_size + 1], NULL, 10) : 101;
        // argvIndex = checkArg(argc, argv, "-int2");
        // if (argvIndex) anInt2 = strtol(argv[argvIndex + 1], NULL, 10); // TODO errorcheck

        // // String args
        // argvIndex = checkArg(argc, argv, "-as1");
        // argvIndex ? strcpy(aString1, argv[argvIndex + 1]) : strcpy(aString1, "not found");
        // argvIndex = checkArg(argc, argv, "-as2");
        // argvIndex ? strcpy(aString2, argv[argvIndex + 1]) : strcpy(aString2, "not found");


         /* Setting arg in array of structs */
        // for (int i = 1; i < NUMBER_OF_SORTS; i++) 
        //     sorts[i].run = (bool)checkArg(argc, argv, sorts[i].name); 

        /* Filename / string */  
        // int arg_fname = checkArg(argc, argv, "-file");
        // *fname = arg_fname ? argv[arg_fname + 1] : "";
       

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
    tcp("-flag1", aFlag1 ? "true" : "false", ca.aFlag1 ? "true" : "false");
    tcp("-flag2", aFlag2 ? "true" : "false", ca.aFlag2 ? "true" : "false");

    sprintf(buf1, "%d", anInt1); sprintf(buf2, "%d", ca.anInt1);  
    tcp("-int1", buf1, buf2);
    sprintf(buf1, "%d", anInt2); sprintf(buf2, "%d", ca.anInt2);  
    tcp("-int2", buf1, buf2);

    tcp("-as1", aString1, ca.aString1);
    tcp("-as2", aString2, ca.aString2);

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
