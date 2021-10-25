/*************************************
 * Utils for working with bit values
 * 
 * @author Joakim Odermalm
 ************************************/

/**
 @brief Convert a single int to array of bit values 
 
 @note Variants for Most / Least Significant First 

 @example
    size_t type_size = sizeof(anInt);
    uint8_t bitArray[type_size * 8];
    int2bitsXSF(type_size, &testval, bitArray);
*/
void int2bitsLSF(size_t const size, void const * const ptr, uint8_t out[]);
void int2bitsMSF(size_t const size, void const * const ptr, uint8_t out[]);


/**
 @brief Convert an array of bit values to a single int 
 
 @note Variants for Most / Least Significant First 

 @return Converted int as uint32_t 

 @example
    // bitArray holds array of bit values
    // size is length of bitArray
    bits2intXSF(size, bitArray));
*/
uint32_t bits2intLSF(size_t const len, uint8_t* bits);
uint32_t bits2intMSF(size_t const len, uint8_t* bits);


/**
 @brief Convert an array of ints to a concatenated array of bit values 
 
 @note Variants for Most / Least Significant First 

 @example
    uint32_t message[] = {1, 2, 3};
    size_t type_size = sizeof(message[0]); 
    size_t msg_size = (sizeof(message));
    uint8_t messageBits[sizeof(message) * 8];

    ints2bitsXSF(msg_size, type_size, &message, messageBits);
*/
void ints2bitsLSF(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[]);
void ints2bitsMSF(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[]);

/**
 @brief Convert an array bit values to array of ints 
 
 @note Variants for Most / Least Significant First 

 @example
    size_t total_bits = COUNT_OF(bitValues);        // Length of bit value array
    size_t type_size = sizeof(uint32_t);            // Expected type size
    uint32_t nums[total_bits / type_size * 8];      // Count of nums based on expected type size
    bits2intsXSF(total_bits, type_size, bitValues, nums);
*/
void bits2intsLSF(size_t const size, size_t const type_size, uint8_t const bits[], uint32_t out[]);
void bits2intsMSF(size_t const size, size_t const type_size, uint8_t const bits[], uint32_t out[]);

/**
  @brief Function pointer types for calling from MSF/LSF generalized functions  
 */
typedef void (*int2bits_ptr_t)(size_t const size, void const * const ptr, uint8_t out[]); 
typedef uint32_t (*bits2int_ptr_t)(size_t const len, uint8_t* bits);
typedef void (*ints2bits_ptr_t)(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[]); 
typedef void (*bits2ints_ptr_t)(size_t const total_bits, size_t const type_size, uint8_t const bits[], uint32_t out[]);

/**
  @brief Convert array of chars to null-terminated string  
*/
void charArrayToString(char ca[], size_t size, char* out);

/**
  @brief Various (failed) experiments for reducing memory usage 
*/
typedef union byte_union {
    struct byte_struct
    {
        int b0 : 1;
        int b1 : 1;
        int b2 : 1;
        int b3 : 1;
        int b4 : 1;
        int b5 : 1;
        int b6 : 1;
        int b7 : 1;
    } bit;
    int get[8];
} byte;

 struct byte_struct2
 {
     int b0 : 1;
     int b1 : 1;
     int b2 : 1;
     int b3 : 1;
     int b4 : 1;
     int b5 : 1;
     int b6 : 1;
     int b7 : 1;
     int* next;
 } bit2;
