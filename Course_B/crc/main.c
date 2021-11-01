

#include "crc.h"

void main(int argc, char* argv[] )
{
    // Data structures
    crc_t* crc;  
    msg_t* msg;

    /* Setup */
    #include "crc_zoo.c" // Unconventional use of #include, just a convenient way to put the CRC definitions in a separate file.
    // Program settings and variables
    prog_t new_prog = {
        .verbose = VERBOSE,
        .printMsg = PRINTMSG,
        .printSteps = PRINTSTEPS,
        .printStepsGen = PRINTSTEPSGEN,
        .selfTest = SELFTEST,
    };
    prog = &new_prog;    
    // Command line arguments
    struct benchargs {
        bool zoo, enc, validate,                // Command
             printSteps, verbose, timing,    // Flags
             refIn, refOut;                  // Custom spec
        int checksum, 
            crc_spec, n, g, init, xor; // Custom spec
        char* msg[MAX_MESSAGE_ARGLENGTH];
        char* inFile[FILENAME_MAX];
        char* outFile[FILENAME_MAX];
    } ca;
    if (argc < 2 || checkArg(argc, argv, "help")) {
        // Print help if no args or help command
        printf(HELPTEXT1, NULL);
        exit(EXIT_SUCCESS);
    }
    argdef_t defs[] = {
        // Commands
        { .isFlag = true, .var = (bool*)&ca.zoo, .str = "zoo" },                     // zoo
        { .isFlag = true, .var = (bool*)&ca.enc, .str = "enc" },                     // encode
        { .isFlag = true, .var = (bool*)&ca.validate, .str = "val" },                   // validate
        // Input
        { .isInt = true,  .var = (int*)&ca.crc_spec, .str = "-s", .defaultString = 0 },                // CRC spec index
        { .isString = true, .var = (char*)&ca.msg, .str = "-m", .defaultString = "" },               // message
        { .isInt = true,  .var = (int*)&ca.checksum, .str = "-c", .defaultInt = 0 },                       // checksum for validation
        { .isString = true, .var = (char*)&ca.inFile, .str = "-in", .defaultString = "" },    // input file
        { .isString = true, .var = (char*)&ca.outFile, .str = "-out", .defaultString = "" }, // output file
        // Flags
        { .isFlag = true, .var = (bool*)&ca.printSteps, .str = "-steps" },                 // print steps
        { .isFlag = true, .var = (bool*)&ca.verbose, .str = "-v" },                       // verbose
        { .isFlag = true, .var = (bool*)&ca.timing, .str = "-t" },                       // timing
        // Custom spec
        { .isInt = true,  .var = (int*)&ca.n, .str = "-n", .defaultInt = 8 },    // bit width
        { .isInt = true,  .var = (int*)&ca.g, .str = "-g", .defaultInt = 0x07 }, // generator polynomial
        { .isInt = true,  .var = (int*)&ca.init, .str = "-i", .defaultInt = 0 }, // init seed
        { .isInt = true,  .var = (int*)&ca.xor, .str = "-x", .defaultInt = 0 },  // final xor
        { .isFlag = true, .var = (bool*)&ca.refIn, .str = "-ri" },               // inRef
        { .isFlag = true, .var = (bool*)&ca.refOut, .str = "-ro" },              // outRef

    };
    processArgs(argv, argc, defs, COUNT_OF(defs));
    // Set flags
    if (ca.printSteps) PROG.printSteps = true;
    if (ca.verbose) PROG.verbose = true;
    if (ca.timing) PROG.timing = true;
    // Check for a known command
    if (!ca.zoo && !ca.enc && !ca.validate) {
        printf("Available commands:\n\tzoo\tWhere all the CRCs live\n\tenc\tEncode a message\n"
               "\tval\tValidate a message\n\thelp\tMore help\n", NULL);
        exit(EXIT_SUCCESS);
    }
    // Command line arguments end


    // Commmand: Show CRC inventory
    if (ca.zoo) {
        zooTour(zoo, COUNT_OF(zoo));
        exit(EXIT_SUCCESS);
    }

    // Read message for encode / validate
    char* message;
    // In command line?
    if (strlen((char*)ca.msg) > 0 ) 
        message = (char*)ca.msg;
    // Else in file?
    else if (strlen((char*)ca.inFile) > 0) {
        // char fcontent[MAXMESSAGELENGTH] = "";
        char* fcontent = calloc(MAX_MESSAGE_READLENGTH, sizeof(char));
        assert( ("Memory allocation failed.", fcontent != NULL) );
        FILE* fp; 
        char buf[0x400];
        size_t elementsRead;
        fp = fopen((char*)ca.inFile, "r");
        if (fp != NULL) {
            printf("Reading file %s  ...  ", ca.inFile);
            while ((elementsRead = fread(buf, 1, sizeof buf, fp)) > 0)
                strcat(fcontent, buf);
            if (ferror(fp)) {
                PRINTERR("File read error, exiting.")
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            fclose(fp);
            strcat(fcontent, "\0");
            printf("%d characters read.\n", strlen(fcontent));
            message = fcontent;      fcontent = NULL;
        }
        else {
            PRINTERR("File not found.")
        }
    }
    // Still no message?
    if (strlen(message) < 1 ) {
        PRINTERR("No message, exiting.");
        exit(EXIT_FAILURE);
    }

    // Command: Encode
    if (ca.enc) {      
        // Load CRC spec 
        crc_t enc_crc;
        crc = &enc_crc;
        loadSpec(zoo, ca.crc_spec, &enc_crc, false); 

        // Prepare message 
        msg_t encode_msg = {
            .msgStr =         message,
            .len =            strlen(message),
            .originalBitLen = strlen(message) * BITSINBYTE,
            .paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + SPECIALWIDTH,     // Special
            // .paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + crc->n,       // Normal
        }; msg = &encode_msg;
  
        uint8_t new_msgBits[msg->paddedBitLen];     msg->msgBits = new_msgBits;
        // msg->msgBits = calloc(sizeof *message * BITSINBYTE, sizeof(uint8_t));
        // assert( ("Memory allocation failed.", msg->msgBits != NULL) );

        // Expected checksum value for testing checksum calculation. Skips check when set to 0.
        msg->expected = strcmp(msg->msgStr, "AB") ? 0 : crc->checkAB; 
        // uint32_t expected = 0;     // Testing use       

        timer_start = clock();
            // Arrange message
            arrangeMsg(crc, msg);
            // Calculate remainder
            msg->res = getRem(msg->msgBits, msg->paddedBitLen, msg->originalBitLen, crc);
        timer_end = clock();

        // Printing 
        if (PRINTMSG) {
            if (msg->len < PRINTLIMIT)
                printf("Message:\t%s\n", msg->msgStr);
            else
                printf("Message:\t[%d characters]\n", msg->len);
        }
        printf("Checksum:\t%#X\n", msg->res);
        if (ca.timing) printf("%d chars in %5.3f seconds.\n", msg->len, TIMING(timer_start, timer_end));

        // Compare result with a expected value, for debugging purposes
        if (msg->expected && msg->res != msg->expected) {
            printf("Expected:\t%#X\n", msg->expected);
            if (PROG.verbose) {                                   // Print bits of result and expected for analysis
                uint8_t checksumBits[sizeof(msg->res) * 8];
                int2bits(sizeof(msg->res), &msg->res, checksumBits, false);
                printBits("Checksum", checksumBits, COUNT_OF(checksumBits), crc->n);
                uint8_t expectedBits[sizeof(msg->expected) * 8];
                int2bits(sizeof(msg->expected), &msg->expected, expectedBits, false);
                printBits("Expected", expectedBits, COUNT_OF(expectedBits), crc->n);
            }
            printf("\e[1;31m%s\e[m\n", "Checksum does not match expected value."); // red
        }
        
        // Open file for writing result      
        char csStr[100];
        sprintf(csStr, "[%#X]", msg->res); 
        char outStr[strlen(msg->msgStr) + strlen(csStr)];
        sprintf(outStr, "%s%s", csStr, msg->msgStr); 
        // puts(outStr);
        if (ca.outFile != NULL) {
            FILE* fp; 
            fp = fopen((char*)ca.outFile, "w");
            if (fp != NULL) {
                fprintf(fp, outStr);
                fclose(fp);
            }
        }
        free(msg->msgStr);
        free(msg->msgBits);
        exit(EXIT_SUCCESS);
    }
  
    // Command: Validate
    if (ca.validate) {     
        // Check for available checksum:
        uint32_t checksum;
        // In message?
        char checksumStr[0x20] = "";
        char* remaining;
        char** end;
        remaining = strchr(message, '[');
        if (remaining) {
            checksum = strtol(remaining + 1, end, 16); // 0 if no valid conversion
        }
        if (checksum > 0) {
            if (PROG.verbose) printf("Checksum in message: %#X\n", checksum);
            // Remove from message-string
            remaining = strchr(message, ']');
            if (remaining) {
                message = remaining + 1;
            if (PROG.verbose) printf("Remaining message:%s\n", message);
            }
            else {
                // No ending ], where does message begin? Better to use end from strtol.
            }
        }        
        // In command line?
        else if (ca.checksum > 0) 
            checksum = ca.checksum;
        else {
            PRINTERR("No checksum for validation, exiting.");   
            exit(EXIT_FAILURE);
        }
        
        // Load CRC spec 
        crc_t new_crc;
        crc = &new_crc;
        loadSpec(zoo, ca.crc_spec, &new_crc, false); 

        // Prepare message 
        msg_t valid_msg = {
             .msgStr =         message,
             .len =            strlen(message),
             .originalBitLen = strlen(message) * BITSINBYTE,
             .paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + SPECIALWIDTH,     // Special
            //  .paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + crc->n,       // Normal
            .res = checksum
        }; msg = &valid_msg;
        
        uint8_t new_msgBits[msg->paddedBitLen];     msg->msgBits = new_msgBits;

        // Arrange message
        arrangeMsg(crc, msg);

        // if (PROG.verbose) 
        printf("Checksum:\t\t%#X\n", msg->res);

        /* Checksum the messsage. I.e replace the zeros with the CRC accroding to the requirements. */
        uint8_t new_csmsgBits[msg->paddedBitLen];
        msg->csmsgBits = new_csmsgBits;
        memcpy(msg->csmsgBits, msg->msgBits, msg->paddedBitLen);
        checksumMsg(msg->paddedBitLen, msg->res, SPECIALWIDTH, msg->csmsgBits);    // Special
        // checksumMsg(msg->paddedBitLen, msg->res, crc->n, msg->csmsgBits);    // Normal
        
        /* Validate the messsage.
        If the remainder is zero print "The data is OK\n"; otherwise print "The data is not OK\n" */
        bool valid;
        valid = validate(msg->csmsgBits, msg->paddedBitLen, msg->originalBitLen, crc);
        
        // Print result        
        validPrint(msg->msgStr, msg->len, valid);
    
        free(msg->msgStr);
        free(msg->msgBits);
        exit(EXIT_SUCCESS);
    }

}
