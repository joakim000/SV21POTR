#include <stdint.h>

// Add special params for sort functions here
struct sort_args {
    uint32_t* num;          // Standard
    uint32_t size;          // Standard
    
    uint16_t maxThreads;    // For threaded sorts 
    int shellSeq[];         // Gap sequence for shell sort
};

// Definition of a sort
typedef struct sort_holder {
    char* name;
    char* print_name;
    bool default_run;
    void (*sort_ptr)(void* args); // ptr to sort function
    bool run;
} sort;