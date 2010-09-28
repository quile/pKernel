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
