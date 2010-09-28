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
// pScript.cpp
// The method definitions for the
// pScript class
//-------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "../include/pScript.hpp"

// Include the pKernel information

#include "../include/pKernel.hpp"

pScript::pScript (void)
{
	int i;

	if (debug)
	{
		fprintf(stderr, "New pScript object\n");
	}
	pCounter = 0;
	codeLength = 0;
	pCode = NULL;
	totalBeats = 0.0;
	elapsedTime= 0.0;
	pK = NULL;
	pT = NULL;

	// script execution stuff:

	zero = FALSE;
	diff = 0.0;

	// set the stack to empty

	stackPtr = -1;

	// zero the registers

	for (i=0; i<SREGISTERS; i++)
	{
		sRegisters[i] = NULL;
	}
	for (i=0; i<NREGISTERS; i++)
	{
		nRegisters[i] = NULL;
	}

	for (i=0; i<HREGISTERS; i++)
	{
		hRegisters[i] = NULL;
	}

	for (i=0; i<FREGISTERS; i++)
	{
		fRegisters[i] = 0.0;
	}
}

pScript::~pScript (void)
{
	if (debug)
	{
		fprintf(stderr, "Destroying pScript object\n");
	}
}

//-------------------------------------------
// add : adds a script to the scriptList
//-------------------------------------------

void pScript::add(pScript *newScript)
{
	if (nextScript == NULL)
	{
		nextScript = newScript;
		newScript->nextScript = NULL;
		newScript->prevScript = this;
	}
	else
	{
		nextScript->add(newScript);
	}
}

//-------------------------------------
// setKernel
// sets a pointer to the current
// pKernel (this may prove useful
// in later developments if there
// are more than one pKernels).
//-------------------------------------

void pScript::setKernel(void *k)
{
	pK = k;
}

//-------------------------------------
// setThread
// sets a thread to be the parent of
// this pScript
//-------------------------------------

void pScript::setThread(void *t)
{
	pT = t;
}

//-------------------------------------
// allocScript
//-------------------------------------

unsigned char *pScript::allocScript(short size)
{
	if ((pCode = (unsigned char *) malloc (size)))
	{
		codeLength = size;
		if (debug)
		{
			fprintf(stderr, "pCode is %lx\n", (unsigned long) pCode);
		}
		return pCode;
	}
	else
	{
		return NULL;
	}
}

//-------------------------------------
// dump
//-------------------------------------

void pScript::dump(short recursive)
{
	if (!debug)
	{
		return;
	}

	fprintf(stderr, "Script: %s\n", name);

	if (recursive && nextScript != NULL)
	{
		nextScript->dump(1);
	}
}

void pScript::dumpData(void)
{
	int i;
	int c;

	if (!debug)
	{
		return;
	}

	fprintf(stderr, "Script length %04x\n", codeLength);

	for (i=0; i<codeLength; i++)
	{
		if (i%16 == 0)
		{
			fprintf(stderr, "\n");
			fprintf(stderr, "%04x ", i);
		}

		c = (int) *(pCode+i);

		fprintf(stderr, "%02x ", c);
	}

	fprintf(stderr, "Finished\n");
}

//-------------------------------------------
// go: this is the script execution
// unit.  It is responsible for running
// all scripts, handling input and
// output, and feeding things to the
// right places.  It works on a suspend/
// awaken idea whereby the script is
// suspended as soon as it reaches its
// destination of sending the right
// number of beats to the outlet.
//-------------------------------------------

