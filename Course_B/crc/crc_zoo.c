// Sources: https://crccalc.com/                                 8-32 bit
//          http://srecord.sourceforge.net/crc16-ccitt.html      CRC-16/CCITT 
//          https://reveng.sourceforge.io/crc-catalogue/all.htm  40-64 bit, residue 

crcdef_t zoo[] = {
    // Name                0 Width  1 "AB"     2 123456789  3 Generator  4 Init    5 RefIn  6 RefOut 7 XorOut      8 IL1  9 Nondirect_init  10 Residue
    { "CRC-8",             { 8,     0x87,        0xF4,        0x07,        0x00,         0,      0,     0x00,          1  }}, //  0 
    { "CRC-8/CDMA2000",    { 8,     0x18,        0xDA,        0x9B,        0xFF,         0,      0,     0x00,          1  }}, //  1 
    { "CRC-8/DARC",        { 8,     0x28,        0x15,        0x39,        0x00,         1,      1,     0x00,          1  }}, //  2 
    { "CRC-8/DVB-S2",      { 8,     0x56,        0xBC,        0xD5,        0x00,         0,      0,     0x00,          1  }}, //  3 
    { "CRC-8/EBU",         { 8,     0x9C,        0x97,        0x1D,        0xFF,         1,      1,     0x00,          1  }}, //  4 
    { "CRC-8/I-CODE",      { 8,     0x56,        0x7E,        0x1D,        0xFD,         0,      0,     0x00,          1  }}, //  5 
    { "CRC-8/ITU",         { 8,     0xD2,        0xA1,        0x07,        0x00,         0,      0,     0x55,          1  }}, //  6 
    { "CRC-8/MAXIM",       { 8,     0xA5,        0xA1,        0x31,        0x00,         1,      1,     0x00,          1  }}, //  7 
    { "CRC-8/ROHC",        { 8,     0x41,        0xD0,        0x07,        0xFF,         1,      1,     0x00,          1  }}, //  8 
    { "CRC-8/WCDMA",       { 8,     0x66,        0x25,        0x9B,        0x00,         1,      1,     0x00,          1  }}, //  9 
    { "CRC-15/CAN",        { 15,    0x48B1,      0x5136,      0xC599,      0x00,         1,      0,     0x0000,        0  }}, // 10 
    { "CRC-16/CCITT",      { 16,    0x0,         0xE5CC,      0x1021,      0xFFFF,       0,      0,     0x0000,        1  }}, // 11     
    { "CRC-16/ARC",        { 16,    0x61B0,      0xBB3D,      0x8005,      0x0000,       1,      1,     0x0000,        1  }}, // 12 
    { "CRC-16/AUG-CCITT",  { 16,    0xD2BB,      0xE5CC,      0x1021,      0x1D0F,       0,      0,     0x0000,        1  }}, // 13 
    { "CRC-16/T10-DIF",    { 16,    0xEB7A,      0xD0DB,      0x8BB7,      0x0000,       0,      0,     0x0000,        1  }}, // 14 
    { "CRC-16/TELEDISK",   { 16,    0x4D99,      0x0FB3,      0xA097,      0x0000,       0,      0,     0x0000,        1  }}, // 15 
    { "CRC-16/BUYPASS",    { 16,    0x0789,      0xFEE8,      0x8005,      0x0000,       0,      0,     0x0000,        1  }}, // 16 
    { "CRC-16/USB",        { 16,    0x2E4E,      0xB4C8,      0x8005,      0xFFFF,       1,      1,     0xFFFF,        1  }}, // 17 
    { "CRC-16/CDMA2000",   { 16,    0x144F,      0x4C06,      0xC867,      0xFFFF,       0,      0,     0x0000,        1  }}, // 18 
    { "CRC-16/DDS-110",    { 16,    0x07AD,      0x9ECF,      0x8005,      0x800D,       0,      0,     0x0000,        1  }}, // 19 
    { "CRC-16/DECT-R",     { 16,    0xF121,      0x007E,      0x0589,      0x0000,       0,      0,     0x0001,        1  }}, // 20 
    { "CRC-16/DECT-X",     { 16,    0xF120,      0x007F,      0x0589,      0x0000,       0,      0,     0x0000,        1  }}, // 21 
    { "CRC-16/DNP",        { 16,    0xC078,      0xEA82,      0x3D65,      0x0000,       1,      1,     0xFFFF,        1  }}, // 22 
    { "CRC-16/EN-13757",   { 16,    0xEB03,      0xC2B7,      0x3D65,      0x0000,       0,      0,     0xFFFF,        1  }}, // 23 
    { "CRC-16/GENIBUS",    { 16,    0xB48B,      0xD64E,      0x1021,      0xFFFF,       0,      0,     0xFFFF,        1  }}, // 24 
    { "CRC-16/MAXIM",      { 16,    0x9E4F,      0x44C2,      0x8005,      0x0000,       1,      1,     0xFFFF,        1  }}, // 25 
    { "CRC-16/MCRF4XX",    { 16,    0xCE10,      0x6F91,      0x1021,      0xFFFF,       1,      1,     0x0000,        1  }}, // 26 
    { "CRC-16/RIELLO",     { 16,    0xCD9E,      0x63D0,      0x1021,      0xB2AA,       1,      1,     0x0000,        1  }}, // 27 
    { "CRC-16/TMS37157",   { 16,    0x7B11,      0x26B1,      0x1021,      0x89EC,       1,      1,     0x0000,        1  }}, // 28 
    { "CRC-A",             { 16,    0x2008,      0xBF05,      0x1021,      0xC6C6,       1,      1,     0x0000,        1  }}, // 29 
    { "CRC-16/KERMIT",     { 16,    0x3EA8,      0x2189,      0x1021,      0x0000,       1,      1,     0x0000,        1  }}, // 30 
    { "CRC-16/MODBUS",     { 16,    0xD1B1,      0x4B37,      0x8005,      0xFFFF,       1,      1,     0x0000,        1  }}, // 31 
    { "CRC-16/X-25",       { 16,    0x31EF,      0x906E,      0x1021,      0xFFFF,       1,      1,     0xFFFF,        1  }}, // 32 
    { "CRC-16/XMODEM",     { 16,    0x567B,      0x31C3,      0x1021,      0x0000,       0,      0,     0x0000,        1  }}, // 33 
    { "CRC-32",            { 32,    0x30694C07,  0xCBF43926,  0x04C11DB7,  0xFFFFFFFF,   1,      1,     0xFFFFFFFF,    1  }}, // 34 
    { "CRC-32/BZIP2",      { 32,    0x51137D0B,  0xFC891918,  0x04C11DB7,  0xFFFFFFFF,   0,      0,     0xFFFFFFFF,    1  }}, // 35 
    { "CRC-32/POSIX",      { 32,    0x51A41976,  0x765E7680,  0x04C11DB7,  0x00000000,   0,      0,     0xFFFFFFFF,    1  }}, // 36 
    { "CRC-32Q",           { 32,    0x53BFD78F,  0x3010BF7F,  0x814141AB,  0x00000000,   0,      0,     0x00000000,    1  }}, // 37 
    { "CRC-32/XFER",       { 32,    0x002B459E,  0xBD0BE338,  0x000000AF,  0x00000000,   0,      0,     0x00000000,    1  }}, // 38 
    { "CRC-32C",           { 32,    0xBD9444EA,  0xE3069283,  0x1EDC6F41,  0xFFFFFFFF,   1,      1,     0xFFFFFFFF,    1  }}, // 39 
    { "CRC-32D",           { 32,    0x9BF68F39,  0x87315576,  0xA833982B,  0xFFFFFFFF,   1,      1,     0xFFFFFFFF,    1  }}, // 40 
    { "CRC-32/MPEG-2",     { 32,    0xAEEC82F4,  0x0376E6E7,  0x04C11DB7,  0xFFFFFFFF,   0,      0,     0x00000000,    1  }}, // 41 
    { "CRC-32/JAMCRC",     { 32,    0xCF96B3F8,  0x340BC6D9,  0x04C11DB7,  0xFFFFFFFF,   1,      1,     0x00000000,    1  }}, // 42 
};  // Name                  0 Width  1 "AB"     2 123456789  3 Generator  4 Init    5 RefIn  6 RefOut 7 XorOut      8 IL1  9 Nondirect_init  10 Residue

