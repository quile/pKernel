//--------------------------------------
// pCompiler.hpp
// header definitions for the pCompiler
// class.  This class should be
// instantiated only once for a
// system (ie. game, plug-in, etc.)
//--------------------------------------

#include <stdio.h>

#ifndef __PCOMPILER__
#define __PCOMPILER__ 1

#include "../include/pScript.hpp"
#include "../include/pOpcode.hpp"
#include "../include/pLineBuffer.hpp"
#include "../include/pAddress.hpp"

extern "C" {
#include "../include/pSystem.h"
#include "../include/pFile.h"
}

class pCompiler
{
	public:
		pCompiler (char *);
		~pCompiler (void);
		pScript *compile(FILE *);		
		void decompile(pScript *);
		void dumpOpcodes(void);
	private:
		void loadCodes (char *);
		pOpcode *opcodes;		
		void putFloat(unsigned char *, float);
};

#endif
