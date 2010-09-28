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
// pThread.cpp
// The method definitions for the
// pThread class
//-------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/pNote.hpp"
#include "../include/pHarm.hpp"
#include "../include/pScript.hpp"
#include "../include/pCompiler.hpp"

extern "C" {
#include "../include/pSystem.h"
#include "../include/pFile.h"
}

#include "../include/pKernel.hpp"
#include "../include/pThread.hpp"

//-------------------------------------
// most basic constructor:
//-------------------------------------

pThread::pThread (char *n, short th)
{
	if (strlen(n) < 32)
	{
		strcpy(name, n);
	}
	else
	{
		strcpy(name, "<null>");
	}

	// set up harmonic structure

	harmList = NULL;

	// set the threadId

	threadId = th;

	// set up noteLists

	noteList = NULL;
	outList = NULL;

	// set up scripts

	scriptList = NULL;

	// initialise time offset

	timeOffset = noteList;
	totalBeats = 0.0;
	active = FALSE;

	if (debug)
	{
		fprintf(stderr, "New pThread object %s\n", name);
	}
}

pThread::pThread (FILE *in, pCompiler *c, void *k, short th)
{
	char inbuf[MAXCHARS];

	setKernel(k);
	compiler = c;
	harmList = NULL;
	scriptList = NULL;
	noteList = NULL;
	outList = NULL;

	threadId = th;

	pfReadNext(in, name, MAXCHARS);

	while (pfReadNext(in, inbuf, MAXCHARS))
	{
		if (!strncmp(inbuf, "endthread", 9))
		{
			break;
		}

		// reading in a note:

		if (!strncmp(inbuf, "note", 4))
		{
			pNote *newNote = new pNote(in, threadId);

			if (noteList == NULL)
			{
				noteList = newNote;
			}
			else
			{
				noteList->add(newNote);
			}
		}
		else if (!strncmp(inbuf, "harm", 4))
		{
			if (debug)
			{
				fprintf(stderr, "   => New harmony found\n");
			}
			pHarm *newHarm = new pHarm(in);
			newHarm->nextHarm = NULL;
			newHarm->prevHarm = NULL;

			if (harmList == NULL)
			{
				harmList = newHarm;
			}
			else
			{
				harmList->add(newHarm);
			}

			if (debug)
			{
				newHarm->dump(0);
			}
		}
		else if (!strncmp(inbuf, "script", 4))
		{
			if (debug)
			{
				fprintf(stderr, "   => New script found\n");
			}
			pScript *newScript = compiler->compile(in);

			if (newScript != NULL)
			{
				// set the script's internal references
				newScript->setThread(this);
				newScript->setKernel(pK);

				sscanf (inbuf, "script %s", newScript->name);
				if (strlen(newScript->name) == 0)
				{
					fprintf(stderr, "      => Error: script has no name\n");
				}
				newScript->nextScript = NULL;
				newScript->prevScript = NULL;

				if (scriptList == NULL)
				{
					scriptList = newScript;
				}
				else
				{
					scriptList->add(newScript);
				}
				scriptList->dump(1);
			}
		}
	}

	/*
	if (harmList != NULL)
	{
		if (debug)
		{
			fprintf (stderr, "Harmony list is:\n");
		}
		harmList->dump(1);
	}
	*/

	timeOffset = noteList;
	harmOffset = harmList;
	totalBeats = 0.0;
	active = FALSE;
}

//-------------------------------------
// destructor
//-------------------------------------

pThread::~pThread (void)
{
	if (debug)
	{
		fprintf(stderr, "Destroying pThread object %s\n", name);
	}

	if (noteList != NULL)
	{
		delete (noteList);
	}

	if (harmList != NULL)
	{
		delete (harmList);
	}

	if (scriptList != NULL)
	{
		delete (scriptList);
	}
}

//-------------------------------------
// setKernel
// sets the pThread's parent pKernel
// pointer
//-------------------------------------

void pThread::setKernel(void *k)
{
	pK = k;

	if (debug)
	{
		fprintf (stderr, "Setting kernel to %08x\n", (long)k);
	}
}

//-------------------------------------
// think method
// is used for the pThread to do its
// thinking, generate new content,
// adapt to current events, etc.
//-------------------------------------

