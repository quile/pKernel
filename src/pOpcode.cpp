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

//-------------------------------------
// pOpcode.cpp
// The method definitions for the
// pOpcode class
//-------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pOpcode.hpp"

// basic constructor

pOpcode::pOpcode (void)
{
	printf("New pOpcode object\n");

	// admittedly this is inefficient but it's
	// only used once during compilation
	// and then destroyed:

	argSize[NOTHING] = 0;
	argSize[HREG] = sizeof(unsigned char);
	argSize[NREG] = sizeof(unsigned char);
	argSize[SREG] = sizeof(unsigned char);
	argSize[FREG] = sizeof(unsigned char);
	argSize[FLOAT] = sizeof(float);
	argSize[ADDRESS] = sizeof(short);

	nextOpcode = NULL;
	prevOpcode = NULL;
}

// actual constructor used to create an opcode
// object from the input stream

pOpcode::pOpcode (int code, char *opCode, int bytes,
						char *a1, char *a2, char *a3)
{
	value = code;
	nextOpcode = NULL;
	prevOpcode = NULL;

	strcpy (opcode, opCode);
	int i;

	// this is useless so it's going to be replaced
	size = bytes;


	args[0] = getValue(a1);
	args[1] = getValue(a2);
	args[2] = getValue(a3);

	// admittedly this is inefficient but it's
	// only used once during compilation
	// and then destroyed:


	argSize[NOTHING] = 0;
	argSize[HREG] = sizeof(unsigned char);
	argSize[NREG] = sizeof(unsigned char);
	argSize[SREG] = sizeof(unsigned char);
	argSize[FREG] = sizeof(unsigned char);
	argSize[FLOAT] = sizeof(float);
	argSize[ADDRESS] = sizeof(short);

	// calculate the size of the opcode

	size = 1;

	for (i=0; i<3; i++)
	{
		size += argSize[args[i]];
	}
}

// destroys the opcode when it's no longer
// needed by the system

pOpcode::~pOpcode (void)
{
	printf("Destroying pOpcode object\n");

	if (nextOpcode != NULL)
	{
		delete nextOpcode;
	}
}

// adds an opcode to the list

void pOpcode::add(pOpcode *newOpcode)
{
	if (nextOpcode == NULL)
	{
		nextOpcode = newOpcode;
		newOpcode->nextOpcode = NULL;
		newOpcode->prevOpcode = this;
	}
	else
	{
		nextOpcode->add(newOpcode);
	}
}

// finds an opcode given the string
// that identifies it

short pOpcode::findCode (char *opCode)
{
#ifdef WIN32
	if (!stricmp(opCode, opcode))
#else
	if (!strcasecmp(opCode, opcode))
#endif
	{
		// found
		return value;
	}
	else
	{
		if (nextOpcode == NULL)
		{
			return (short)NULL;
		}
		else
		{
			return nextOpcode->findCode(opCode);
		}
	}

	return (short)NULL;
}

// find an opcode object from the
// string identifier

pOpcode *pOpcode::find(char *l)
{
	//printf("%s : %s \n", l, opcode);

#ifdef WIN32
	if (!stricmp(l, opcode))
#else
	if (!strcasecmp(l, opcode))
#endif
	{
		// found
		return this;
	}
	else
	{
		if (nextOpcode == NULL)
		{
			return NULL;
		}
		else
		{
			return nextOpcode->find(l);
		}
	}

	return NULL;
}

// find an opcode object from the
// value identifier

pOpcode *pOpcode::find(short v)
{
	if (v == value)
	{
		// found
		return this;
	}
	else
	{
		if (nextOpcode == NULL)
		{
			return NULL;
		}
		else
		{
			return nextOpcode->find(v);
		}
	}

	return NULL;
}


// find an opcode string given the
// bytecode that is identified with it

char *pOpcode::findString (short code)
{
	if (code == value)
	{
		return opcode;
	}
	else
	{
		if (nextOpcode == NULL)
		{
			return (char *)NULL;
		}
		else
		{
			return nextOpcode->findString(code);
		}
	}

	return (char *)NULL;
}

// evaluates the arguments listed for
// this opcode

short pOpcode::getValue(char *argType)
{
	if (!strcmp(argType, "hr"))
	{
		return HREG;
	}
	else if (!strcmp(argType, "nr"))
	{
		return NREG;
	}
	else if (!strcmp(argType, "fr"))
	{
		return FREG;
	}
	else if (!strcmp(argType, "sr"))
	{
		return SREG;
	}
	else if (!strcmp(argType, "f"))
	{
		return FLOAT;
	}
	else if (!strcmp(argType, "a"))
	{
		return ADDRESS;
	}
	else if (!strcmp(argType, "b"))
	{
		return BYTE;
	}


	return NOTHING;
}

void pOpcode::dump(short recursive)
{
	printf("%02x\t%s\t\t%d\t%d\t%d\t%d\n",
		value, opcode, size, args[0], args[1], args[2]);

	if (recursive && nextOpcode != NULL)
	{
		nextOpcode->dump(1);
	}
}
