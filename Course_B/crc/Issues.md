###Issues
 Testa: 0, 12, 17, 34, 44 


##Win

##Deb
Read files (too many chars)                                    files.c


##Both
FIXED   64-bit rem blir avhuggen till 32                                binutils.c
Tomt -m (oavsett kommando)                     Segfault         cmdargs.c
Tomt -c (oavsett kommando) el outofrange       Segfault         cmdargs.c





Validation not ok with RefOut (/Refin)

Validation ok with 8n, 15n+ segfault:

joakim@Porcelina:/mnt/r/code/SV21POTR/Course_B/crc$ ./crc val -m AB -c 12 -s 0x87
CRC-16/CCITT   Poly:0X1021   Init:0XFFFF   NDI:Yes   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-16/CCITT; matching 0XE5CC
Checksum:               0
Segmentation fault (core dumped)

joakim@Porcelina:/mnt/r/code/SV21POTR/Course_B/crc$ ./crc val -m AB -c 12 -s 0xD2BB
CRC-16/CCITT   Poly:0X1021   Init:0XFFFF   NDI:Yes   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-16/CCITT; matching 0XE5CC
Checksum:               0
Segmentation fault (core dumped)

joakim@Porcelina:/mnt/r/code/SV21POTR/Course_B/crc$ ./crc val -m AB -c 34 -s 0X30694C07
CRC-32   Poly:0X4C11DB7   Init:0XFFFFFFFF   NDI:No   XorOut:0XFFFFFFFF   RefIn:Yes   RefOut:Yes
Passed check value-test for CRC-32; matching 0XCBF43926
Checksum:               0
Segmentation fault (core dumped)
joakim@Porcelina:/mnt/r/code/SV21POTR/Course_B/crc$



##OK deb


##OK both
Perf + Validate 8-bit 
joakim@Porcelina:/mnt/r/code/SV21POTR/Course_B/crc$ ./crc perf -c 0
CRC-8   Poly:0X7   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-8; matching 0XF4
  Encode: 65536 chars in 0.016 seconds, 4.000 MiB/s.
Validate: 65536 chars in 0.000 seconds,   inf MiB/s. Passed.
  Encode: 1048576 chars in 0.062 seconds, 16.000 MiB/s.
Validate: 1048576 chars in 0.078 seconds, 12.800 MiB/s. Passed.
  Encode: 16777216 chars in 1.016 seconds, 15.754 MiB/s.
Validate: 16777216 chars in 0.984 seconds, 16.254 MiB/s. Passed.

Ej validate 15n+
joakim@Porcelina:/mnt/r/code/SV21POTR/Course_B/crc$ ./crc perf -c 10
CRC-15/CAN   Poly:0XC599   Init:No   NDI:No   XorOut:No   RefIn:Yes   RefOut:No
Passed check value-test for CRC-15/CAN; matching 0X5136
  Encode: 65536 chars in 0.016 seconds, 4.000 MiB/s.
Segmentation fault (core dumped)

// Perf encode only, alla n, testat upp till 134 MiB
./crc perf -c 38
CRC-32/XFER   Poly:0XAF   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-32/XFER; matching 0XBD0BE338
  Encode: 65536 chars in 0.000 seconds,   inf MiB/s.
  Encode: 1048576 chars in 0.109 seconds, 9.143 MiB/s.
  Encode: 16777216 chars in 1.719 seconds, 9.309 MiB/s.
  Encode: 67108864 chars in 6.859 seconds, 9.330 MiB/s.
  Encode: 134217728 chars in 13.625 seconds, 9.394 MiB/s.

PS R:\code\SV21POTR\Course_B\crc> .\crc perf -c 38
CRC-32/XFER   Poly:0XAF   Init:No   NDI:No   XorOut:No   RefIn:No   RefOut:No
Passed check value-test for CRC-32/XFER; matching 0XBD0BE338
  Encode: 65536 chars in 0.006 seconds, 10.417 MiB/s.
  Encode: 1048576 chars in 0.098 seconds, 10.204 MiB/s.
  Encode: 16777216 chars in 1.599 seconds, 10.006 MiB/s.
  Encode: 134217728 chars in 12.888 seconds, 9.932 MiB/s.





