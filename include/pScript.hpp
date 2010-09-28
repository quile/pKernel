//--------------------------------------
// pScript.hpp
// header definitions for the pScript
// class. This class is instantiated
// once for every executable script
//--------------------------------------

#ifndef __PSCRIPT__
#define __PSCRIPT__ 1

#define HREGISTERS 16
#define SREGISTERS 4
#define NREGISTERS 16
#define FREGISTERS 256
#define STACKSIZE 64

//#include "pKernel.hpp"
//#include "pThread.hpp"
#include "pHarm.hpp"
#include "pNote.hpp"
#include "pScale.hpp"

extern "C" {
#include "../include/pSystem.h"
#include "../include/pFile.h"
}

class pScript
{
	public:
		pScript (void);
		~pScript (void);
		char name[32];
		long codeLength;
		void go(short, short);
		pScript *nextScript;
		pScript *prevScript;
		void add(pScript *);
		void dump(short);
		void dumpData(void);
		void setKernel(void *);
		void setThread(void *);
		unsigned char *allocScript(short);
		unsigned char *pCode;

		pHarm	*hRegisters[HREGISTERS];
		pScale *sRegisters[SREGISTERS];
		pNote	*nRegisters[NREGISTERS];
		float fRegisters[FREGISTERS];

	private:
		long pCounter;
		float totalBeats;
		float elapsedTime;

		// for references around the system

		void *pK;
		void *pT;

		// for reading args:

		short getAddress(short);
		float getFloat(short);
		unsigned char getUChar(short);
		void compare(float, float);

		// these are flags:

		short zero;
		float diff;
		
		// a stack for subroutine calls:

		short stack[64];
		short stackPtr;
};

#endif
