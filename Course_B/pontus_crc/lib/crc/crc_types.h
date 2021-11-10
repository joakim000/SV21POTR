#ifndef CRC_TYPES_H
#define CRC_TYPES_H

#include <stdint.h>

struct crc_type
{
    uint8_t *name;
    uint8_t width;
    uint64_t polynomial;
    uint8_t il1;
    uint64_t initial;
    uint8_t nondir;
    uint8_t refin;
    uint8_t refout;
    uint64_t xorout;
    uint64_t Residue;
    uint64_t Check;
    uint64_t AB;
};

extern struct crc_type CRC_8;
extern struct crc_type CRC_8_CDMA2000;
extern struct crc_type CRC_8_DARC;
extern struct crc_type CRC_8_DVB_S2;
extern struct crc_type CRC_8_EBU;
extern struct crc_type CRC_8_I_CODE;
extern struct crc_type CRC_8_ITU;
extern struct crc_type CRC_8_MAXIM;
extern struct crc_type CRC_8_ROHC;
extern struct crc_type CRC_8_WCDMA;
extern struct crc_type CRC_15_CAN;
extern struct crc_type CRC_16_ARC;
extern struct crc_type CRC_16_CCITT;
extern struct crc_type CRC_16_AUG_CCITT;
extern struct crc_type CRC_16_T10_DIF;
extern struct crc_type CRC_16_TELEDISK;
extern struct crc_type CRC_16_BUYPASS;
extern struct crc_type CRC_16_USB;
extern struct crc_type CRC_16_CDMA2000;
extern struct crc_type CRC_16_DDS_110;
extern struct crc_type CRC_16_DECT_R;
extern struct crc_type CRC_16_DECT_X;
extern struct crc_type CRC_16_DNP;
extern struct crc_type CRC_16_EN_13757;
extern struct crc_type CRC_16_GENIBUS;
extern struct crc_type CRC_16_MAXIM;
extern struct crc_type CRC_16_MCRF4XX;
extern struct crc_type CRC_16_RIELLO;
extern struct crc_type CRC_16_TMS37157;
extern struct crc_type CRC_A;
extern struct crc_type CRC_16_KERMIT;
extern struct crc_type CRC_16_MODBUS;
extern struct crc_type CRC_16_X_25;
extern struct crc_type CRC_16_XMODEM;
extern struct crc_type CRC_32;
extern struct crc_type CRC_32_BZIP2;
extern struct crc_type CRC_32_POSIX;
extern struct crc_type CRC_32Q;
extern struct crc_type CRC_32_XFER;
extern struct crc_type CRC_32C;
extern struct crc_type CRC_32D;
extern struct crc_type CRC_32_MPEG_2;
extern struct crc_type CRC_32_JAMCRC;
extern struct crc_type CRC_40_GSM;
extern struct crc_type CRC_64_ECMA_182;
extern struct crc_type CRC_64_GO_ISO;
extern struct crc_type CRC_64_XZ;

#endif