Changes; 32 to 64:
uint32_t bits2intMSF(size_t const len, uint8_t* bits);
uint32_t bits2intLSF(size_t const len, uint8_t* bits);
<!-- typedef uint32_t (*bits2int_t)(size_t const len, uint8_t* bits); -->

Båda 32:
   34           CRC-32          0X4C11DB7         0XFFFFFFFF              0XFFFFFFFF    X     X   Passed 0XCBF43926
   35     CRC-32/BZIP2          0X4C11DB7         0XFFFFFFFF              0XFFFFFFFF              Passed 0XFC891918
   36     CRC-32/POSIX          0X4C11DB7                                 0XFFFFFFFF              Passed 0X765E7680
   37          CRC-32Q         0X814141AB                                                         Passed 0X3010BF7F
   38      CRC-32/XFER               0XAF                                                         Passed 0XBD0BE338
   39          CRC-32C         0X1EDC6F41         0XFFFFFFFF              0XFFFFFFFF    X     X   Passed 0XE3069283
   40          CRC-32D         0XA833982B         0XFFFFFFFF              0XFFFFFFFF    X     X   Passed 0X87315576
   41    CRC-32/MPEG-2          0X4C11DB7         0XFFFFFFFF                                      Passed 0X376E6E7
   42    CRC-32/JAMCRC          0X4C11DB7         0XFFFFFFFF                            X     X   Passed 0X340BC6D9
   43       CRC-40/GSM          0X4820009                               0XFFFFFFFFFF              Failed 0X164FC646 != 0XD4164FC646
   44  CRC-64/ECMA-182 0X42F0E1EBA9EA3693                                                         Failed 0XB497347 != 0X6C40DF5F0B497347
   45    CRC-64/GO-ISO               0X1B 0XFFFFFFFFFFFFFFFF      0XFFFFFFFFFFFFFFFF    X     X   Failed 0XB90956C7 != 0XB90956C775A41001
   46        CRC-64/XZ 0X42F0E1EBA9EA3693 0XFFFFFFFFFFFFFFFF      0XFFFFFFFFFFFFFFFF    X     X   Failed 0X995DC9BB != 0X995DC9BBDF1939FA

MSF 64:
   35     CRC-32/BZIP2          0X4C11DB7         0XFFFFFFFF              0XFFFFFFFF              Failed 0XFFFFFFFFFC891918 != 0XFC891918
   36     CRC-32/POSIX          0X4C11DB7                                 0XFFFFFFFF              Passed 0X765E7680
   37          CRC-32Q         0X814141AB                                                         Passed 0X3010BF7F
   38      CRC-32/XFER               0XAF                                                         Failed 0XFFFFFFFFBD0BE338 != 0XBD0BE338
   39          CRC-32C         0X1EDC6F41         0XFFFFFFFF              0XFFFFFFFF    X     X   Passed 0XE3069283
   40          CRC-32D         0XA833982B         0XFFFFFFFF              0XFFFFFFFF    X     X   Passed 0X87315576
   41    CRC-32/MPEG-2          0X4C11DB7         0XFFFFFFFF                                      Passed 0X376E6E7
   42    CRC-32/JAMCRC          0X4C11DB7         0XFFFFFFFF                            X     X   Passed 0X340BC6D9
   43       CRC-40/GSM          0X4820009                               0XFFFFFFFFFF              Failed 0X164FC646 != 0XD4164FC646
   44  CRC-64/ECMA-182 0X42F0E1EBA9EA3693                                                         Failed 0XB497347 != 0X6C40DF5F0B497347
   45    CRC-64/GO-ISO               0X1B 0XFFFFFFFFFFFFFFFF      0XFFFFFFFFFFFFFFFF    X     X   Failed 0XB90956C7 != 0XB90956C775A41001
   46        CRC-64/XZ 0X42F0E1EBA9EA3693 0XFFFFFFFFFFFFFFFF      0XFFFFFFFFFFFFFFFF    X     X   Failed 0X995DC9BB != 0X995DC9BBDF1939FA

