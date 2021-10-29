// Source: https://crccalc.com/

struct crc_def zoo[] = {
    // Name                  0 Width  1 "AB"     2 123456789  3 Generator  4 Init    5 RefIn  6 RefOut 7 XorOut      8 IL1
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
    { "CRC-15/CAN",        { 15,    0x48B1,      0x2CF,       0xC599,      0x00,         1,      0,     0x00,          0  }}, // 10
    { "CRC-16/ARC",        { 16,    0x61B0,      0xBB3D,      0x8005,      0x0000,       1,      1,     0x0000,        1  }}, // 11
    { "CRC-16/AUG-CCITT",  { 16,    0xD2BB,      0xE5CC,      0x1021,      0x1D0F,       0,      0,     0x0000,        1  }}, // 12
    { "CRC-16/T10-DIF",    { 16,    0xEB7A,      0xD0DB,      0x8BB7,      0x0000,       0,      0,     0x0000,        1  }}, // 13
    { "CRC-16/TELEDISK",   { 16,    0x4D99,      0x0FB3,      0xA097,      0x0000,       0,      0,     0x0000,        1  }}, // 14
    { "CRC-16/BUYPASS",    { 16,    0x0789,      0xFEE8,      0x8005,      0x0000,       0,      0,     0x0000,        1  }}, // 15
    { "CRC-16/USB",        { 16,    0x2E4E,      0xB4C8,      0x8005,      0xFFFF,       1,      1,     0xFFFF,        1  }}, // 16
    { "CRC-16/CDMA2000",   { 16,    0x144F,      0x4C06,      0xC867,      0xFFFF,       0,      0,     0x0000,        1  }}, // 17
    { "CRC-16/DDS-110",    { 16,    0x07AD,      0x9ECF,      0x8005,      0x800D,       0,      0,     0x0000,        1  }}, // 18
    { "CRC-16/DECT-R",     { 16,    0xF121,      0x007E,      0x0589,      0x0000,       0,      0,     0x0001,        1  }}, // 19
    { "CRC-16/DECT-X",     { 16,    0xF120,      0x007F,      0x0589,      0x0000,       0,      0,     0x0000,        1  }}, // 20
    { "CRC-16/DNP",        { 16,    0xC078,      0xEA82,      0x3D65,      0x0000,       1,      1,     0xFFFF,        1  }}, // 21
    { "CRC-16/EN-13757",   { 16,    0xEB03,      0xC2B7,      0x3D65,      0x0000,       0,      0,     0xFFFF,        1  }}, // 22 
    { "CRC-16/GENIBUS",    { 16,    0xB48B,      0xD64E,      0x1021,      0xFFFF,       0,      0,     0xFFFF,        1  }}, // 23
    { "CRC-16/MAXIM",      { 16,    0x9E4F,      0x44C2,      0x8005,      0x0000,       1,      1,     0xFFFF,        1  }}, // 24
    { "CRC-16/MCRF4XX",    { 16,    0xCE10,      0x6F91,      0x1021,      0xFFFF,       1,      1,     0x0000,        1  }}, // 25
    { "CRC-16/RIELLO",     { 16,    0xCD9E,      0x63D0,      0x1021,      0xB2AA,       1,      1,     0x0000,        1  }}, // 26
    { "CRC-16/TMS37157",   { 16,    0x7B11,      0x26B1,      0x1021,      0x89EC,       1,      1,     0x0000,        1  }}, // 27
    { "CRC-A",             { 16,    0x2008,      0xBF05,      0x1021,      0xC6C6,       1,      1,     0x0000,        1  }}, // 28 
    { "CRC-16/KERMIT",     { 16,    0x3EA8,      0x2189,      0x1021,      0x0000,       1,      1,     0x0000,        1  }}, // 29
    { "CRC-16/MODBUS",     { 16,    0xD1B1,      0x4B37,      0x8005,      0xFFFF,       1,      1,     0x0000,        1  }}, // 30
    { "CRC-16/X-25",       { 16,    0x31EF,      0x906E,      0x1021,      0xFFFF,       1,      1,     0xFFFF,        1  }}, // 31
    { "CRC-16/XMODEM",     { 16,    0x567B,      0x31C3,      0x1021,      0x0000,       0,      0,     0x0000,        1  }}, // 32
    { "CRC-32",            { 32,    0x30694C07,  0xCBF43926,  0x04C11DB7,  0xFFFFFFFF,   1,      1,     0xFFFFFFFF,    1  }}, // 33
    { "CRC-32/BZIP2",      { 32,    0x51137D0B,  0xFC891918,  0x04C11DB7,  0xFFFFFFFF,   0,      0,     0xFFFFFFFF,    1  }}, // 34 
    { "CRC-32/POSIX",      { 32,    0x51A41976,  0x765E7680,  0x04C11DB7,  0x00000000,   0,      0,     0xFFFFFFFF,    1  }}, // 35
    { "CRC-32Q",           { 32,    0x53BFD78F,  0x3010BF7F,  0x814141AB,  0x00000000,   0,      0,     0x00000000,    1  }}, // 36
    { "CRC-32/XFER",       { 32,    0x002B459E,  0xBD0BE338,  0x000000AF,  0x00000000,   0,      0,     0x00000000,    1  }}, // 37
    { "CRC-32C",           { 32,    0xBD9444EA,  0xE3069283,  0x1EDC6F41,  0xFFFFFFFF,   1,      1,     0xFFFFFFFF,    1  }}, // 38
    { "CRC-32D",           { 32,    0x9BF68F39,  0x87315576,  0xA833982B,  0xFFFFFFFF,   1,      1,     0xFFFFFFFF,    1  }}, // 39
    { "CRC-32/MPEG-2",     { 32,    0xAEEC82F4,  0x0376E6E7,  0x04C11DB7,  0xFFFFFFFF,   0,      0,     0x00000000,    1  }}, // 40
    { "CRC-32/JAMCRC",     { 32,    0xCF96B3F8,  0x340BC6D9,  0x04C11DB7,  0xFFFFFFFF,   1,      1,     0x00000000,    1  }}, // 41
    { "CRC-15/CANT",       { 15,    0x54FB,      0x00,        0xC599,      0x00,         1,      0,     0x00,          0  }}  // 42
};

