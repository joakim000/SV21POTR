#include "crc.h"
#define CRC15CAN_INDEX 10
#define ENCODE true
#define VALIDATE true

void main(int argc, char* argv[] )
{
    // Message
    char message[0x10] = "Hello World!";
    // char message[0x10] = "AB";

    // Assignment requirements for message length
    messageLengthCheck(strlen(message));

    // Data structures
    crc_t* crc;  
    msg_t* msg;
    #include "crc_zoo.c" // Unconventional use of #include, just a convenient way to put the CRC definitions in a separate file. Better as a constant perhaps.

    // Program settings and variables
    prog_t new_prog = {
        .verbose = VERBOSE,
        .printMsg = PRINTMSG,
        .printSteps = PRINTSTEPS,
        .printStepsGen = PRINTSTEPSGEN,
    };
    prog = &new_prog;    

    //  Encode
    if (ENCODE) {
        // Load CRC spec 
        crc_t enc_crc;
        crc = &enc_crc;
        LoadDefWrapper(zoo, CRC15CAN_INDEX, crc, false); 

        // Prepare message 
        msg_t encode_msg = {
            .msgStr =         message,
            .len =            strlen(message),
            .msgBitLen = strlen(message) * BITSINBYTE,
            .paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + SPECIALWIDTH,     // Special
            // .paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + crc->n,       // Normal
        }; msg = &encode_msg;
        STR2ARR(message, new_msg_arr);               msg->msg = new_msg_arr;
        uint8_t new_msgBits[msg->paddedBitLen];      msg->msgBits = new_msgBits;

        // Arrange message
        ArrangeMsg(crc, msg);

        // Print original message 
        puts("\tEncode");
        if (PRINTMSG) printf("Message:\t\t%s\n", msg->msgStr);

        /* Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB */
        msg->rem = PolyDivision(msg->msgBits, msg->paddedBitLen, msg->msgBitLen, crc);
        printf("Calculated checksum:\t%#X\n", msg->rem);
    }

    // Redoing validation after message change, for demo purpose
    int loops = 0;
    validate_again:
    loops++;

    //  Validate
    if (VALIDATE) {

        // Prepare and arrange message
        msg->msgStr = message;
        STR2ARR(message, new_msg_arr);              msg->msg = new_msg_arr;
        uint8_t new_msgBits[msg->paddedBitLen];     msg->msgBits = new_msgBits;

         // Arrange message
        ArrangeMsg(crc, msg);

        /* Checksum the messsage. I.e replace the zeros with the CRC accroding to the requirements. */
        uint8_t new_csmsgBits[msg->paddedBitLen];
        msg->csmsgBits = new_csmsgBits;
        memcpy(msg->csmsgBits, msg->msgBits, msg->paddedBitLen);
        checksumMsg(msg->paddedBitLen, msg->rem, SPECIALWIDTH, msg->csmsgBits);    // Special
        // checksumMsg(msg->paddedBitLen, msg->rem, crc->n, msg->csmsgBits);          // Normal

        /* Validate the messsage.
        If the remainder is zero print "The data is OK\n"; otherwise print "The data is not OK\n" */
        bool valid;
        valid = validate(msg->csmsgBits, msg->paddedBitLen, msg->msgBitLen, crc);
        
        // Print result        
        puts("\tValidate");
        printf("Validation checksum:\t%#X\n", msg->rem);
        ValidPrint(msg->msg, msg->len, valid);
    }

    // Redo validation with changed message as per assignment
    if (loops < 2) {
        message[1] = 'a';
        goto validate_again;
    }

}
