#include <unity.h>
#include "crc-w.h"
#include "crc_types.h"

uint8_t check[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
uint8_t AB[] = {'A', 'B'};

void setUp()
{
}

void tearDown()
{
}

void test_CRC_8()
{
    struct crc_type ctype = CRC_8;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_CDMA2000()
{
    struct crc_type ctype = CRC_8_CDMA2000;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_DARC()
{
    struct crc_type ctype = CRC_8_DARC;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_DVB_S2()
{
    struct crc_type ctype = CRC_8_DVB_S2;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_EBU()
{
    struct crc_type ctype = CRC_8_EBU;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_I_CODE()
{
    struct crc_type ctype = CRC_8_I_CODE;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_ITU()
{
    struct crc_type ctype = CRC_8_ITU;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_MAXIM()
{
    struct crc_type ctype = CRC_8_MAXIM;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_ROHC()
{
    struct crc_type ctype = CRC_8_ROHC;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_8_WCDMA()
{
    struct crc_type ctype = CRC_8_WCDMA;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_15_CAN()
{
    struct crc_type ctype = CRC_15_CAN;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_ARC()
{
    struct crc_type ctype = CRC_16_ARC;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_CCITT()
{
    struct crc_type ctype = CRC_16_CCITT;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_AUG_CCITT()
{
    struct crc_type ctype = CRC_16_AUG_CCITT;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_T10_DIF()
{
    struct crc_type ctype = CRC_16_T10_DIF;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_TELEDISK()
{
    struct crc_type ctype = CRC_16_TELEDISK;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_BUYPASS()
{
    struct crc_type ctype = CRC_16_BUYPASS;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_USB()
{
    struct crc_type ctype = CRC_16_USB;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_CDMA2000()
{
    struct crc_type ctype = CRC_16_CDMA2000;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_DDS_110()
{
    struct crc_type ctype = CRC_16_DDS_110;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_DECT_R()
{
    struct crc_type ctype = CRC_16_DECT_R;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_DECT_X()
{
    struct crc_type ctype = CRC_16_DECT_X;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_DNP()
{
    struct crc_type ctype = CRC_16_DNP;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_EN_13757()
{
    struct crc_type ctype = CRC_16_EN_13757;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_GENIBUS()
{
    struct crc_type ctype = CRC_16_GENIBUS;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_MAXIM()
{
    struct crc_type ctype = CRC_16_MAXIM;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_MCRF4XX()
{
    struct crc_type ctype = CRC_16_MCRF4XX;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_RIELLO()
{
    struct crc_type ctype = CRC_16_RIELLO;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_TMS37157()
{
    struct crc_type ctype = CRC_16_TMS37157;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_A()
{
    struct crc_type ctype = CRC_A;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_KERMIT()
{
    struct crc_type ctype = CRC_16_KERMIT;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_MODBUS()
{
    struct crc_type ctype = CRC_16_MODBUS;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_X_25()
{
    struct crc_type ctype = CRC_16_X_25;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_16_XMODEM()
{
    struct crc_type ctype = CRC_16_XMODEM;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32()
{
    struct crc_type ctype = CRC_32;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32_BZIP2()
{
    struct crc_type ctype = CRC_32_BZIP2;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32_POSIX()
{
    struct crc_type ctype = CRC_32_POSIX;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32Q()
{
    struct crc_type ctype = CRC_32Q;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32_XFER()
{
    struct crc_type ctype = CRC_32_XFER;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32C()
{
    struct crc_type ctype = CRC_32C;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32D()
{
    struct crc_type ctype = CRC_32D;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32_MPEG_2()
{
    struct crc_type ctype = CRC_32_MPEG_2;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_32_JAMCRC()
{
    struct crc_type ctype = CRC_32_JAMCRC;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_40_GSM()
{
    struct crc_type ctype = CRC_40_GSM;
    TEST_ASSERT_EQUAL_HEX(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_64_ECMA_182()
{
    struct crc_type ctype = CRC_64_ECMA_182;
    TEST_ASSERT_EQUAL_HEX64(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX64(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_64_GO_ISO()
{
    struct crc_type ctype = CRC_64_GO_ISO;
    TEST_ASSERT_EQUAL_HEX64(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX64(ctype.AB, crc(AB, sizeof(AB), ctype));
}
void test_CRC_64_XZ()
{
    struct crc_type ctype = CRC_64_XZ;
    TEST_ASSERT_EQUAL_HEX64(ctype.Check, crc(check, sizeof(check), ctype));
    TEST_ASSERT_EQUAL_HEX64(ctype.AB, crc(AB, sizeof(AB), ctype));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_CRC_8);
    RUN_TEST(test_CRC_8_CDMA2000);
    RUN_TEST(test_CRC_8_DARC);
    RUN_TEST(test_CRC_8_DVB_S2);
    RUN_TEST(test_CRC_8_EBU);
    RUN_TEST(test_CRC_8_I_CODE);
    RUN_TEST(test_CRC_8_ITU);
    RUN_TEST(test_CRC_8_MAXIM);
    RUN_TEST(test_CRC_8_ROHC);
    RUN_TEST(test_CRC_8_WCDMA);
    RUN_TEST(test_CRC_15_CAN);
    RUN_TEST(test_CRC_16_ARC);
    RUN_TEST(test_CRC_16_CCITT);
    RUN_TEST(test_CRC_16_AUG_CCITT);
    RUN_TEST(test_CRC_16_T10_DIF);
    RUN_TEST(test_CRC_16_TELEDISK);
    RUN_TEST(test_CRC_16_BUYPASS);
    RUN_TEST(test_CRC_16_USB);
    RUN_TEST(test_CRC_16_CDMA2000);
    RUN_TEST(test_CRC_16_DDS_110);
    RUN_TEST(test_CRC_16_DECT_R);
    RUN_TEST(test_CRC_16_DECT_X);
    RUN_TEST(test_CRC_16_DNP);
    RUN_TEST(test_CRC_16_EN_13757);
    RUN_TEST(test_CRC_16_GENIBUS);
    RUN_TEST(test_CRC_16_MAXIM);
    RUN_TEST(test_CRC_16_MCRF4XX);
    RUN_TEST(test_CRC_16_RIELLO);
    RUN_TEST(test_CRC_16_TMS37157);
    RUN_TEST(test_CRC_A);
    RUN_TEST(test_CRC_16_KERMIT);
    RUN_TEST(test_CRC_16_MODBUS);
    RUN_TEST(test_CRC_16_X_25);
    RUN_TEST(test_CRC_16_XMODEM);
    RUN_TEST(test_CRC_32);
    RUN_TEST(test_CRC_32_BZIP2);
    RUN_TEST(test_CRC_32_POSIX);
    RUN_TEST(test_CRC_32Q);
    RUN_TEST(test_CRC_32_XFER);
    RUN_TEST(test_CRC_32C);
    RUN_TEST(test_CRC_32D);
    RUN_TEST(test_CRC_32_MPEG_2);
    RUN_TEST(test_CRC_32_JAMCRC);
    RUN_TEST(test_CRC_40_GSM);
    RUN_TEST(test_CRC_64_ECMA_182);
    RUN_TEST(test_CRC_64_GO_ISO);
    RUN_TEST(test_CRC_64_XZ);

    return UNITY_END();
}