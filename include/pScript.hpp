/* --------------------------------------------------------------------
 * pKernel - System for real-time musical transformation
 * (C) kd 2010
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

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