void pThread::think(short beats, short beat)
{
	if (debug)
	{
		fprintf(stderr, "pThread %s thinking for %d beats...\n", name, beats);
	}

	if (outList != NULL)
	{
		//fprintf(stderr, "Freeing outList...\n");
		free (outList);
		outList = NULL;
	}

	// debugging

	if (beats == 0)
	{
		if (noteList != NULL)
		{
			noteList->dump();
		}
	}

	// check if the thread is allowed to think:

	if (!active)
	{
		return;
	}

	if (timeOffset == NULL)
	{
		timeOffset = noteList;
	}

	if (scriptList == NULL)
	{
		// there are no scripts so just echo the data

		if (noteList == NULL)
		{
			if (harmList == NULL)
			{
				// AND there's no harmonic data

				return;
			}
			else
			{
				// pThread has ONLY harmonic data

				pNote *currentChord = NULL;

				// get the current chord
			}

			// there is no note data

			return;
		}

		// there is note data so loop through

		float timeSlice = (float)beats/beat;
		while (totalBeats < (float)timeSlice)
		{
			out(timeOffset);
			totalBeats = totalBeats + (float) timeOffset->getDurationF();

			timeOffset = timeOffset->nextNote;

			if (timeOffset == NULL)
			{
				timeOffset = noteList;
			}
		}
		totalBeats = totalBeats - timeSlice;
	}
	else
	{
		// there are scripts so run any appropriate ones

		scriptList->go (beats, beat);

		if (debug)
		{
			fprintf(stderr, "Script returned.\n");
			fflush(stderr);
		}
	}
}

//------------------------------------------------------------------
// this is the harmonic version of the think() method. It doesn't
// send output anywhere; it simply generates harmonic lookahead and
// puts it in the pKernel's harmBuffer.
//------------------------------------------------------------------

void pThread::think (pHarm **harmBuffers, short laBuffer, short beats, short beat)
{
	if (debug)
	{
		fprintf(stderr, "pThread %s thinking for %d beats to generate harmonic information\n", name, beats);
	}

	if (outList != NULL)
	{
		free (outList);
		outList = NULL;
	}

	// debugging

	if (beats == 0)
	{
		if (noteList != NULL)
		{
			harmList->dump(1);
		}
	}

	if (harmOffset == NULL)
	{
		harmOffset = harmList;
	}

	// ? i dunno
	//scriptList = NULL;

	if (scriptList == NULL)
	{
		// there are no scripts so just echo the data

		if (harmList == NULL)
		{
			// AND there's no harmonic data

			return;
		}

		// there is harm data so loop through

		//fprintf(stderr, "For debugging purposes, here is the harmony list:\n");
		//harmList->dump(1);
		//fflush(stdout);

		float timeSlice = (float)beats/beat;
		while (totalBeats < (float)timeSlice)
		{
			//fprintf(stderr, "Total beats : %2f\n", totalBeats);
			//harmOffset->dump(0);

			// here we have to create a copy of the harmony and stick it
			// into the right place in the lookahead buffer

			pHarm *copy = new pHarm();

			// clone the current harmony into the copy

			copy->clone(harmOffset);

			copy->nextHarm = NULL;
			copy->prevHarm = NULL;

			// debugging:

			//fprintf(stderr, "Debug original: ");
			//harmOffset->dump(0);
			//fflush(stdout);
			//fprintf(stderr, "Debug copy: ");
			//copy->dump(0);
			//fflush(stdout);

			if (harmBuffers[laBuffer] == NULL)
			{
				harmBuffers[laBuffer] = copy;
			}
			else
			{
				harmBuffers[laBuffer]->add(copy);
			}

			totalBeats = totalBeats + (float) harmOffset->getDurationF();
			//fprintf(stderr, "Note length is : %2f\n", harmOffset->getDurationF());

			harmOffset = harmOffset->nextHarm;

			if (harmOffset == NULL)
			{
				harmOffset = harmList;
			}
		}

		if (debug)
		{
			fprintf(stderr, "Harmony lookahead buffer:\n");

		}
		harmBuffers[laBuffer]->dump(1);

		totalBeats = totalBeats - timeSlice;
	}
	else
	{
		if (debug)
		{
			fprintf(stderr, "Scripts:\n");
		}
		scriptList->dump(1);

		scriptList->go(beats, beat);
	}
}

//----------------------------------------
// activate:
// sets the thread to be active for the
// next think()
//----------------------------------------

void pThread::activate(void)
{
	active = TRUE;
}

//----------------------------------------
// deactivate:
// sets the thread to be inactive for the
// next think()
//----------------------------------------

void pThread::deactivate(void)
{
	active = FALSE;
}

//----------------------------------------
// reset:
// resets the thread to its initial state
//----------------------------------------

void pThread::reset(void)
{
	timeOffset = noteList;
	totalBeats = 0.0;
	active = FALSE;

	if (outList != NULL)
	{
		free (outList);
	}
}

//----------------------------------------
// out:
// This function is responsible for
// sending the note back to the pKernel
// although at present it's just dumped.
//----------------------------------------

void pThread::out(pNote *n)
{
	//printf ("Note: %2.2f %d %d\n", n->fdur, n->octave, n->pitchClass);

	pNote *newNote = new pNote(n->fdur, n->octave, n->pitchClass, threadId);

	newNote->clone(n);
	newNote->thread = threadId;

	if (outList == NULL)
	{
		outList = newNote;
		newNote->nextNote = NULL;
	}
	else
	{
		outList->add(newNote);
	}
}
