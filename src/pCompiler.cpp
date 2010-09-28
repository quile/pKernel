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
// pCompiler.cpp
// The method definitions for the
// pCompiler class
//-------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "../include/pScript.hpp"
#include "../include/pOpcode.hpp"
#include "../include/pLineBuffer.hpp"
#include "../include/pAddress.hpp"
#include "../include/pCompiler.hpp"

pCompiler::pCompiler (char *byteCodeFile)
{
	if (debug)
	{
		fprintf(stderr, "New pCompiler object\n");
	}

	opcodes = NULL;
	loadCodes(byteCodeFile);
	//dumpOpcodes();
}

pCompiler::~pCompiler (void)
{
	if (debug)
	{
		fprintf(stderr, "Destroying pCompiler object\n");
	}
}

void pCompiler::loadCodes (char *file)
{
	FILE *bFile;
	char inBuf[128];
	char opCode[24];
	int code, bytes;
	char a1[24], a2[24], a3[24];

	bFile = fopen(file, "r");

	if (bFile)
	{
		while (fgets(inBuf, 128, bFile))
		{
			sscanf(inBuf, "%x %s %d %s %s %s", &code, opCode, &bytes, a1, a2, a3);

			if (strlen(opCode))
			{
				//printf ("Code: %d OpCode: %s Size: %d Arg1: %s Arg2: %s Arg3: %s\n",
				//	code, opCode, bytes, a1, a2, a3);

				pOpcode *newOpcode = new pOpcode(code, opCode, bytes, a1, a2, a3);

				if (opcodes == NULL)
				{
					opcodes = newOpcode;
				}
				else
				{
					//printf ("Adding opcode %s\n", opCode);
					opcodes->add(newOpcode);
				}
			}

			// zero the fields

			code = 0;
			strcpy (opCode, "");
			strcpy (a1, "");
			strcpy (a2, "");
			strcpy (a3, "");
			bytes = 0;
		}

		fclose (bFile);
	}
	else
	{
		fprintf(stderr, "Error loading file %s\n", file);
		exit (1);
	}
}

// dumps all the opcodes read in so far

void pCompiler::dumpOpcodes(void)
{
	if (debug)
	{
		fprintf (stderr, "Dumping opcodes... \n");
	}

	if (opcodes != NULL)
	{
		opcodes->dump(1);
	}
}

//--------------------------------------
// compile: reads a stream and compiles
// it, returning a pScript object
// or NULL in the event of a failure
//--------------------------------------