// crcdef_t zoo[] = {
//     // Name                  0 n   1 Gen    2 IL1  3 Init  4 Nondir. 5 RefIn 6 RefOut 7 XorOut   8 Residue 9 Check      10 "AB"
//     { "CRC-8",             { 8,    0x07,       1,  0x00,       0,      0,      0,     0x00,        0x00,   0x87,        0xF4,          }}, //  0 
//     { "CRC-8/CDMA2000",    { 8,    0x9B,       1,  0xFF,       0,      0,      0,     0x00,        0x00,   0x18,        0xDA,          }}, //  1 
//     { "CRC-8/DARC",        { 8,    0x39,       1,  0x00,       0,      1,      1,     0x00,        0x00,   0x28,        0x15,          }}, //  2 
//     { "CRC-8/DVB-S2",      { 8,    0xD5,       1,  0x00,       0,      0,      0,     0x00,        0x00,   0x56,        0xBC,          }}, //  3 
//     { "CRC-8/EBU",         { 8,    0x1D,       1,  0xFF,       0,      1,      1,     0x00,        0x00,   0x9C,        0x97,          }}, //  4 
//     { "CRC-8/I-CODE",      { 8,    0x1D,       1,  0xFD,       0,      0,      0,     0x00,        0x00,   0x56,        0x7E,          }}, //  5 
//     { "CRC-8/ITU",         { 8,    0x07,       1,  0x00,       0,      0,      0,     0x55,        0x00,   0xD2,        0xA1,          }}, //  6 
//     { "CRC-8/MAXIM",       { 8,    0x31,       1,  0x00,       0,      1,      1,     0x00,        0x00,   0xA5,        0xA1,          }}, //  7 
//     { "CRC-8/ROHC",        { 8,    0x07,       1,  0xFF,       0,      1,      1,     0x00,        0x00,   0x41,        0xD0,          }}, //  8 
//     { "CRC-8/WCDMA",       { 8,    0x9B,       1,  0x00,       0,      1,      1,     0x00,        0x00,   0x66,        0x25,          }}, //  9 
//     { "CRC-15/CAN",        { 15,   0xC599,     0,  0x00,       0,      1,      0,     0x0000,      0x00,   0x48B1,      0x5136,        }}, // 10 
//     { "CRC-16/CCITT",      { 16,   0x1021,     1,  0xFFFF,     1,      0,      0,     0x0000,      0x00,   0x0,         0xE5CC,        }}, // 11     
//     { "CRC-16/ARC",        { 16,   0x8005,     1,  0x0000,     0,      1,      1,     0x0000,      0x00,   0x61B0,      0xBB3D,        }}, // 12 
//     { "CRC-16/AUG-CCITT",  { 16,   0x1021,     1,  0x1D0F,     0,      0,      0,     0x0000,      0x00,   0xD2BB,      0xE5CC,        }}, // 13 
//     { "CRC-16/T10-DIF",    { 16,   0x8BB7,     1,  0x0000,     0,      0,      0,     0x0000,      0x00,   0xEB7A,      0xD0DB,        }}, // 14 
//     { "CRC-16/TELEDISK",   { 16,   0xA097,     1,  0x0000,     0,      0,      0,     0x0000,      0x00,   0x4D99,      0x0FB3,        }}, // 15 
//     { "CRC-16/BUYPASS",    { 16,   0x8005,     1,  0x0000,     0,      0,      0,     0x0000,      0x00,   0x0789,      0xFEE8,        }}, // 16 
//     { "CRC-16/USB",        { 16,   0x8005,     1,  0xFFFF,     0,      1,      1,     0xFFFF,      0x00,   0x2E4E,      0xB4C8,        }}, // 17 
//     { "CRC-16/CDMA2000",   { 16,   0xC867,     1,  0xFFFF,     0,      0,      0,     0x0000,      0x00,   0x144F,      0x4C06,        }}, // 18 
//     { "CRC-16/DDS-110",    { 16,   0x8005,     1,  0x800D,     0,      0,      0,     0x0000,      0x00,   0x07AD,      0x9ECF,        }}, // 19 
//     { "CRC-16/DECT-R",     { 16,   0x0589,     1,  0x0000,     0,      0,      0,     0x0001,      0x00,   0xF121,      0x007E,        }}, // 20 
//     { "CRC-16/DECT-X",     { 16,   0x0589,     1,  0x0000,     0,      0,      0,     0x0000,      0x00,   0xF120,      0x007F,        }}, // 21 
//     { "CRC-16/DNP",        { 16,   0x3D65,     1,  0x0000,     0,      1,      1,     0xFFFF,      0x00,   0xC078,      0xEA82,        }}, // 22 
//     { "CRC-16/EN-13757",   { 16,   0x3D65,     1,  0x0000,     0,      0,      0,     0xFFFF,      0x00,   0xEB03,      0xC2B7,        }}, // 23 
//     { "CRC-16/GENIBUS",    { 16,   0x1021,     1,  0xFFFF,     0,      0,      0,     0xFFFF,      0x00,   0xB48B,      0xD64E,        }}, // 24 
//     { "CRC-16/MAXIM",      { 16,   0x8005,     1,  0x0000,     0,      1,      1,     0xFFFF,      0x00,   0x9E4F,      0x44C2,        }}, // 25 
//     { "CRC-16/MCRF4XX",    { 16,   0x1021,     1,  0xFFFF,     0,      1,      1,     0x0000,      0x00,   0xCE10,      0x6F91,        }}, // 26 
//     { "CRC-16/RIELLO",     { 16,   0x1021,     1,  0xB2AA,     0,      1,      1,     0x0000,      0x00,   0xCD9E,      0x63D0,        }}, // 27 
//     { "CRC-16/TMS37157",   { 16,   0x1021,     1,  0x89EC,     0,      1,      1,     0x0000,      0x00,   0x7B11,      0x26B1,        }}, // 28 
//     { "CRC-A",             { 16,   0x1021,     1,  0xC6C6,     0,      1,      1,     0x0000,      0x00,   0x2008,      0xBF05,        }}, // 29 
//     { "CRC-16/KERMIT",     { 16,   0x1021,     1,  0x0000,     0,      1,      1,     0x0000,      0x00,   0x3EA8,      0x2189,        }}, // 30 
//     { "CRC-16/MODBUS",     { 16,   0x8005,     1,  0xFFFF,     0,      1,      1,     0x0000,      0x00,   0xD1B1,      0x4B37,        }}, // 31 
//     { "CRC-16/X-25",       { 16,   0x1021,     1,  0xFFFF,     0,      1,      1,     0xFFFF,      0x00,   0x31EF,      0x906E,        }}, // 32 
//     { "CRC-16/XMODEM",     { 16,   0x1021,     1,  0x0000,     0,      0,      0,     0x0000,      0x00,   0x567B,      0x31C3,        }}, // 33 
//     { "CRC-32",            { 32,   0x04C11DB7, 1,  0xFFFFFFFF, 0,      1,      1,     0xFFFFFFFF,  0x00,   0x30694C07,  0xCBF43926,    }}, // 34 
//     { "CRC-32/BZIP2",      { 32,   0x04C11DB7, 1,  0xFFFFFFFF, 0,      0,      0,     0xFFFFFFFF,  0x00,   0x51137D0B,  0xFC891918,    }}, // 35 
//     { "CRC-32/POSIX",      { 32,   0x04C11DB7, 1,  0x00000000, 0,      0,      0,     0xFFFFFFFF,  0x00,   0x51A41976,  0x765E7680,    }}, // 36 
//     { "CRC-32Q",           { 32,   0x814141AB, 1,  0x00000000, 0,      0,      0,     0x00000000,  0x00,   0x53BFD78F,  0x3010BF7F,    }}, // 37 
//     { "CRC-32/XFER",       { 32,   0x000000AF, 1,  0x00000000, 0,      0,      0,     0x00000000,  0x00,   0x002B459E,  0xBD0BE338,    }}, // 38 
//     { "CRC-32C",           { 32,   0x1EDC6F41, 1,  0xFFFFFFFF, 0,      1,      1,     0xFFFFFFFF,  0x00,   0xBD9444EA,  0xE3069283,    }}, // 39 
//     { "CRC-32D",           { 32,   0xA833982B, 1,  0xFFFFFFFF, 0,      1,      1,     0xFFFFFFFF,  0x00,   0x9BF68F39,  0x87315576,    }}, // 40 
//     { "CRC-32/MPEG-2",     { 32,   0x04C11DB7, 1,  0xFFFFFFFF, 0,      0,      0,     0x00000000,  0x00,   0xAEEC82F4,  0x0376E6E7,    }}, // 41 
//     { "CRC-32/JAMCRC",     { 32,   0x04C11DB7, 1,  0xFFFFFFFF, 0,      1,      1,     0x00000000,  0x00,   0x340BC6D9,  0xCF96B3F8,    }}, // 42 
// 	// { "CRC-40/GSM",        { 40,   0x0004820009,       1,  0x0000000000,        0,  0,      0,      0xffffffffff,       0xc4ff8071ff,       0xd4164fc646,       0x0,  }},  // 43 
// 	// { "CRC-64/ECMA-182",   { 64,   0x42f0e1eba9ea3693, 1,  0x0000000000000000,  0,  0,      0,      0x0000000000000000, 0x0000000000000000, 0x6c40df5f0b497347, 0x0,  }},  // 44 
// 	// { "CRC-64/GO-ISO",     { 64,   0x000000000000001b, 1,  0xffffffffffffffff,  0,  1,      1, 	    0xffffffffffffffff, 0x5300000000000000, 0xb90956c775a41001, 0x0,  }},  // 45 
//  // { "CRC-64/XZ",         { 64,   0x42f0e1eba9ea3693, 1,  0xffffffffffffffff,  0,  1,      1,      0xffffffffffffffff, 0x49958c9abd7d353f, 0x995dc9bbdf1939fa, 0x0,  }},  // 46 
// };  // Name                  0 n   1 Gen           2 IL1   3 Init      4 Nondirect  5 RefIn  6 RefOut  7 XorOut         8 Residue           9 Check             10 "AB"

