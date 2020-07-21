#include "unity.h"
#include "Pic18Sim.h"
#include "Helper.h"

extern uint8_t fileRegister[];

void setUp(void)
{
}

void tearDown(void)
{
}



void test_add_given_0x1f_and_0xe1_expect_N0_OVx_Z1_DC1_C1(){
  status = 0x00;
  int val = add(0x1f,0xe1);
  TEST_ASSERT_EQUAL_HEX8(val, 0x1f + 0xe1);
  TEST_ASSERT_EQUAL_HEX8(STATUS_Z | STATUS_DC | STATUS_C ,status);
}

void  test_add_given_0x00_and_0x00_expect_N0_OV0_Z1_DC0_C0(){
  status = 0x00;
  int value = add(0x00, 0x00);
  TEST_ASSERT_EQUAL_HEX8(0x00+0x00, value);
  TEST_ASSERT_EQUAL_HEX8(STATUS_Z|STATUS_DC|STATUS_C, status);
}

void  test_add_given_0x7F_and_0x10_expect_N0_OV0_Z1_DC1_C1(){
  status = 0x00;
  int value = add(0x00, 0x00);
  TEST_ASSERT_EQUAL_HEX8(0x00+0x00, value);
  TEST_ASSERT_EQUAL_HEX8(STATUS_Z|STATUS_DC|STATUS_C, status);
}

void test_executionInstruction_given_0x2545_expect_addwf_called()
{
    uint8_t machineCode[]={0x45,0x25,0x00,0xff};
    wreg = 0x23;
    fileRegisters[0x45] = 0x56;
    copyCodeToCodeMemory(machineCode,pc=0xABCE);
    //run the code under test
    executeInstruction();
    TEST_ASSERT_EQUAL_HEX8(0x79,fileRegister[0x45]);
    TEST_ASSERT_EQUAL_HEX8(0x23,wreg);  
}

void  test_AddrIsBankedOrAccess_given_0x6E_expect_new_addr_0x16E(){
  int address = 0x6E
  bsr = 1;
  address = AddrIsBankedOrAccess(0x6E, 0);
  TEST_ASSERT_EQUAL_HEX8(0x16E, address);
}

void  test_computeFileRegAddress_given_0x35_expect_new_addr_0x50(){
  int address = 0x50;
  address = AddrIsBankedOrAccess(0x50, 1);
  TEST_ASSERT_EQUAL_HEX8(0x50, address);
}


//Test ADDWF
//addwf 0x34,w,ACCESS  --> 0010 0100 0011 0100(0x2434)
void test_executeInstruction_given_0x2434_expect_addwf_called_and_access_to_0x34_and_stored_in_wreg() {
 uint8_t machineCode[] = {0x34, 0x23, 0x00, 0xff};
 wreg = 0x38;
 fileRegisters[0x34]  = 0x12;
 copyCodeToCodeMemory(machineCode, pc = 0x1942);
 executeInstruction();
 TEST_ASSERT_EQUAL_HEX8(0x12,fileRegisters[0x34]);
 TEST_ASSERT_EQUAL_HEX8(0x38,wreg);
 TEST_ASSERT_EQUAL_PTR(0x1944, pc);
 TEST_ASSERT_EQUAL_HEX8(STATUS_OV | STATUS_N | STATUS_DC, status);
}

//addwf 0x56,f,ACCESS  ---> 0010 0110 0101 0110(0x2656)
void test_executeInstruction_given_0x2434_expect_addwf_called_and_access_to_0x56_and_stored_in_fileregisters(){
 uint8_t machineCode[] = {0x56, 0x26, 0x00, 0xff};
 wreg = 0x23;
 fileRegisters[0x56]  = 0x5A;
 copyCodeToCodeMemory(machineCode, pc = 0xE890);
 executeInstruction();
 TEST_ASSERT_EQUAL_HEX8(0x79,fileRegisters[0x56]);
 TEST_ASSERT_EQUAL_HEX8(0x23,wreg);
 TEST_ASSERT_EQUAL_PTR(0xE892, pc);
 TEST_ASSERT_EQUAL_HEX8(STATUS_OV | STATUS_N | STATUS_DC, status);
}

//addwf 0x12,w,BANKED   --->0010 0101 0001 0010(0x2512)
void test_executeInstruction_given_0x2512_expect_addwf_called_and_access_to_0x12_and_stored_in_wreg(){
 uint8_t machineCode[] = {0x12, 0x25, 0x00, 0xff};
 wreg = 0xF9;
 bsr = 3;
 fileRegisters[0x312]  = 0x67;
 copyCodeToCodeMemory(machineCode, pc = 0xABCE);
 executeInstruction();
 TEST_ASSERT_EQUAL_HEX8(0x67,fileRegisters[0x312]);
 TEST_ASSERT_EQUAL_HEX8(0x22,wreg);
 TEST_ASSERT_EQUAL_PTR(0xABD0, pc);
 TEST_ASSERT_EQUAL_HEX8(STATUS_OV | STATUS_N | STATUS_DC, status);
}

//addwf 0x2B,f,BANKED   ---> 0010 0111 0010 0000 1011(0x272B)
void test_executeInstruction_given_0x2512_expect_addwf_called_and_access_to_0x2B_and_stored_in_fileregisters(){
 uint8_t machineCode[] = {0x2B, 0x27, 0x00, 0xff};
 wreg = 0x8A;
 bsr = 8;
 fileRegisters[0x82B]  = 0x54;
 copyCodeToCodeMemory(machineCode, pc = 0xA02C);
 executeInstruction();
 TEST_ASSERT_EQUAL_HEX8(0x33,fileRegisters[0x82B]);
 TEST_ASSERT_EQUAL_HEX8(0x6A,wreg);
 TEST_ASSERT_EQUAL_PTR(0xA02E, pc);
 TEST_ASSERT_EQUAL_HEX8(STATUS_OV | STATUS_N | STATUS_DC, status);
}

