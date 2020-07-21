#include<stdint.h>
#include<stdio.h>
#include "Pic18Sim.h"

InstructionExecutor pic18ExecutionTable[256] = {
  [0x24] = addwf,addwf,addwf,addwf,
  [0x28] = incf,incf,incf,incf,
  [0x6A] = clrf,clrf,
  [0x90] = bcf,bcf,bcf,bcf,bcf,bcf,bcf,bcf,
           bcf,bcf,bcf,bcf,bcf,bcf,bcf,bcf,
  [0x80] = bsf,bsf,bsf,bsf,bsf,bsf,bsf,bsf,
           bsf,bsf,bsf,bsf,bsf,bsf,bsf,bsf,
  [0x14] = andwf,andwf,andwf,andwf,
  [0x6E] = movwf,movwf,
  [0x34] = rlcf,rlcf,rlcf,rlcf,
  [0xE6] = bn,
  [0xE1] = bnz,
  [0xE5] = bnov,
 };
 
 //Range of file rgister: 0x000 - 0xfff
int8_t fileRegisters[0x1000];
// Max range of code memory: 0x000000 - 0x1FFFFF
// Range of code memory for this simulator: 0x0000 - 0xffff
uint8_t codeMemory[0x10000];

int AddrIsBankedOrAccess(int addr,int a){
  if(a){
    //BANKED
    addr+=(unsigned int)bsr<8;
  }else{
    //ACCESS
    if(addr>0x5f)
      addr+=0xf00;
  }
  return addr;
}
    
void executeInstruction(){
 uint8_t *codeptr = &codeMemory[pc];
 InstructionExecutor executor = pic18ExecutionTable[codeptr[1]];
 executor();
}

int add(int val1, int val2){
  int result;
  result = val1 + val2;
  //Adjust the status flags C, DC, Z, OV, N acordingly
  if((uint8_t)result == 0)
    status | = STATUS_Z;  //set Z flag to 1
  if(result>0xFF)
    status | = STATUS_C;  
  if(result & 0x80)
    status | = STATUS_N;
  if(((val1&0x0F)+(val2&0x0F))>0x0F)
    status | = STATUS_DC;
  if(!((uint8_t)val1&0x80)&&!((uint8_t)val2&0x80)&&((uint8_t)val3&0x80))
    status | =STATUS_OV;
  return result;
}

/*
*Direction  (d) 0==> WREG             d 1==> File Register
*Bank       (a) 0==> ACCESS BANKED    a 1==> BANKED
*File register ==> range from 0 to 255
*Mnemonic:addwf f, d,a 
*Opcode : 0010 01da ffff ffff
*/
void addwf(){
 uint8_t *codeptr = &codeMemory[pc];
 int addr = (uint8_t)codeptr[0];
 int d   = codeptr[1] & 0x02;
 int a   = codeptr[1] & 0x01;
 addr = AddrIsBankedOrAccess(addr,a);
  if(d){
    //d is a 1 ==>store result in file register
  fileRegisters[addr] =add( fileRegisters[addr],wreg);
} else{
    //d is a 0 ==>store result in WREG
    wreg = add(fileRegisters[addr],wreg);
}
  pc+=2;
}

/*
*Mnemonic:incf f, d,a 
*Opcode : 0010 10da ffff ffff
*/
void incf (){
 uint8_t *codeptr = &codeMemory[pc];
 int addr =(uint8_t)codeptr[0];
 int d   = codeptr[1] & 0x02;
 int a   = codeptr[1] & 0x01;  
 addr = AddrIsBankedOrAccess(addr,a);
 
  if(d){
    //d is a 1 ==>store result in file register
    fileRegisters[addr] =add( fileRegisters[addr],1);
} else{
    //d is a 0 ==>store result in WREG
    wreg = add(fileRegisters[addr],1);
}
 pc+=2;
}

/*
*Mnemonic:clrf f,a 
*Opcode : 0110 101a ffff ffff
*/
void clrf (){
uint8_t *codeptr = &codeMemory[pc];
int addr = (uint8_t) codeptr[0];
int a = codeptr[1]&0x01;
addr = AddrIsBankedOrAccess(addr, a);
fileRegisters[addr]=0x00;
status|=STATUS_Z;
pc+=2;
}

/*
 * Bit       (b) ]]]range from 0 to 7
 * Bank      (a) 0==> ACCESS BANKED    (a) 1==> BANKED
 *File register ==> range from 0 to 255
 *Mnemonic:bcf f,b,a 
 *Opcode : 1001 bbba ffff ffff
*/

void bcf (){
  
 uint8_t *codeptr = &codeMemory[pc];
 int addr =(uint8_t)codeptr[0];
 int a   = codeptr[1] & 0x01;
 int b   = (codeptr[1] & 0x0E)>>1; 
  
 addr = AddrIsBankedOrAccess(addr, a);
 
  swtich(b){
    case 0 : fileRegisters[addr] &= 0xFE; break;
    case 1 : fileRegisters[addr] &= 0xFD; break;
    case 2 : fileRegisters[addr] &= 0xFB; break;
    case 3 : fileRegisters[addr] &= 0xF7; break;
    case 4 : fileRegisters[addr] &= 0xEF; break;
    case 5 : fileRegisters[addr] &= 0xDF; break;
    case 6 : fileRegisters[addr] &= 0xBF; break;
    case 7 : fileRegisters[addr] &= 0x7F; break;
  }
 pc+=2;
}  


