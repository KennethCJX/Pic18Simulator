#ifndef PIC18SIM_H
#define PIC18SIM_H
# include<stdint.h>

typedef struct _21bitInteger  _21bitInteger;
struct _21bitInteger {
  int value:21;
};

#define wreg    fileRegisters [0xFE8]
#define bsr     fileRegisters [0xFE0]
#define status  fileRegisters [0xFD8]
#define pc      ((_21bitInteger *)&fileRegisters[0xff9])->value

#define STATUS_C    (1 << 0)
#define STATUS_DC   (1 << 1)
#define STATUS_Z    (1 << 2)
#define STATUS_OV   (1 << 3)
#define STATUS_N    (1 << 4)

extern  uint8_t codeMemory[];

//instructionExecutor is pointer to function taking in
//(unit8_t *codeptr) returning pinter to unit8_t
typedef void (*InstructionExecutor)(void);

int AddrIsBankedOrAccess(int addr,int a);
int add(int val1, int val2);
void executeInstruction();
void addwf();
void incf();
void clrf();
void bcf();
void bsf();
void andwf();
void rlcf();
void bn();
void bnz();
void bnov();


#endif // PIC18SIM_H