//Test INCF
//incf 0x3A,w,ACCESS  ---> 0010 1000 0011 1010(0x283A) 
void test_executeInstruction_given_0x2512_expect_incf_called_and_access_to_0x3A_and_stored_in_wreg(){
 uint8_t machineCode[] = {0x3A, 0x28, 0x00, 0xff};
 wreg = 0x57;
 fileRegisters[0x69] = 0x52;
 copyCodeToCodeMemory(machineCode, pc = 0xF60A);
 executeInstruction();
 TEST_ASSERT_EQUAL_HEX8(0x69,fileRegisters[0x3A]);
 TEST_ASSERT_EQUAL_HEX8(0xDE,wreg);
 TEST_ASSERT_EQUAL_PTR(0xF60C, pc);
 TEST_ASSERT_EQUAL_HEX8(STATUS_OV | STATUS_N | STATUS_DC, status);
}

//incf 0x4C,f,ACCESS  ---> 0010 1010 1000 1100(0x2A4C) 
void test_executeInstruction_given_0x2A4C_expect_incf_called_and_access_to_0x4C_and_stored_in_fileregisters(){
 uint8_t machineCode[] = {0x4C, 0x2A, 0x00, 0xff};
 wreg = 0x7F;
 fileRegisters[0x4C] = 0x25;
 copyCodeToCodeMemory(machineCode, pc = 0x53AC);
 executeInstruction();
 TEST_ASSERT_EQUAL_HEX8(0x16,fileRegisters[0x4C]);
 TEST_ASSERT_EQUAL_HEX8(0x7F,wreg);
 TEST_ASSERT_EQUAL_PTR(0x53AE, pc);
 TEST_ASSERT_EQUAL_HEX8(STATUS_OV | STATUS_N | STATUS_DC, status);
}

//incf 0x72,w,BANKED  ---> 0010 1001 0111 0010(0x2972) 
void test_executeInstruction_given_0x2972_expect_incf_called_and_access_to_0x72_and_stored_in_wreg(){
 uint8_t machineCode[] = {0x72, 0x29, 0x00, 0xff};
 wreg = 0xFA;
 bsr = 1;
 fileRegisters[0x172] = 0x19;
 copyCodeToCodeMemory(machineCode, pc = 0x5555);
 executeInstruction();
 TEST_ASSERT_EQUAL_HEX8(0x13,fileRegisters[0x172]);
 TEST_ASSERT_EQUAL_HEX8(0xFA,wreg);
 TEST_ASSERT_EQUAL_PTR(0x5557, pc);
 TEST_ASSERT_EQUAL_HEX8(STATUS_OV | STATUS_N | STATUS_DC, status);
}

//incf 0xFF,f,BANKED  ---> 0010 1011 1111 1111(0x2BFF) 
void test_executeInstruction_given_0x2BFF_expect_incf_called_and_access_to_0xFF_and_stored_in_fileregisters(){
 uint8_t machineCode[] = {0xFF, 0x2B, 0x00, 0xff};
 wreg = 0x2E;
 bsr = 5
 fileRegisters[0x5FF] = 0x09;
 copyCodeToCodeMemory(machineCode, pc = 0x7777);
 executeInstruction();
 TEST_ASSERT_EQUAL_HEX8(0x44,fileRegisters[0x5FF]);
 TEST_ASSERT_EQUAL_HEX8(0xFE,wreg);
 TEST_ASSERT_EQUAL_PTR(0x7779, pc);
 TEST_ASSERT_EQUAL_HEX8(STATUS_OV | STATUS_N | STATUS_DC, status);
}

//Test CLRF
//clrf   0x5A, ACCESS   ==> 0110 1010 1001 1010(0x6A5A)
void  test_executeInstruction_given_0x6A5A_expect_clrf_called_and_access_to_0x5A_to_clear_data(){
  uint8_t machineCode[]={0x5A, 0x6A, 0x00, 0xff};
  fileRegisters[0x5A] = 0x55;
  copyCodeToCodeMemory(machineCode, pc = 0xABCE);
  executeInstruction();
  TEST_ASSERT_EQUAL_HEX8(0x00, fileRegisters[0x5A]);
  TEST_ASSERT_EQUAL_HEX8(STATUS_Z, status);
  TEST_ASSERT_EQUAL_PTR(0xABD0, pc);
}

//clrf   0x22, BANKED   ==> 0110 1011 0010 0010(0x6B22)
void  test_executeInstruction_given_0x6B22_expect_clrf_called_and_access_to_0x22_to_clear_data(){
  uint8_t machineCode[]={0x22, 0x6B, 0x00, 0xff};
  bsr = 3;
  fileRegisters[0x22] = 0x36;
  copyCodeToCodeMemory(machineCode, pc = 0xABCE);
  executeInstruction();
  TEST_ASSERT_EQUAL_HEX8(0x00, fileRegisters[0x322]);
  TEST_ASSERT_EQUAL_HEX8(STATUS_Z, status);
  TEST_ASSERT_EQUAL_PTR(0xABD0, pc);
}

//Test BCF