/*
 *Mnemonic:bcf f,b,a 
 *Opcode : 1000 bbba ffff ffff
*/
void bsf (){
  
 uint8_t *codeptr = &codeMemory[pc];
 int addr =(uint8_t)codeptr[0];
 int a   = codeptr[1] & 0x01;
 int b   = (codeptr[1] & 0x0E)>>1; 
  
 addr = AddrIsBankedOrAccess(addr, a);
 
  swtich(b){
    case 0 : fileRegisters[addr] |= 0x01; break;
    case 1 : fileRegisters[addr] |= 0x02; break;
    case 2 : fileRegisters[addr] |= 0x04; break;
    case 3 : fileRegisters[addr] |= 0x08; break;
    case 4 : fileRegisters[addr] |= 0x10; break;
    case 5 : fileRegisters[addr] |= 0x20; break;
    case 6 : fileRegisters[addr] |= 0x40; break;
    case 7 : fileRegisters[addr] |= 0x80; break;
  }
 pc+=2;
}  


/*
Direction     (d):    0 ==> WREG , 1 ==> file register
Bank          (a):    0 ==> ACCESS BANK, 1 ==> BANKED
File Register (f):    range from 0 to 255
*Mnemonic: andwf f,d,a
*opcode: 0001 01da ffff ffff
*/
void andwf(){
 uint8_t *codeptr = &codeMemory[pc];
 int addr = (uint8_t)codeptr[0];
 int d = codePtr[1] & 0x02;
 int a = codePtr[1] & 0x01;
 
 addr = AddrIsBankedOrAccess(addr, a);
 if(d){
    //d is a 1 ==>store result in file register
  fileRegisters[addr] &= wreg;
  if(fileRegisters[addr] == 0x00)
      status |= STATUS_Z;
  if(fileRegisters[addr] & 0x80)
      status |= STATUS_N;
} else{
    //d is a 0 ==>store result in WREG
    wreg &= fileRegisters[addr];
    if(wreg == 0x00)
      status |= STATUS_Z;
    if(wreg & 0x80)
      status |= STATUS_N;
  }
  pc += 2;
}

/*
Bank          (a):    0 ==> ACCESS BANK, 1 ==> BANKED
File Register (f):    range from 0 to 255
Mnemonic: movwf f, a
Opcode: 0110 111a ffff ffff
*/
void movwf(){
 uint8_t *codeptr = &codeMemory[pc];
 int addr = (uint8_t)codeptr[0];
 int a = codeptr[1] & 0x01;

 addr = AddrIsBankedOrAccess(addr, a);

 fileRegisters[addr] = wreg;

 pc += 2;
} 

/*
d (Direction)     :0==>WREG, 1==>bank
a (BANK)          :0==> ACCESS BANK, 1==>BANK
f (File register) : range from 0 to 255
Mnemonic: rlcf f, d, a
Opcode : 0011 01da ffff ffff
*/  
void rlcf(){  
 uint8_t *codeptr = &codeMemory[pc];
 int addr = (uint8_t) codeptr[0];
 int a = codeptr[1]&0x01;
 int d = codeptr[1]&0x02; 
 int LastVal;
 addr = AddrIsBankedOrAccess(addr, a);
 if(status & 0x01)
   LastVal = (fileRegisters[addr]<<1)+1;
 else
   LastVal = fileRegisters[addr]<<1;
 if(d){
   //d is a 1 ==>store result in file register
   fileRegisters[addr] = LastVal;
 }else{
   wreg += LastVal;
 }
 pc+=2;
  if(LastVal==0)
    status |=STATUS_Z;
  if((LastVal&0x80))
    status |= STATUS_N;
  if(LastVal>0xFF)
    status |= STATUS_C;
  else 
    status &=0xFE;
}  

/*
n (value to branch)           :MSB=1, branch backward and vice versa
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Mnemonic: bn n
Opcode : 1110 0110 nnnn nnnn
*/
void bn(){
 uint8_t *codeptr = &codeMemory[pc]; 
 int8_t n = codePtr[0];
 if(status&0x10)
  {
    //backward
  if(n&0x80)
  {
    n=~n;
    pc=pc-(2*n);
  }else{
    //forward
  pc=pc+(2*n)+2;
    }
  }else{
    pc+=2;
  }
}

/*
Mnemonic: bnz n
Opcode: 1110 0001 nnnn nnnn
*/
void bnz(){
 uint8_t *codeptr = &codeMemory[pc]; 
 int8_t n = codePtr[0];
 if(status&0x14)
  {
    pc+=2;
  }else{
    if(n&0x80){
      n=~n;
      pc=pc-(2*n);
    }else{
    pc=pc+(2*n)+2;
    }
  }
}

/*
Mnemonic: bnov  n
Opcode : 1110 0010 nnnn nnnn
*/
void bnov(){
 uint8_t *codeptr = &codeMemory[pc]; 
 int8_t n = codePtr[0];  
  if(status&0x08)
  { 
    pc+=2;
  }else
    if(n&0x80)
  {
    //backward
    n=~n;
    pc=pc-(2*n);
  }else{
  //forward
  pc=pc+(2*n)+2;
    }
}