pScript *pCompiler::compile (FILE *in)
{
	pScript *newScript;
	pLineBuffer *lines = NULL;
	pAddress *addresses = NULL;
	pAddress *replacements = NULL;
	char inbuf[MAXCHARS];
	long offset = 0;
	short done = FALSE;
	char label[16];
	char op[24];
	char a[3][24] = {"", "", ""};
	char l[3][24] = {"", "", ""};

	pOpcode *newOp = NULL;
	short opcodeOffset = 1;
	short startIndex;
	int k;

	if (debug)
	{
		fprintf(stderr, "   => Assembling script: Pass 1\n");
	}

	while (!done && fgets(inbuf, MAXCHARS, in))
	{
		startIndex = strspn(inbuf, " \t");

		if (startIndex != 0)
		{
			strcpy(inbuf, inbuf+startIndex);
		}

		if (debug)
		{
			fprintf (stderr, "%s", inbuf);
		}

		if (!strncmp(inbuf, "endscript", 9))
		{
			done = TRUE;
			break;
		}

		pLineBuffer *newLine = new pLineBuffer(inbuf);

		// add line to memory buffer

		if (lines == NULL)
		{
			lines = newLine;
		}
		else
		{
			lines->add(newLine);
		}

		// now dissect line

		if (inbuf[0] == '.')
		{
			// line is a label so store it

			if (strlen(inbuf) <= 15 && strlen(inbuf) > 1)
			{
				sscanf(&(inbuf[1]), "%s", label);
				pAddress *newAddress = new pAddress(label, offset);

				if (addresses == NULL)
				{
					addresses = newAddress;
				}
				else
				{
					addresses->add(newAddress);
				}
			}
		}
		else if (inbuf[0] == '#')
		{
			// line is a comment so ignore it
		}
		else
		{
			// get the opcode

			sscanf(inbuf, "%s", op);

			// I have to check...

			newOp = opcodes->find(op);

			if (newOp != NULL)
			{
				if (debug)
				{
					fprintf(stderr, "%s => value %02x size %d\n", newOp->opcode, newOp->value, newOp->size);
				}
			}
			else
			{
				if (debug)
				{
					fprintf(stderr, "Error: No such opcode (%s)\n");
				}
				return NULL;
			}

			// now check if the opcode has an ADDRESS argument

			opcodeOffset = 1;

			for (k=0; k<3; k++)
			{
				if (newOp->args[k] == ADDRESS)
				{
					// found an address reference so:

					// scan the name in

					sscanf(inbuf, "%*s %s %s %s", l[0], l[1], l[2]);

					strcpy (label, l[k]);

					if (debug)
					{
						fprintf(stderr, "Address referenced: %s\n", label);
					}

					pAddress *newAddr = new pAddress( label, offset+opcodeOffset );

					// and add it to the replacement list

					if (replacements == NULL)
					{
						replacements = newAddr;
					}
					else
					{
						replacements->add(newAddr);
					}
				}

				opcodeOffset += newOp->argSize[newOp->args[k]];
			}

			// finally increment the offset marker

			offset = offset + newOp->size;
		}
	}

	if (lines)
	{
		lines->dump(1);
		if (debug)
		{
			fprintf(stderr, "   => Finished dumping script.\n");
		}

		if (addresses)
		{
			if (debug)
			{
				fprintf(stderr, "Dumping labels:\n");
			}
			addresses->dump(1);
		}

		if (replacements)
		{
			if (debug)
			{
				fprintf(stderr, "Dumping label references:\n");
			}
			replacements->dump(1);
		}
	}

	// Now we have the script in memory
	// and if we got this far then we can
	// now actually allocate memory for the script and
	// continue

	newScript = new pScript();

	//fprintf(stderr, "Script length should be %d", offset);

	if (!newScript->allocScript(offset+16))
	{
		if (debug)
		{
			fprintf(stderr, "Allocation error.\n");
		}

		delete newScript;

		// free up the lines

		if (lines != NULL)
		{
			delete lines;
		}

		if (addresses)
		{
			delete addresses;
		}

		if (replacements)
		{
			delete replacements;
		}

		return NULL;
	}
	else
	{
		if (debug)
		{
			fprintf (stderr, "New script's length set to %d\n", newScript->codeLength);
		}
	}

	// malloc succeeded so start pass 2

	offset = 0;
	pLineBuffer *currentLine = lines;
	float constFloat = 0.0;
	float *floatPtr;
	pAddress *tempAddress = NULL;
	int regNum = 0;

	while (currentLine != NULL)
	{
		// current line is in "currentLine"

		if (debug)
		{
			fprintf(stderr, "%d : compiling %s", offset, currentLine->line);
		}

		if (currentLine->line[0] != '.' && currentLine->line[0] != '#')
		{
			// line is not a label or comment so it's totally valid

			for (k=0; k<3; k++)
			{
				strcpy(a[k], "");
			}

			sscanf(currentLine->line, "%s %s %s %s", op, a[0], a[1], a[2]);

			newOp = opcodes->find(op);

			// write the opcode into the array if it's supposed to
			// be written (any opcodes that are "equates" are
			// NOT supposed to be written into the array because
			// they are instructions to write something else into the
			// array instead).

			if (newOp->value != 0x1e && newOp->value != 0x1f)
			{
				newScript->pCode[offset] = (unsigned char) newOp->value;
				opcodeOffset = 1;
			}
			else
			{
				opcodeOffset = 0;
			}

			// now do the three arguments:

			for (k=0; k<3; k++)
			{
				if (debug)
				{
					fprintf(stderr, "Argument %d (%d) is %s\n", k, newOp->args[k], a[k]);
				}

				if (newOp->args[k] == NOTHING)
				{
					break;
				}

				// check for the different arg types

				switch (newOp->args[k])
				{
					case BYTE:
						regNum = atoi( &(a[k][0]) );
						newScript->pCode[offset+opcodeOffset] = (unsigned char) regNum;
						//fprintf(stderr, "Assigned byte %02x\n", newScript->pCode[offset+opcodeOffset]);
						break;
					case HREG:
					case NREG:
					case SREG:
					case FREG:
						regNum = atoi( &(a[k][1]) );
						newScript->pCode[offset+opcodeOffset] = (unsigned char) regNum;
						//fprintf(stderr, "Assigned byte %02x\n", newScript->pCode[offset+opcodeOffset]);
						break;
					case FLOAT:
						if (debug)
						{
							fprintf(stderr, "Opcode offset is %d\n", opcodeOffset);
						}
						sscanf(a[k], "%f", &constFloat);
						if (debug)
						{
							fprintf(stderr, "Float is %f", constFloat);
						}
						fflush(stdout);
						//floatPtr = (float *) &(newScript->pCode[offset+opcodeOffset]);
						putFloat((unsigned char *) (&(newScript->pCode[0]) + offset + opcodeOffset), constFloat);

						//floatPtr = (float *) &(newScript->pCode[0]) + offset+opcodeOffset;
						//*floatPtr = constFloat;
						constFloat = 0.0;
						newScript->dumpData();
						break;
					case ADDRESS:
						// these get filled in later so just
						// put zeroes
						newScript->pCode[offset+opcodeOffset] = 0;
						newScript->pCode[offset+opcodeOffset+1] = 0;
						break;
					default:
						break;
				}

				opcodeOffset += newOp->argSize[newOp->args[k]];
			}

			// finally increment the offset
			offset = offset + newOp->size;
		}

		currentLine = currentLine->nextLineBuffer;
	}

	// now do any replacements

	pAddress *currentRep = replacements;

	while (currentRep != NULL)
	{
		if (debug)
		{
			fprintf (stderr, "Replacing occurrences of %s\n", currentRep->label);
		}

		pAddress *tempAddr = addresses->find(currentRep->label);

		if (tempAddr)
		{
			//short *shortPtr = NULL;
			short addr = currentRep->offset; // where we wanna put it

			//shortPtr = (short *) (newScript->pCode + addr);
			if (debug)
			{
				fprintf(stderr, "Putting %d into space %d\n", tempAddr->offset, currentRep->offset);
				//fprintf(stderr, "pCode is %lx, pointer is %lx\n", (unsigned long) newScript->pCode, (unsigned long) shortPtr);
			}

			newScript->pCode[addr] = (unsigned char) (tempAddr->offset)%256;
			newScript->pCode[addr+1] = (unsigned char) int(tempAddr->offset/256);

			//fprintf(stderr, "Location now contains %d\n", *shortPtr);
			//fflush(stdout);
			//newScript->dumpData();
			//fflush(stdout);
		}
		else
		{
			fprintf(stderr, "Error: No such label (%s)\n", currentRep->label);

			if (addresses)
				delete addresses;
			if (replacements)
				delete replacements;
			if (lines)
				delete lines;
			delete newScript;
			exit (1);
		}

		currentRep = currentRep->nextAddress;
	}

	// dump the script

	newScript->dumpData();

	// free up the lines

	if (lines != NULL)
	{
		delete lines;
	}

	if (addresses)
	{
		delete addresses;
	}

	if (replacements)
	{
		delete replacements;
	}

	// testing...

	decompile(newScript);

	return newScript;
}