void pScript::go (short beats, short beat)
{
	unsigned char opCode;
	float timeSlice = (float) beats/beat;
	short rts = FALSE;
	unsigned char index1, index2, index3;
	float arg1, arg2, arg3;
	short addr1, addr2, addr3;
	short error = FALSE;
	short nCount = 0;
	short no, npc;
	short distanceUp, distanceDown, minDistance;
	short i, lastPC;
	pNote *tempNote, *closestNote, *newNote;
	pHarm *tempHarm, *newHarm;
	float min, max;
	long tRand;
	float tRange;
	pNote *newNPtr;
	pHarm *newHPtr;

	// for now, exit the loop on an rts.  However
	// later, we will want to continue from the beginning of
	// the script on an rts and keep going until we've output
	// the right number of beats

	while ((totalBeats + TOL) < timeSlice && !rts && !error)
	{
		// first get the next opcode

		if (pCounter > codeLength)
		{
			pCounter = 0;

			// error
		}

		opCode = pCode[pCounter];

		if (debug)
		{
			fprintf(stderr, "%2.2f %04x %02x\n", totalBeats, pCounter, opCode);
			fflush(stderr);
		}

		// now act on it by testing the opCode against
		// our opCode table.  Once we've figured out what it
		// is, perform the appropriate action

		switch (opCode)
		{
			case 0x00:
				// break opcode
				rts = TRUE;
				pCounter+=1;
				break;
			case 0x01:
				// return
				if (stackPtr != -1)
				{
					pCounter = stack[stackPtr];
					stackPtr = stackPtr - 1;
				}
				else
				{
					rts = TRUE;
					pCounter = 0;
				}
				break;
			case 0x02:
				// random
				min = getFloat(pCounter+1);
				max = getFloat(pCounter+sizeof(float));
				index1 = getUChar(pCounter+sizeof(float)+sizeof(float)+1);

				// generate random float between min and max

				tRand = rand();
				tRange = max - min;
				fRegisters[index1] = (float) (tRand % (int)tRange) + min;
				pCounter += 2 + 2*(sizeof(float));
				break;
			case 0x03:
				// reset
				pCounter = 0;
				break;
			case 0x04:
				// getElapsedTime
				index1 = getUChar(pCounter+1);
				fRegisters[index1] = elapsedTime;
				pCounter += 2;
				break;
			case 0x05:
				// setElapsedTime
				index1 = getUChar(pCounter+1);
				elapsedTime = fRegisters[index1];
				pCounter += 2;
				break;
			case 0x08:
				// addHarm

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				newHarm = new pHarm();
				newHarm->clone(hRegisters[index1]);
				newHarm->nextHarm = NULL;
				newHarm->prevHarm = NULL;

				if (debug)
				{
					fprintf(stderr, "Dumping new harmony clone:\n");
					newHarm->dump(1);

					if (hRegisters[index1] == NULL)
					{
						fprintf(stderr, "h%d is NULL\n", index1);
					}
					else
					{
						fprintf(stderr, "Displaying data for h%d:\n", index1);
						hRegisters[index1]->dump(0);
					}

					if (hRegisters[index2] == NULL)
					{
						fprintf(stderr, "h%d is NULL\n", index2);
					}
					else
					{
						fprintf(stderr, "Displaying data for h%d:\n", index2);
						hRegisters[index2]->dump(1);
					}
				}

				if (hRegisters[index2] == NULL)
				{
					hRegisters[index2] = newHarm;
				}
				else
				{
					hRegisters[index2]->add(newHarm);
				}

				if (debug)
				{
					fprintf(stderr, "Added harmony to h%d, harmony list is now:\n", index2);
					hRegisters[index2]->dump(1);
				}

				pCounter += 3;
				break;
			case 0x09:
				// addNote

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				newNote = new pNote();
				newNote->clone(nRegisters[index1]);
				newNote->thread = ((pThread *)pT)->threadId;
				newNote->nextNote = NULL;
				newNote->prevNote = NULL;

				if (nRegisters[index2] == NULL)
				{
					nRegisters[index2] = newNote;
				}
				else
				{
					nRegisters[index2]->add(newNote);
				}

				pCounter += 3;
				break;
			case 0x10:
				// insertHarm
				pCounter += 3;
				break;
			case 0x11:
				// insertNote
				pCounter += 3;
				break;
			case 0x14:
				// copyHarm
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				newHarm = new pHarm();

				if (hRegisters[index2] != NULL)
				{
						newHarm->clone(hRegisters[index2]);

					if (hRegisters[index1] == NULL)
					{
						hRegisters[index1] = newHarm;
					}
					else
					{
						hRegisters[index1]->add(newHarm);
					}
				}
				else
				{
					hRegisters[index1] = NULL;
				}
				pCounter += 3;
				break;
			case 0x15:
				// nearestNote
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				// kill whatever's there before we use it

				if (nRegisters[index3] != NULL)
				{
					delete nRegisters[index3];
					nRegisters[index3] = NULL;
				}

				if (nRegisters[index2] != NULL)
				{
					if (nRegisters[index2]->pitchClass == REST)
					{
						nRegisters[index3] = new pNote();
						nRegisters[index3]->clone(nRegisters[index2]);
					}
					else
					{
						if (nRegisters[index2]->pitchClass == DEG)
						{
							npc = nRegisters[index2]->degreePitchClass(pK, ((pKernel *)pK)->currentScale, UP);
						}
						else
						{
							npc = nRegisters[index2]->pitchClass;
						}

						tempNote = nRegisters[index1];
						nRegisters[index3] = new pNote();
						nRegisters[index3]->clone(nRegisters[index1]);

						minDistance = 12;

						while (tempNote)
						{
							// printf(";Checked one...%d, looking for %d\n", tempNote->pitchClass, npc);
							// calculate distance down

							if (tempNote->pitchClass > npc)
							{
								distanceDown = npc + (12-tempNote->pitchClass);
							}
							else
							{
								distanceDown = npc - tempNote->pitchClass;
							}

							// calculate distance up

							if (tempNote->pitchClass < npc)
							{
								distanceUp = tempNote->pitchClass + (12-npc);
							}
							else
							{
								distanceUp = tempNote->pitchClass - npc;
							}

							if ( (minDistance > distanceUp || minDistance > distanceDown) && distanceDown != 0)
							{
								minDistance = (distanceUp>distanceDown? distanceDown:distanceUp);
								//printf("   => minDistance is %d, setting note.\n", minDistance);
								nRegisters[index3]->clone(tempNote);
								no = nRegisters[index2]->octave;

								if (distanceDown < distanceUp)
								{
									if (npc-distanceDown <0)
									{
										no = no-1;
									}
								}
								else
								{
									if (npc+distanceUp>11)
									{
										no = no+1;
									}
								}

								nRegisters[index3]->octave = no;
							}

							tempNote = tempNote->nextNote;
						}

					}
				}
				pCounter += 4;
				break;
			case 0x16:
				// nearestNoteUp
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				if (nRegisters[index3] != NULL)
				{
					delete nRegisters[index3];
					nRegisters[index3] = NULL;
				}

				if (nRegisters[index2] != NULL)
				{
					if (nRegisters[index2]->pitchClass == REST)
					{
						nRegisters[index3] = new pNote();
						nRegisters[index3]->clone(nRegisters[index2]);
					}
					else
					{
						if (nRegisters[index2]->pitchClass == DEG)
						{
							npc = nRegisters[index2]->degreePitchClass(pK, ((pKernel *)pK)->currentScale, UP);
						}
						else
						{
							npc = nRegisters[index2]->pitchClass;
						}

						tempNote = nRegisters[index1];
						nRegisters[index3] = new pNote();
						nRegisters[index3]->clone(nRegisters[index1]);

						minDistance = 12;

						while (tempNote)
						{
							// calculate distance up

							if (tempNote->pitchClass < npc)
							{
								distanceUp = tempNote->pitchClass + (12-npc);
							}
							else
							{
								distanceUp = tempNote->pitchClass - npc;
							}

							if ( minDistance > distanceUp && distanceUp != 0)
							{
								minDistance = distanceUp;
								nRegisters[index3]->clone(tempNote);
								no = nRegisters[index2]->octave;

								if (npc+distanceUp>11)
								{
									no = no+1;
								}

								nRegisters[index3]->octave = no;
							}

							tempNote = tempNote->nextNote;
						}

					}
				}
				pCounter += 4;
				break;
			case 0x17:
				// nearestNoteDown
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				if (nRegisters[index3] != NULL)
				{
					delete nRegisters[index3];
					nRegisters[index3] = NULL;
				}

				if (nRegisters[index2] != NULL)
				{
					if (nRegisters[index2]->pitchClass == REST)
					{
						nRegisters[index3] = new pNote();
						nRegisters[index3]->clone(nRegisters[index2]);
					}
					else
					{
						if (nRegisters[index2]->pitchClass == DEG)
						{
							npc = nRegisters[index2]->degreePitchClass(pK, ((pKernel *)pK)->currentScale, UP);
						}
						else
						{
							npc = nRegisters[index2]->pitchClass;
						}

						tempNote = nRegisters[index1];
						nRegisters[index3] = new pNote();
						nRegisters[index3]->clone(nRegisters[index1]);

						minDistance = 12;

						while (tempNote)
						{
							// printf(";Checked one...%d, looking for %d\n", tempNote->pitchClass, npc);
							// calculate distance down

							if (tempNote->pitchClass > npc)
							{
								distanceDown = npc + (12-tempNote->pitchClass);
							}
							else
							{
								distanceDown = npc - tempNote->pitchClass;
							}

							if ( minDistance > distanceDown && distanceDown != 0)
							{
								minDistance =  distanceDown;
								// printf(";   => minDistance is %d, setting note.\n", minDistance);
								nRegisters[index3]->clone(tempNote);
								no = nRegisters[index2]->octave;

								if (npc-distanceDown <0)
								{
									no = no-1;
								}

								nRegisters[index3]->octave = no;
							}

							tempNote = tempNote->nextNote;
						}

					}
				}
				pCounter += 4;
				break;
			case 0x18:
				// deleteHarm

				index1 = getUChar(pCounter+1);

				newHPtr = hRegisters[index1];

				if (newHPtr != NULL)
				{
					if (newHPtr->nextHarm != NULL)
					{
						if (newHPtr->prevHarm != NULL)
						{
							newHPtr->prevHarm->nextHarm = newHPtr->nextHarm;
							newHPtr->nextHarm->prevHarm = newHPtr->prevHarm;
						}

						newHPtr = newHPtr->nextHarm;
					}
					else if (newHPtr->prevHarm != NULL)
					{
						newHPtr->prevHarm->nextHarm = NULL;

						newHPtr = newHPtr->prevHarm;
					}
					else
					{
						newHPtr = NULL;
					}

					// unhook the register

					hRegisters[index1]->nextHarm = NULL;

					// destroy the object

					delete hRegisters[index1];

					// reassign the register

					hRegisters[index1] = newHPtr;
				}
				pCounter += 2;
				break;
			case 0x19:
				// deleteNote

				index1 = getUChar(pCounter+1);

				newNPtr = nRegisters[index1];

				if (newNPtr != NULL)
				{
					if (newNPtr->nextNote != NULL)
					{
						if (newNPtr->prevNote != NULL)
						{
							newNPtr->prevNote->nextNote = newNPtr->nextNote;
							newNPtr->nextNote->prevNote = newNPtr->prevNote;
						}

						newNPtr = newNPtr->nextNote;
					}
					else if (newNPtr->prevNote != NULL)
					{
						newNPtr->prevNote->nextNote = NULL;

						newNPtr = newNPtr->prevNote;
					}
					else
					{
						newNPtr = NULL;
					}

					// unhook the register

					nRegisters[index1]->nextNote = NULL;

					// destroy the object

					delete nRegisters[index1];

					// reassign the register

					nRegisters[index1] = newNPtr;
				}
				pCounter += 2;
				break;
			case 0x1a:
				// stealHarm

				arg1 = getFloat(pCounter+1);
				arg2 = getFloat(pCounter+1+sizeof(float));
				index3 = getUChar(pCounter+1+2*sizeof(float));

				if (hRegisters[index3] != NULL)
				{
					delete hRegisters[index3];
					hRegisters[index3] = NULL;
				}

				if (pK != NULL && arg1 < MAXTHREADS &&
					 ((pKernel *)pK)->threads[(short)arg1] != NULL)
				{
					hRegisters[index3] = ((pKernel *)pK)->threads[(short)arg1]->scriptList->hRegisters[(short)arg2];
					((pKernel *)pK)->threads[(short)arg1]->scriptList->hRegisters[(short)arg2] = NULL;
				}
				pCounter += 10;
				break;
			case 0x1b:
				// stealNote

				arg1 = getFloat(pCounter+1);
				arg2 = getFloat(pCounter+1+sizeof(float));
				index3 = getUChar(pCounter+1+2*sizeof(float));

				if (nRegisters[index3] != NULL)
				{
					delete nRegisters[index3];
					nRegisters[index3] = NULL;
				}

				if (pK != NULL && arg1 < MAXTHREADS &&
					 ((pKernel *)pK)->threads[(short)arg1] != NULL)
				{
					nRegisters[index3] = ((pKernel *)pK)->threads[(short)arg1]->scriptList->nRegisters[(short)arg2];
					((pKernel *)pK)->threads[(short)arg1]->scriptList->nRegisters[(short)arg2] = NULL;
				}
				pCounter += 10;
				break;
			case 0x1c:
				// destroyHarm
				index1 = getUChar(pCounter+1);

				if (debug)
				{
					fprintf(stderr, "Destroying harmony list %d, which is currently:\n", index1);
					if (hRegisters[index1] != NULL)
					{
						hRegisters[index1]->dump(1);
					}
					else
					{
						fprintf(stderr, "Empty.\n");
					}
				}

				if (hRegisters[index1] != NULL)
				{
					delete hRegisters[index1];
					hRegisters[index1] = NULL;
				}
				pCounter += 2;
				break;
			case 0x1d:
				// destroyNote
				index1 = getUChar(pCounter+1);
				if (nRegisters[index1] != NULL)
				{
					delete nRegisters[index1];
					nRegisters[index1] = NULL;
				}
				if (debug)
				{
					fprintf(stderr, "Destroying notelist %d\n", index1);
				}

				pCounter += 2;
				break;
			case 0x20:
				// newHarm

				index1 = getUChar(pCounter+1);
				hRegisters[index1] = new pHarm();

				pCounter += 2;
				break;
			case 0x21:
				// newNote

				index1 = getUChar(pCounter+1);
				nRegisters[index1] = new pNote();
				nRegisters[index1]->thread = ((pThread *)pT)->threadId;

				pCounter += 2;
				break;
			case 0x22:
				// newFloat

				index1 = getUChar(pCounter+1);
				fRegisters[index1] = getFloat(pCounter+2);

				pCounter += sizeof(float)+2;
				break;
			case 0x23:
				// newScale

				index1 = getUChar(pCounter+1);
				sRegisters[index1] = new pScale("", 1);

				pCounter += 2;
				break;
			case 0x24:
				// getHarmOffset

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				arg1 = fRegisters[index2];
				tempHarm = hRegisters[index1];

				while (tempHarm != NULL)
				{
					if (arg1 < (tempHarm->fdur-TOL))
					{
						break;
					}

					arg1 = arg1 - tempHarm->fdur;

					tempHarm = tempHarm->nextHarm;

					if (tempHarm == NULL)
					{
						tempHarm = hRegisters[index1];
					}
				}

				hRegisters[index3] = tempHarm;
				pCounter += 4;
				break;
			case 0x25:
				// getNoteOffset

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				arg1 = fRegisters[index2];
				tempNote = nRegisters[index1];

				while (tempNote != NULL)
				{
					arg1 = arg1 - tempNote->fdur;

					if (arg1 < TOL)
					{
						break;
					}

					tempNote = tempNote->nextNote;

					if (tempNote == NULL)
					{
						tempNote = nRegisters[index1];
					}
				}

				nRegisters[index3] = tempNote;
				pCounter += 4;
				break;
			case 0x26:
				// copyFloat

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				fRegisters[index1] = fRegisters[index2];

				pCounter += 3;
				break;
			case 0x27:
				// copyNote

				index1 = getUChar (pCounter+1);
				index2 = getUChar (pCounter+2);

				if (nRegisters[index1] == NULL)
				{
					nRegisters[index1] = new pNote();
				}

				if (nRegisters[index2] != NULL)
				{
					nRegisters[index1]->clone(nRegisters[index2]);
					nRegisters[index1]->nextNote = NULL;
					nRegisters[index1]->prevNote = NULL;
				}
				pCounter += 3;
				break;
			case 0x28:
				// setRoot

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					hRegisters[index1]->setRoot((short) fRegisters[index2]);
				}
				pCounter += 3;
				break;
			case 0x29:
				// setInversion

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					hRegisters[index1]->setInversion((short) fRegisters[index2]);
				}
				pCounter += 3;
				break;
			case 0x2a:
				// setOctave

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					hRegisters[index1]->setOctave((short) fRegisters[index2]);
				}
				pCounter += 3;
				break;
			case 0x2b:
				// setDuration
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+1);

				if (hRegisters[index1] != NULL)
				{
					hRegisters[index1]->setDurationF(fRegisters[index2]);
				}
				pCounter += 3;
				break;
			case 0x2c:
				// setScale
				pCounter += 3;
				break;
			case 0x2d:
				// setSize
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					hRegisters[index1]->size = (short) fRegisters[index2];
				}
				pCounter+=3;
				break;
			case 0x2e:
				// setAlteration
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					hRegisters[index1]->alteration = (short) fRegisters[index2];
				}
				pCounter+=3;
				break;
			case 0x2f:
				// setChordTone
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				if (hRegisters[index1] != NULL)
				{
					hRegisters[index1]->chordTones[index2] = fRegisters[index3];
				}

				pCounter += 4;
				break;
			case 0x30:
				// getRoot
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					fRegisters[index2] = hRegisters[index1]->getRoot();
				}

				pCounter += 3;
				break;
			case 0x31:
				// getInversion
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					fRegisters[index2] = (float) hRegisters[index1]->getInversion();
				}

				pCounter += 3;
				break;
			case 0x32:
				// getOctave
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					fRegisters[index2] = (float) hRegisters[index1]->getOctave();
				}

				pCounter += 3;
				break;
			case 0x33:
				// getDuration
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					fRegisters[index2] = hRegisters[index1]->fdur;

					if (debug)
					{
						hRegisters[index1]->dump(0);
						//fprintf(stderr, "Duration of harmony is %2.2f\n", hRegisters[index1]->fdur);
						//fprintf(stderr, "Setting fRegister %d to %2.2f\n", index2, fRegisters[index2]);
					}
				}
				else
				{
					error = 9;
				}

				pCounter += 3;
				break;
			case 0x34:
				// getScale
				pCounter += 3;
				break;
			case 0x35:
				// getSize
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					fRegisters[index2] = (float) hRegisters[index1]->size;
				}
				pCounter += 3;
				break;
			case 0x36:
				// getAlteration
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL)
				{
					fRegisters[index2] = (float) hRegisters[index1]->alteration;
				}
				pCounter+=3;
				break;
			case 0x37:
				// getChordTone

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				if (hRegisters[index1] != NULL)
				{
					fRegisters[index3] = (float) hRegisters[index1]->chordTones[index2];
				}
				pCounter += 4;
				break;
			case 0x38:
				// setNotePitchClass
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL)
				{
					nRegisters[index1]->pitchClass = (short) fRegisters[index2];
				}
				pCounter += 3;
				break;
			case 0x39:
				// setNoteOctave
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL)
				{
					nRegisters[index1]->octave = (short) fRegisters[index2];
				}
				pCounter += 3;
				break;
			case 0x3a:
				// setNoteDuration
				// this is bogus because we can't possibly
				// keep float and char representations of the
				// note duration around...

				index1 = (unsigned char) getUChar(pCounter+1);
				index2 = (unsigned char) getUChar(pCounter+2);

				if (nRegisters[index1] != NULL)
				{
					nRegisters[index1]->fdur = fRegisters[index2];

					if (debug)
					{
						fprintf(stderr, "Setting note %02d's duration to %2.2f\n", index1, nRegisters[index1]->fdur);
					}
				}
				else
				{
					error = 8;
				}
				pCounter += 3;
				break;
			case 0x3b:
				// setNoteDegree
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL)
				{
					nRegisters[index1]->degree = (short)fRegisters[index2];
					nRegisters[index1]->pitchClass = DEG;
				}
				pCounter += 3;
				break;
			case 0x3c:
				// setNoteAlteration
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				if (nRegisters[index1] != NULL)
				{
					nRegisters[index1]->alteration[(short)fRegisters[index2]] = (short)fRegisters[index3];
				}
				pCounter += 4;
				break;
			case 0x3d:
				// setNoteBaggage
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				if (nRegisters[index1] != NULL)
				{
					nRegisters[index1]->baggage[(short)fRegisters[index2]] = (char)fRegisters[index3];

					if (debug)
					{
						fprintf(stderr, "Set note baggage %d to %d... it's now %d.\n", (short) fRegisters[index2], (short)fRegisters[index3], nRegisters[index1]->baggage[(short)fRegisters[index2]]);
					}
				}
				pCounter += 4;
				break;
			case 0x3e:
				// transpose
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] == NULL)
				{
					error = 4;
					pCounter += 3;
					break;
				}

				if (nRegisters[index1]->pitchClass != REST)
				{
					// move the note up or down
					// and make sure that the octave moves with it

					short st = (short) fRegisters[index2];

					if (nRegisters[index1]->pitchClass == DEG)
					{
						nRegisters[index1]->pitchClass = nRegisters[index1]->degreePitchClass(pK, ((pKernel *)pK)->currentScale, UP);
					}

					if (st > 0)
					{
						// going up

						st += nRegisters[index1]->pitchClass;

						while (st > 12)
						{
							nRegisters[index1]->octave ++;
							st -= 12;
						}
						nRegisters[index1]->pitchClass = st;
					}
					else
					{
						// going down

						st += nRegisters[index1]->pitchClass;

						while (st < 0)
						{
							nRegisters[index1]->octave--;
							st += 12;
						}
						nRegisters[index1]->pitchClass = st;
					}
				}

				pCounter += 3;
				break;
			case 0x3f:
				// transposeDirect
				index1 = getUChar(pCounter+1);
				arg2 = getFloat(pCounter+2);

				if (nRegisters[index1] == NULL)
				{
					error = 4;
					pCounter += (sizeof(float)+2);
					break;
				}

				if (nRegisters[index1]->pitchClass != REST)
				{
					// move the note up or down
					// and make sure that the octave moves with it

					short st = (short) arg2;

					if (nRegisters[index1]->pitchClass == DEG)
					{
						nRegisters[index1]->pitchClass = nRegisters[index1]->degreePitchClass(pK, ((pKernel *)pK)->currentScale, UP);
					}


					if (st > 0)
					{
						// going up

						st += nRegisters[index1]->pitchClass;

						while (st > 12)
						{
							nRegisters[index1]->octave ++;
							st -= 12;
						}
						nRegisters[index1]->pitchClass = st;
					}
					else
					{
						// going down

						st += nRegisters[index1]->pitchClass;

						while (st < 0)
						{
							nRegisters[index1]->octave--;
							st += 12;
						}
						nRegisters[index1]->pitchClass = st;
					}
				}

				pCounter += (sizeof(float)+2);
				break;
			case 0x40:
				// getNotePitchClass

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL)
				{
					if (nRegisters[index1]->pitchClass != DEG)
					{
						fRegisters[index2] = (float)nRegisters[index1]->pitchClass;
					}
					else
					{
						fRegisters[index2] = (float)nRegisters[index1]->degreePitchClass( pK, ((pKernel *)pK)->currentScale, UP);
					}
				}

				pCounter += 3;
				break;
			case 0x41:
				// getNoteOctave

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL)
				{
					fRegisters[index2] = (float)nRegisters[index1]->octave;
				}

				pCounter += 3;
				break;
			case 0x42:
				// getNoteDuration

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL)
				{
					fRegisters[index2] = (float)nRegisters[index1]->fdur;
				}

				pCounter += 3;
				break;
			case 0x43:
				// getNoteDegree
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL)
				{
					fRegisters[index2] = nRegisters[index1]->degree;
				}
				pCounter += 3;
				break;
			case 0x44:
				// getNoteAlteration
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				if (nRegisters[index1] != NULL)
				{
					fRegisters[index3] = nRegisters[index1]->alteration[(short)fRegisters[index2]];
				}
				pCounter += 4;
				break;
			case 0x45:
				// getNoteBaggage
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				if (nRegisters[index1] != NULL)
				{
					fRegisters[index3] = (float) nRegisters[index1]->baggage[(short)fRegisters[index2]];
				}
				pCounter += 4;
				break;
			case 0x46:
				// setTotalBeats
				index1 = getUChar(pCounter+1);
				totalBeats = fRegisters[index1];
				pCounter+=2;
				break;
			case 0x47:
				// getTotalBeats
				index1 = getUChar(pCounter+1);
				fRegisters[index1] = totalBeats;
				pCounter+=2;
				break;
			case 0x48:
				// getNextHarm

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL &&
					 hRegisters[index1]->nextHarm != NULL)
				{
					hRegisters[index2] = hRegisters[index1]->nextHarm;
					zero = FALSE;
				}
				else
				{
					// set an error code
					zero = TRUE;
				}
				pCounter += 3;
				break;
			case 0x49:
				// getNextNote

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL &&
					 nRegisters[index1]->nextNote != NULL)
				{
					nRegisters[index2] = nRegisters[index1]->nextNote;
					zero = FALSE;
				}
				else
				{
					// set an error code
					zero = TRUE;
				}
				pCounter += 3;
				break;
			case 0x4a:
				// importByte
				index1 = getUChar(pCounter+1);
				addr1 = getAddress(pCounter+2);
				index2 = getUChar(pCounter+2+sizeof(short));

				fRegisters[index1] = (float) pCode[addr1+(short)(fRegisters[index2])];

				if (debug)
				{
					fprintf(stderr, "importing byte %d after address %d, which is %d\n", index2, addr1, (short)fRegisters[index1]);
				}

				pCounter += ( 3 + sizeof(short));
				break;
			case 0x4b:
				// importFloat
				index1 = getUChar(pCounter+1);
				addr1 = getAddress(pCounter+2);
				index2 = getUChar(pCounter+2+sizeof(short));

				fRegisters[index1] = getFloat( pCounter+addr1+(short)(fRegisters[index2])*sizeof(float) );
				pCounter += ( 3 + sizeof(short));
				break;
			case 0x4c:
				// getCurrentScale

				index1 = getUChar(pCounter+1);

				fRegisters[index1] = ((pKernel *)pK)->currentScale;

				pCounter += 2;
				break;
			case 0x4d:
				// setCurrentScale

				index1 = getUChar(pCounter+1);
				((pKernel *)pK)->currentScale = (short) fRegisters[index1];

				pCounter += 2;
				break;
			case 0x4e:
				// getTimeSlice
				index1 = getUChar(pCounter+1);

				fRegisters[index1] = timeSlice;
				pCounter += 2;
				break;
			case 0x4f:
				// setTimeSlice
				index1 = getUChar(pCounter+1);

				timeSlice = fRegisters[index1];
				pCounter += 2;
				break;
			case 0x50:
				// getPrevHarm

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (hRegisters[index1] != NULL &&
					 hRegisters[index1]->prevHarm != NULL)
				{
					hRegisters[index2] = hRegisters[index1]->prevHarm;
					zero = FALSE;
				}
				else
				{
					// set an error code
					zero = TRUE;
				}
				pCounter += 3;
				break;
			case 0x51:
				// getPrevNote

				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (nRegisters[index1] != NULL &&
					 nRegisters[index2]->prevNote != NULL)
				{
					nRegisters[index2] = nRegisters[index1]->prevNote;
					zero = FALSE;
				}
				else
				{
					// set an error code
					zero = TRUE;
				}
				pCounter += 3;
				break;
			case 0x53:
				// mult
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				fRegisters[index1] = (fRegisters[index1] * fRegisters[index2]);
				pCounter += 3;
				break;
			case 0x54:
				// multDirect
				index1 = getUChar(pCounter+1);
				arg2 = getFloat(pCounter+2);

				fRegisters[index1] = (fRegisters[index1] * arg2);
				pCounter += (2+sizeof(float));
				break;
			case 0x55:
				// int
				index1 = getUChar(pCounter+1);

				fRegisters[index1] = (float) (long) fRegisters[index1];
				pCounter+=2;
				break;
			case 0x56:
				// div
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				fRegisters[index1] = (fRegisters[index1]/fRegisters[index2]);
				pCounter += 3;
				break;
			case 0x57:
				// divDirect
				index1 = getUChar(pCounter+1);
				arg2 = getFloat(pCounter+2);

				fRegisters[index1] = (fRegisters[index1]/arg2);
				pCounter += (2+sizeof(float));
				break;
			case 0x58:
				// inc
				index1 = getUChar(pCounter+1);
				fRegisters[index1] += 1.0;
				pCounter += 2;
				break;
			case 0x59:
				// dec
				index1 = getUChar(pCounter+1);
				fRegisters[index1] -= 1.0;
				pCounter += 2;
				break;
			case 0x5a:
				// add
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				fRegisters[index1] += fRegisters[index2];
				pCounter += 3;
				break;
			case 0x5b:
				// addDirect
				index1 = getUChar(pCounter+1);
				arg2 = getFloat(pCounter+2);
				fRegisters[index1] += arg2;
				pCounter += (sizeof(float)+2);
				break;
			case 0x5c:
				// sub
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				fRegisters[index1] -= fRegisters[index2];
				pCounter += 3;
				break;
			case 0x5d:
				// subDirect
				index1 = getUChar(pCounter+1);
				arg2 = getFloat(pCounter+2);
				fRegisters[index1] -= arg2;
				pCounter += (sizeof(float)+2);
				break;
			case 0x5e:
				// mod
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				fRegisters[index1] =(float) (( (long) fRegisters[index1]) % ((long) fRegisters[index2]));
				pCounter += 3;
				break;
			case 0x5f:
				// modDirect
				index1 = getUChar(pCounter+1);
				arg2 = getFloat(pCounter+2);

				fRegisters[index1] =(float) (((long) fRegisters[index1]) % ((long) arg2));
				pCounter += (2 + sizeof(float));
				break;
			case 0x60:
				// jmp
				pCounter = getAddress(pCounter+1);
				break;
			case 0x61:
				// jsr
				if (stackPtr < STACKSIZE)
				{
					// push PC of next instruction on
					// stack, then set PC to address

					stackPtr+=1;
					stack[stackPtr] = pCounter + 3;
					pCounter = getAddress(pCounter+1);
				}
				else
				{
					pCounter += 3;
				}
				break;
			case 0x62:
				// beq

				if (zero)
				{
					pCounter = getAddress(pCounter+1);
				}
				else
				{
					pCounter += 3;
				}
				break;
			case 0x63:
				// bne

				if (!zero)
				{
					pCounter = getAddress(pCounter+1);
				}
				else
				{
					pCounter += 3;
				}
				break;
			case 0x64:
				// blt

				if (diff<0)
				{
					pCounter = getAddress(pCounter+1);
				}
				else
				{
					pCounter += 3;
				}
				break;
			case 0x65:
				// bgt

				if (diff>0)
				{
					pCounter = getAddress(pCounter+1);
				}
				else
				{
					pCounter += 3;
				}
				break;
			case 0x66:
				// ble

				if (diff<=TOL)
				{
					pCounter = getAddress(pCounter+1);
				}
				else
				{
					pCounter += 3;
				}
				break;
			case 0x67:
				// bge

				if (diff>=0.0)
				{
					pCounter = getAddress(pCounter+1);
				}
				else
				{
					pCounter += 3;
				}
				break;
			case 0x68:
				// cmpHarm
				pCounter += 3;
				break;
			case 0x69:
				// cmpNote
				pCounter += 3;
				break;
			case 0x6a:
				// cmpScale
				pCounter += 3;
				break;
			case 0x6b:
				// cmp
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				compare(fRegisters[index1], fRegisters[index2]);
				pCounter += 3;
				break;
			case 0x6c:
				// cmpDirect
				index1 = getUChar(pCounter+1);
				arg2 = getFloat(pCounter+2);
				compare(fRegisters[index1], arg2);
				pCounter += (sizeof(float)+2);
				break;
			case 0x6d:
				// getInput
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (pK != NULL)
				{
					fRegisters[index1] = ((pKernel *)pK)->iRegisters[index2];
				}
				pCounter += 3;
				break;
			case 0x6e:
				// setInput
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);

				if (pK != NULL)
				{
					((pKernel *)pK)->iRegistersOut[index1] = (short) fRegisters[index2];
				}
				pCounter += 3;
				break;
			case 0x6f:
				// setInputDirect
				index1 = getUChar(pCounter+1);
				arg2 = getFloat(pCounter+2);

				if (pK != NULL)
				{
					((pKernel *)pK)->iRegisters[index1] = (short) arg2;
				}

				pCounter += (2 + sizeof(float));
				break;
			case 0x70:
				// getHarm
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				tempHarm = hRegisters[index1];

				nCount = (short) fRegisters[index2];

				while (nCount > 0 && tempHarm != NULL)
				{
					tempHarm = tempHarm->nextHarm;
					nCount--;
				}

				zero = (tempHarm == NULL);

				hRegisters[index3] = tempHarm;
				pCounter += 4;
				break;
			case 0x71:
				// getNote
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				tempNote = nRegisters[index1];

				if (debug)
				{
					fprintf(stderr, "getNote dumping n%d:\n", index1);
					nRegisters[index1]->dump();
				}

				nCount = (short) fRegisters[index2];

				while (nCount > 0 && tempNote != NULL)
				{
					tempNote = tempNote->nextNote;
					nCount--;
				}

				zero = (tempNote == NULL);

				nRegisters[index3] = tempNote;
				pCounter += 4;
				break;
			case 0x72:
				// degree
				pCounter += 4;
				break;
			case 0x73:
				// getNoteInChord
				index1 = getUChar(pCounter+1);
				index2 = getUChar(pCounter+2);
				index3 = getUChar(pCounter+3);

				tempNote = nRegisters[index1];

				if (debug)
				{
					fprintf(stderr, "getNoteInChord dumping n%d:\n", index1);
					nRegisters[index1]->dump();
				}

				nCount = (short) fRegisters[index2];

				if (tempNote != NULL)
				{
					fRegisters[FREGISTERS-1] = 0;

					lastPC = 0;

					while (nCount > 0)
					{
						tempNote = tempNote->nextNote;

						if (tempNote == NULL)
						{
							tempNote = nRegisters[index1];
						}

						if (tempNote->pitchClass < lastPC)
						{
							fRegisters[FREGISTERS-1] += 1.0;
							lastPC = 0;
						}
						else
						{
							lastPC = tempNote->pitchClass;
						}

						nCount--;
					}
				}

				nRegisters[index3] = tempNote;
				pCounter += 4;
				break;
			case 0x74:
				// importHarm
				arg1 = getFloat(pCounter+1);
				index2 = getUChar(pCounter+1+sizeof(float));

				if (arg1 < MAXTHREADS && pK != NULL &&
					((pKernel *)pK)->threads[(short)arg1] != NULL)
				{
					hRegisters[index2] = ((pKernel *)pK)->threads[(short)arg1]->harmList;
				}
				else
				{
					hRegisters[index2] = NULL;
					error = 6;
				}
				pCounter += 2+sizeof(float);
				break;
			case 0x75:
				// importNote
				if (debug)
				{
					fprintf(stderr, "Parent thread is %s\n", ((pThread *)pT)->name);
				}
				index1 = (int)getFloat(pCounter+1);
				index2 = getUChar(pCounter+sizeof(float)+1);

				if (pK == NULL)
				{
					if (debug)
					{
						fprintf(stderr, "error: Kernel is NULL.\n");
					}
					error = 1;
					pCounter+=6;
					break;
				}

				if ( ((pKernel *)pK)->threads[index1] == NULL)
				{
					if (debug)
					{
						fprintf(stderr, "Thread seems to be NULL.\n");
					}
					error = 2;
					pCounter+=6;
					break;
				}

				if (index1 < MAXTHREADS && pK != NULL &&
					((pKernel *)pK)->threads[index1] != NULL)
				{
					nRegisters[index2] = ((pKernel *)pK)->threads[index1]->noteList;
					if (debug)
					{
						fprintf(stderr, "Assignment to %d made\n", index1);
					}
				}
				else
				{
					//fprintf(stderr, "Failure for some reason...\n");
					nRegisters[index2] = NULL;
				}
				pCounter += (sizeof(float)+2);
				break;
			case 0x76:
				// getKey  (only works on the pitch class)

				index1 = getUChar(pCounter+1);

				fRegisters[index1] = (float) ((pKernel *)pK)->currentKey->pitchClass;
				pCounter += 2;
				break;
			case 0x77:
				// setKey (also on pitchClass right now)

				index1 = getUChar(pCounter+1);

				((pKernel *)pK)->currentKey->pitchClass = (short)fRegisters[index1];

				pCounter += 2;
				break;
			case 0x78:
				// outHarm
				pCounter += 3;
				break;
			case 0x79:
				// outNote
				index1 = getUChar(pCounter+1);

				if (nRegisters[index1] != NULL)
				{
					if (nRegisters[index1]->pitchClass == DEG)
					{
						nRegisters[index1]->pitchClass = nRegisters[index1]->degreePitchClass(pK, ((pKernel *)pK)->currentScale, UP);
					}
					((pThread *)pT)->out(nRegisters[index1]);
					totalBeats = totalBeats + nRegisters[index1]->getDurationF();
					elapsedTime += nRegisters[index1]->getDurationF();

					if (debug)
					{
						fprintf(stderr, "PC: %02d Duration: %5.5f Octave: %02d\n", nRegisters[index1]->pitchClass, nRegisters[index1]->fdur, nRegisters[index1]->octave);
					}
				}
				else
				{
					error = 3;
				}

				pCounter += 2;
				break;
			case 0x7a:
				// replaceHarm
				pCounter += 4;
				break;
			case 0x7b:
				// replaceNote
				pCounter += 4;
				break;
			case 0x7c:
				// getChordNotes

				index1 = (unsigned char) getUChar(pCounter+1);
				index2 = (unsigned char) getUChar(pCounter+2);

				if (hRegisters[index1])
				{
					nRegisters[index2] = hRegisters[index1]->getChordNotes(((pKernel *)pK)->currentKey, ((pKernel *)pK)->scales[((pKernel *)pK)->currentScale]);

					if (nRegisters[index2] == NULL)
					{
						error = 7;

						if (debug)
						{
							fprintf(stderr, "getChordNotes failed\n");
						}
					}
				}
				else
				{
					error = 5;
				}
				pCounter += 3;
				break;
			case 0x7d:
				// outNoteKey
				index1 = getUChar(pCounter+1);

				if (nRegisters[index1] != NULL)
				{
					no = nRegisters[index1]->octave;

					if (nRegisters[index1]->pitchClass != REST &&
						 nRegisters[index1]->pitchClass != DEG)
					{
						npc = nRegisters[index1]->pitchClass + ((pKernel *)pK)->currentKey->pitchClass;

						if (npc > 11)
						{
							no++;
							npc = npc%12;
						}
					}
					else
					{
						if (nRegisters[index1]->pitchClass == DEG)
						{
							//printf("Outputting degree-based note.\n");
							npc = nRegisters[index1]->degreePitchClass(pK, ((pKernel *)pK)->currentScale, UP) + ((pKernel *)pK)->currentKey->pitchClass;

							if (npc > 11)
							{
								no++;
								npc = npc%12;
							}
						}
						else
						{
							npc = REST;
						}
					}

					pNote *tN = new pNote(nRegisters[index1]->fdur, no, npc, 0);
					tN->clone(nRegisters[index1]);
					tN->pitchClass = npc;
					tN->octave = no;

					((pThread *)pT)->out(tN);
					delete tN;
					totalBeats = totalBeats + nRegisters[index1]->getDurationF();
					elapsedTime += nRegisters[index1]->getDurationF();

					if (debug)
					{
						fprintf(stderr, "PC: %02d Duration: %5.5f Octave: %02d\n", nRegisters[index1]->pitchClass, nRegisters[index1]->fdur, nRegisters[index1]->octave);
					}
				}
				else
				{
					error = 3;
				}

				pCounter += 2;
				break;
			case 0x7e:
				// activate
				index1 = getUChar(pCounter+1);

				if (((pKernel *)pK)->threads[index1] != NULL)
				{
					((pKernel *)pK)->threads[index1]->activate();
				}

				pCounter += 2;
				break;
			case 0x7f:
				// deactivate
				index1 = getUChar(pCounter+1);

				if (((pKernel *)pK)->threads[index1] != NULL)
				{
					((pKernel *)pK)->threads[index1]->deactivate();
				}

				pCounter += 2;
				break;
			case 0x80:
				// halt
				exit (1);
				break;
			case 0x81:
				// dumpFloats

				if (debug)
				{
					fprintf(stderr, ";Float registers:\n;");

					for (i=0; i<32; i++)
					{
						fprintf(stderr, "%02x:%2.4f\t", i, fRegisters[i]);

						if (i%4 == 3)
						{
							fprintf(stderr, "\n;");
						}
					}
					fprintf(stderr, "\n");
				}
				else
				{
					fprintf(stdout, ";Float registers:\n;");

					for (i=0; i<32; i++)
					{
						fprintf(stdout, "%02x:%2.4f\t", i, fRegisters[i]);

						if (i%4 == 3)
						{
							fprintf(stdout, "\n;");
						}
					}
					fprintf(stdout, "\n");
				}
				pCounter += 1;
				break;
			case 0x82:
				// dumpHarm

				index1 = getUChar(pCounter+1);

				if (hRegisters[index1] != NULL)
				{
					fprintf(stdout, "Dumping harmony %d\n", index1);

					hRegisters[index1]->dump(1);
				}
				else
				{
					fprintf(stdout, "Attempting to dump NULL harmony.\n");
				}

				pCounter += 2;
				break;
			case 0x83:
				// dumpNote

				index1 = getUChar(pCounter+1);

				if (nRegisters[index1] != NULL)
				{
					nRegisters[index1]->dump();
				}
				pCounter += 2;
			default:
				break;
		}
	}
	if (debug)
	{
		fprintf(stderr, "Finished executing.\n");
	}

	if (error)
	{
		fprintf(stderr, "Error %d occurred.\n", error);
	}
	totalBeats = totalBeats - timeSlice;
}

