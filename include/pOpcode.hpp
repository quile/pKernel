//--------------------------------------
// pOpcode.hpp
// header definitions for the pOpcode
// class. This class is instantiated
// once for every opCode
//--------------------------------------

#ifndef __POPCODE__
#define __POPCODE__

#define NOTHING 0
#define HREG 1
#define NREG 2
#define SREG 3
#define FREG 4
#define FLOAT 5
#define ADDRESS 6
#define BYTE 7

class pOpcode
{
	public:
		pOpcode (void);
		pOpcode (int, char *, int, char *, char *, char *);
		~pOpcode (void);
		void add(pOpcode *);
		pOpcode *find(char *);
		pOpcode *find(short);
		short findCode(char *);
		char *findString(short);
		pOpcode *nextOpcode;
		pOpcode *prevOpcode;
		void dump(short);
		char opcode[24];
		short value;
		short size;
		short getValue(char *);
		short args[3];
		short argSize[8];
};

#endif