//--------------------------------
// decompile
// disassembles a script
//--------------------------------

void pCompiler::decompile(pScript *script)
{
	short offset = 0;
	unsigned char op;

	if (!debug)
	{
		return;
	}

	fprintf (stderr, "Decompiling script for Testing:\n");
	fflush(stdout);

	while (offset < script->codeLength)
	{
		fprintf(stderr, "%4x ", offset);
		fflush(stdout);

		op = script->pCode[offset];

		pOpcode *newOp = opcodes->find((short)op);

		if (!newOp)
		{
			fprintf (stderr, "Opcode error. Exiting\n");
			break;
		}

		fprintf (stderr, "%s ", newOp->opcode);
		fflush(stdout);

		int k;
		int j;
		short opcodeOffset = 1;

		for (k=0; k<3; k++)
		{
			switch (newOp->args[k])
			{
				case NOTHING:
					k=3; break;
				case HREG:
				case NREG:
				case SREG:
				case FREG:
					fprintf(stderr, "%02x ", script->pCode[offset+opcodeOffset]);
					break;
				case FLOAT:
					float constFloat;
					float *fAddr;

					fAddr = (float *) (script->pCode+offset+opcodeOffset);

					//fprintf(stderr, "Address of float is %lx\n", (unsigned long)fAddr);
					//constFloat = *fAddr;
					//printf ("%2.2f ", constFloat);


					for (j=0; j<sizeof(float); j++)
					{
						fprintf (stderr, "%02x ", script->pCode[offset+opcodeOffset+j]);
					}
					fflush(stdout);
					break;
				case ADDRESS:
					short constShort;
					//constShort = *( (short *) (script->pCode+offset+opcodeOffset));
					//printf ("%04x ", constShort);

					for (j=0; j<sizeof(short); j++)
					{
						fprintf (stderr, "%02x ", script->pCode[offset+opcodeOffset+j]);
					}
					fflush(stdout);
					break;
				default:
					break;
			}
			opcodeOffset = opcodeOffset + newOp->argSize[newOp->args[k]];
		}
		fprintf (stderr, "\n");

		offset += newOp->size;
	}
}

void pCompiler::putFloat(unsigned char *cp, float f)
{
	int i;

	memcpy(cp, (unsigned char *)&(f), sizeof(float));
}
