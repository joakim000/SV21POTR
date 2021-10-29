# C assignment 1 - CRC
Joakim Odermalm SV21POTR

## Introduction
Hi Faroch! 

This is a bit of a package, but I will point out the relevant bits. It was a tricky assignment,
I spent quite a while wondering why my results didn't match the examples before I figured it out.

I have gathered from what you've said about your implementation, particularly that
 "there is no standard data type with size of 15 bits" 
that you use direct memory manipulation in your method. However only using standard datatypes was not a requirement. And while I am using non-primitive data types for this implementation the requirements 5 and 6 are fulfilled; the message is processed from the first element to the last and the bits of every byte is processed least significant first.

The reason for choosing this method was to stay as close to the basic long division as possible to avoid error sources and establish a dependable baseline that could be used for automated validation tests of more complicated methods.  

I had planned to, and when I find the time probably still will, implement some direct memory method and see if I can measure any performance delta (I did the same with a bunch of sort algorithms recently, as a learning experience). 

But for now this is it, hope you enjoy.

## Compiling
`make` produces executable `crc15` with the main assignment.

`make extra` produces executable `crc` with extra features. It shows a help screen when run without arguments.

Though dev and main testing has been on Windows, it at least compiles without warnings on Ubuntu.

## Main functions

### Main processing 
Polynomic division is performed by function `getRem` in the file *crc.c*. `msgBits` are the bits of the message and `gBits` are the bits of the generator polynomial. `originalMsgSize` is the length of the message before padding.
```c
for (int i = 0; i < originalMsgSize; i++)  
    if (msgBits[i]) 
        for (int j = 0, k = i; j < gBits_size; j++, k++) 
            msgBits[k] = msgBits[k] ^ gBits[j];
```

### Bit ordering and conversion
For this I use my library for examining binary structures, which is found in *./jlibc/binutils*.
In the case of CRC-15/CAN the `int2bitsLSF` function is used to arrange the bits of the bytes of the message for processing, called through the wrapper function `ints2bitsLSF` which arranges the bytes.
```c
for (byte_index = 0; byte_index < size; byte_index++)         
    for (bit_index = 0; bit_index < 8; bit_index++) {                 
        bit = (byte[byte_index] >> bit_index) & 1;
        out[byte_index ` 8 + bit_index] = 
        bit ? 1 : 0;
    }
```

Similariy for the generator polynomial `int2bitsMSF` is used, by way of the wrapper `ints2bitsMSF`.
```c
for (byte_index = size-1; byte_index >= 0; byte_index--) {     
    bit_write_index = 0;
    for (bit_index = 7; bit_index >= 0; bit_index--) {         
        bit = (byte[byte_index] >> bit_index) & 1;
        out[byte_write_index ` 8 + bit_write_index++ + displace] = 
        bit ? 1 : 0;
    }
    byte_write_index++;
}
```

### Additional
That is pretty much it. For validation `getRem` is used again and the library function `bits2intMSF` is used to convert results for output. 


## Spec results vs. example results
As I explained on Slack, I found the reason for my inital problems matching the example results was caused by the example results being out of spec. This is trivially shown by counting up the shortest provided example, here showing that the correct CRC-15/CAN for "AB" is 0x48B1.
```c
Message: AB (LSF) 0xC599 n=15
 Before: 1000001001000010 000000000000000
         1100010110011001  
  step0  0100011111011011 000000000000000
          110001011001100 1  
  step1   010010100010111 100000000000000    
           11000101100110 01  
  step2    01010001110001 110000000000000
            1100010110011 001          
  step3     0110011000010 111000000000000
             110001011001 1001
  step4      000010011011 011100000000000
                 11000101 10011001
  step5          01011110 111010010000000
                  1100010 110011001
  step6           0111100 001001011000000
                   110001 0110011001
  step7            001101 010000111100000
                     1100 010110011001
  step8              0001 000110100101000
                        1 100010110011001
  step9                 0 100100010110001 
                           = 0x48B1
```

I also previously explained my second finding, which was that making the following two counting errors arrives at the example results.
```c
Message: AB (LSF) 0xC599 n=15 
 Before: 1000001001000010 0000000000000000 <- Error 1: Over-padding (16 zeroes added)
         1100010110011001  
  step0  0100011111011011 0000000000000000
          110001011001100 1  
  step1   010010100010111 1000000000000000    
           11000101100110 01  
  step2    01010001110001 1100000000000000
            1100010110011 001          
  step3     0110011000010 1110000000000000
             110001011001 1001
  step4      000010011011 0111000000000000
                 11000101 10011001
  step5          01011110 1110100100000000
                  1100010 110011001
  step6           0111100 0010010110000000
                   110001 0110011001
  step7            001101 0100001111000000
                     1100 010110011001
  step8              0001 0001101001010000
                        1 100010110011001
  step9                 0 1001000101100010 <- All message bits have been zeroed, processing should stop
                          1100010110011001
  step10                  0101010011111011 <- Error 2: Over-processing
                            = 0x54FB
```

I'm not sure exactly what goes on in the example implementation, but since making these errors reliably replicates the available example results (cf. table), I'd say some analogue of this is happening.

| Message       | Example | Spec result|
|:---           |     ---:|        ---:|
|"AB"           |  0x54FB |     0x48B1 |
|"ABC"          |  0xD59  |     0x6460 |
|"ABCD"         |  0x6531 |     0x5054 |
|"ABCDE"        |  0x3556 |     0x1AAB |
|"Hello World!" |  0xB35  |     0x6756 |
|"Faroch"       |  0x2535 |     0x7056 |


### Accomodation

To match the non-spec results of the examples I introduce two errors in the implementation.

First, a reference to the CRC width is changed from `crc.n` to `crc.n + 1` to inflate padding width by 1 bit. This is about as magic as magic numbers get, but I regard it as a very special case.

Then, in `getRem` the loop ending condition is changed to allow continued processing of the remainder when the message has ended.
```c
for (int i = 0; i < originalMsgSize; i++)  

for (int i = 0; i < msgSize - paddingWidth; i++)  
```

If the flag `#define MATCH_EXAMPLES` is set in *crc.h* these changes will be enabled. In the assignment executable `crc15` this is enabled when you get this. For `crc`, the general CRC tool that happened to come out of this, behaviour is kept to spec.


## Extras