//-------------------------------
// get Address
// returns an address in a short
//-------------------------------

short pScript::getAddress(short p)
{
	/*
	short newShort;
	unsigned char *sArray;
	sArray = (unsigned char *) &newShort;

	int j;

	for (j=0;j<sizeof(short); j++)
	{
		sArray[j] = pCode[p+j];
	}

	newShort = *((short *) sArray);
	if (debug)
	{
		fprintf(stderr, "New short is %2.2f\n", newShort);
	}

	return newShort;
	*/

	return (short) (pCode[p] + pCode[p+1]*256);
}

//-------------------------------
// get Float
// returns a float
//-------------------------------

float pScript::getFloat(short p)
{
	float newFloat;
	unsigned char *fArray;
	//unsigned char fArray[8];
	fArray = (unsigned char *) &newFloat;

	//if (debug)
	//{
		//fprintf(stderr, "New float is %2.2f\n", newFloat);
	//}

	int j;

	for (j=0;j<sizeof(float); j++)
	{
		fArray[j] = pCode[p+j];
	//	if (debug)
	//	{
	//		fprintf(stderr, "%02x %02x\n", j, fArray[j]);
	//	}
	}

	newFloat = *((float *) fArray);
	if (debug)
	{
		fprintf(stderr, "New float is %2.2f\n", newFloat);
	}

	return newFloat;
}

//-------------------------------
// getUChar
// returns an unsigned char
//-------------------------------

unsigned char pScript::getUChar(short p)
{
	return pCode[p];
}

//-------------------------------
// compare
//-------------------------------

void pScript::compare(float a, float b)
{
	diff = a-b;

	// if this doesn't always work, we'll need
	// to use a tolerance

	if (diff < TOL && diff > -TOL)
	{
		zero = TRUE;
	}
	else
	{
		zero = FALSE;
	}

	return;
}