// CRC-16/CCITT   
// Width = 16 bits
// Truncated polynomial = 0x1021
// Initial value = 0xFFFF
// Input data is NOT reflected
// Output CRC is NOT reflected
// No XOR is performed on the output CRC
// “Zero” bits explicitly appended to message: yes
// check value for message, “123456789”, using “the” CRC16-CCITT is 0xE5CC

// width=40 poly=0x0004820009 init=0x0000000000 refin=false refout=false xorout=0xffffffffff check=0xd4164fc646 residue=0xc4ff8071ff name="CRC-40/GSM"
// width=64 poly=0x42f0e1eba9ea3693 init=0x0000000000000000 refin=false refout=false xorout=0x0000000000000000 check=0x6c40df5f0b497347 residue=0x0000000000000000 name="CRC-64/ECMA-182"
// width=64 poly=0x42f0e1eba9ea3693 init=0xffffffffffffffff refin=true refout=true xorout=0xffffffffffffffff check=0x995dc9bbdf1939fa residue=0x49958c9abd7d353f name="CRC-64/XZ"
// width=64 poly=0x000000000000001b init=0xffffffffffffffff refin=true refout=true xorout=0xffffffffffffffff check=0xb90956c775a41001 residue=0x5300000000000000 name="CRC-64/GO-ISO"
