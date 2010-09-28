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