LSF 64:
   34           CRC-32          0X4C11DB7         0XFFFFFFFF              0XFFFFFFFF    X     X   Failed 0XFFFFFFFFCBF43926 != 0XCBF43926
   35     CRC-32/BZIP2          0X4C11DB7         0XFFFFFFFF              0XFFFFFFFF              Passed 0XFC891918
   36     CRC-32/POSIX          0X4C11DB7                                 0XFFFFFFFF              Passed 0X765E7680
   37          CRC-32Q         0X814141AB                                                         Passed 0X3010BF7F
   38      CRC-32/XFER               0XAF                                                         Passed 0XBD0BE338
   39          CRC-32C         0X1EDC6F41         0XFFFFFFFF              0XFFFFFFFF    X     X   Failed 0XFFFFFFFFE3069283 != 0XE3069283
   40          CRC-32D         0XA833982B         0XFFFFFFFF              0XFFFFFFFF    X     X   Failed 0XFFFFFFFF87315576 != 0X87315576
   41    CRC-32/MPEG-2          0X4C11DB7         0XFFFFFFFF                                      Passed 0X376E6E7
   42    CRC-32/JAMCRC          0X4C11DB7         0XFFFFFFFF                            X     X   Passed 0X340BC6D9
   43       CRC-40/GSM          0X4820009                               0XFFFFFFFFFF              Failed 0X164FC646 != 0XD4164FC646
   44  CRC-64/ECMA-182 0X42F0E1EBA9EA3693                                                         Failed 0XB497347 != 0X6C40DF5F0B497347
   45    CRC-64/GO-ISO               0X1B 0XFFFFFFFFFFFFFFFF      0XFFFFFFFFFFFFFFFF    X     X   Failed 0XFFFFFFFFB90956C7 != 0XB90956C775A41001
   46        CRC-64/XZ 0X42F0E1EBA9EA3693 0XFFFFFFFFFFFFFFFF      0XFFFFFFFFFFFFFFFF    X     X   Failed 0XFFFFFFFF995DC9BB != 0X995DC9BBDF1939FA

Båda 64:
   34           CRC-32          0X4C11DB7         0XFFFFFFFF              0XFFFFFFFF    X     X   Failed 0XFFFFFFFFCBF43926 != 0XCBF43926               NOK: Init, Xor, RI, RO
   35     CRC-32/BZIP2          0X4C11DB7         0XFFFFFFFF              0XFFFFFFFF              Failed 0XFFFFFFFFFC891918 != 0XFC891918               NOK: Init, Xor 
   36     CRC-32/POSIX          0X4C11DB7                                 0XFFFFFFFF              Passed 0X765E7680                                      OK:       Xor
   37          CRC-32Q         0X814141AB                                                         Passed 0X3010BF7F                                      OK:      
   38      CRC-32/XFER               0XAF                                                         Failed 0XFFFFFFFFBD0BE338 != 0XBD0BE338               NOK:   Short gen
   39          CRC-32C         0X1EDC6F41         0XFFFFFFFF              0XFFFFFFFF    X     X   Failed 0XFFFFFFFFE3069283 != 0XE3069283               NOK: Init, Xor, RI, RO
   40          CRC-32D         0XA833982B         0XFFFFFFFF              0XFFFFFFFF    X     X   Failed 0XFFFFFFFF87315576 != 0X87315576               NOK: Init, Xor, RI, RO
   41    CRC-32/MPEG-2          0X4C11DB7         0XFFFFFFFF                                      Passed 0X376E6E7                                       OK: Init
   42    CRC-32/JAMCRC          0X4C11DB7         0XFFFFFFFF                            X     X   Passed 0X340BC6D9                                      OK: Init,      RI, RO
   43       CRC-40/GSM          0X4820009                               0XFFFFFFFFFF              Failed 0X164FC646 != 0XD4164FC646                     NOK:       Xor           40 som ovan
   44  CRC-64/ECMA-182 0X42F0E1EBA9EA3693                                                         Failed 0XB497347 != 0X6C40DF5F0B497347                NOK: Init, Xor, RI, RO,  64 som ovan 
   45    CRC-64/GO-ISO               0X1B 0XFFFFFFFFFFFFFFFF      0XFFFFFFFFFFFFFFFF    X     X   Failed 0XFFFFFFFFB90956C7 != 0XB90956C775A41001       NOK: Init, Xor, RI, RO   64 nu med 8 F
   46        CRC-64/XZ 0X42F0E1EBA9EA3693 0XFFFFFFFFFFFFFFFF      0XFFFFFFFFFFFFFFFF    X     X   Failed 0XFFFFFFFF995DC9BB != 0X995DC9BBDF1939FA       NOK: Init, Xor, RI, RO   64 nu med 8 F

