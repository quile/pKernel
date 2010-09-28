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

//-----------------------------------
// pNote.cc
// pNote implementation
//-----------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/pNote.hpp"

extern "C" {
#include "../include/pSystem.h"
#include "../include/pFile.h"
}

#include "../include/pKernel.hpp"

//-----------------------------------
// pNote::pNote
// most basic constructor for the
// pNote class
//-----------------------------------

pNote::pNote (char *d, short o, short pc, short th)
{
	setDuration(d);
	octave = o;
	pitchClass = pc;
	thread = th;

	int i;
	for (i=0; i<MAXSCALES; i++)
	{
		alteration[i] = 0;
	}
	prevNote = NULL;
	nextNote = NULL;
}

pNote::pNote (float d, short o, short pc, short th)
{
	fdur = d;
	octave = o;
	pitchClass = pc;
	thread = th;

	int i;
	for (i=0; i<MAXSCALES; i++)
	{
		alteration[i] = 0;
	}

	for (i=0; i<MAXBAGGAGE; i++)
	{
		baggage[i] = 0;
	}

	prevNote = NULL;
	nextNote = NULL;
}

pNote::pNote(void)
{

	int i;
	for (i=0; i<MAXSCALES; i++)
	{
		alteration[i] = 0;
	}

	for (i=0; i<MAXBAGGAGE; i++)
	{
		baggage[i] = 0;
	}

	prevNote = NULL;
	nextNote = NULL;
	thread = 0;
}

pNote::pNote(FILE *in, short th)
{
	char inbuf[MAXCHARS];
	char durTok[10];
	char octTok[10];
	char pcTok[10];
	short aindex;
	char *ptr, *ptr2;

	int i;
	for (i=0; i<MAXSCALES; i++)
	{
		alteration[i] = 0;
	}

	for (i=0; i<MAXBAGGAGE; i++)
	{
		baggage[i] = 0;
	}


	if ( pfReadNext(in, inbuf, MAXCHARS) )
	{
		// for now, just read this in and set the variables.
		// later we may want to read more lines.

		sscanf(inbuf, "%s %s %s", durTok, octTok, pcTok);

		setDuration(durTok);
		octave = atoi(octTok);

		if (pcTok[0] != 'd')
		{
			pitchClass = atoi(pcTok);
		}
		else
		{
			pitchClass = DEG;

			degree = atoi(pcTok+1);

			// return the first token
			ptr = strtok(inbuf, " ");
			// then the second
			ptr = strtok(NULL, " ");
			// then the third
			ptr = strtok(NULL, " ");

			// now go through all the others

			while (ptr = strtok(NULL, " "))
			{
				aindex = atoi(ptr);

				alteration[aindex] = 0;

				ptr2 = ptr;

				while (*ptr2)
				{
					switch (*ptr2)
					{
						case '+':
						case '#':
							alteration[aindex]++;
							break;
						case '-':
						case 'b':
							alteration[aindex]--;
							break;
						default:
							break;
					}
					ptr2++;
				}
			}
		}

		if (debug)
		{
			fprintf(stderr, "D%s O%d PC%d\n", duration, octave, pitchClass);
		}
	}

	// that's it for now

	prevNote = NULL;
	nextNote = NULL;
}

//-----------------------------------
// pNote::~pNote
// destructor
// only allowed to destroy notes
// that follow it in the list if
// we're the beginning of the list.
//-----------------------------------

pNote::~pNote (void)
{
	if (prevNote == NULL)
	{
		if (nextNote != NULL)
		{
			nextNote->prevNote = NULL;
			delete nextNote;
		}
	}

	/*
	if (debug)
	{
		fprintf(stderr, "Note is dying: ");
		dump();
	}
	else
	{
		printf("Note is dying:");
		dump();
	}
	*/
}

//-----------------------------------
// clone : copies data from one note
// to this one
//-----------------------------------

void pNote::clone (pNote *old)
{
	octave = old->octave;
	pitchClass = old->pitchClass;
	strcpy(duration, old->duration);
	fdur = old->fdur;
	thread = old->thread;

	int i;
	for (i=0; i<MAXSCALES; i++)
	{
		alteration[i] = old->alteration[i];
	}
	for (i=0; i<MAXBAGGAGE; i++)
	{
		baggage[i] = old->baggage[i];
	}
	degree = old->degree;

	nextNote = NULL;
	prevNote = NULL;
}

//-----------------------------------
// adds a note to the list
//-----------------------------------

void pNote::add(pNote *newNote)
{
	if (nextNote == NULL)
	{
		nextNote = newNote;
		newNote->prevNote = this;
		newNote->nextNote = NULL;
	}
	else
	{
		nextNote->add(newNote);
	}
}

//-----------------------------------
// dump
//-----------------------------------

void pNote::dump (void)
{
	FILE *output;

	if (!debug)
	{
		output = stdout;
	}
	else
	{
		output = stderr;
	}

	fprintf(output, "%d: %2.4f %d %d\n", thread, fdur, octave, pitchClass);

	if (nextNote != NULL)
	{
		nextNote->dump();
	}
}

//------------------------------------
// out
//------------------------------------

void pNote::out(void)
{
	fprintf(stderr, "%s %d %d\n", duration, octave, pitchClass);
}

//------------------------------------
// getDuration
//------------------------------------

float pNote::getDurationF(void)
{
	return (float) fdur;
}

char *pNote::getDurationC(void)
{
	return duration;
}

//---------------------------------
// setDuration
//---------------------------------

void pNote::setDuration(char *d)
{
	if (d == NULL || strlen(d) > 11)
	{
		strcpy (duration, "");
		fdur = 0.0;
		return;
	}

	// d is an OK string

	strcpy (duration, d);

	short tdur = atoi(d);

	// if the token is not a zero, then everything's
	// fine. Otherwise we have to avoid a division by
	// zero and handle the "breve" case

	if (tdur != 0)
	{
		fdur = 1.0 / (float)tdur;
	}
	else
	{
		fdur = 2.0;
	}

	float dot = fdur / 2;

	int i;

	for (i=0; i<strlen(duration); i++)
	{
		if (duration[i] == '.')
		{
			fdur += dot;
			dot = dot / 2;
		}
	}
	return;
}

//------------------------------------------
// degreePitchClass : returns the pitch
// class of the note based on the
// scale, the degree and the alteration
// of the note.  Remember that key is
// NOT taken into account because that is
// dealt with during the output phase
// at the moment
//------------------------------------------

short pNote::degreePitchClass(pScale *scale, short dir)
{
	short rpc;

	rpc = scale->degree( (degree-1), dir) + alteration[0];

	return rpc;
}

short pNote::degreePitchClass(void *pK, short si, short dir)
{
	short rpc;

	rpc = ((pKernel *)pK)->scales[si]->degree( (degree-1), dir) + alteration[si];

	return rpc;
}
