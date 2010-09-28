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
// pKernel.cpp
// The method definitions for the
// pKernel class
//-------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define __I_AM_PKERNEL__

#include "../include/pThread.hpp"
#include "../include/pHarm.hpp"
#include "../include/pScale.hpp"
#include "../include/pCompiler.hpp"

extern "C" {
#include "../include/pSystem.h"
#include "../include/pFile.h"
}

#include "../include/pKernel.hpp"

pKernel::pKernel (void)
{
	if (debug)
	{
		fprintf(stderr,"New pKernel object\n");
	}

	numThreads = 0;
	eventList = NULL;

	int i;

	for (i=0; i<MAXTHREADS; i++)
	{
		threads[i] = NULL;
	}

	hStructure = NULL;
	//printf ("hStructure is %x\n", (long)hStructure);

	numScales = 0;

	for (i=0; i<MAXSCALES; i++)
	{
		scales[i] = NULL;
	}

	// standard scales: major, melodic minor, harmonic minor

	scales[0] = new pScale("0 2 4 5 7 9 11", UP);
	scales[1] = new pScale("0 2 3 5 7 9 11", UP);
	scales[1]->setScale("0 2 3 5 7 8 10", DOWN);
	scales[2] = new pScale("0 2 3 5 7 8 11", UP);
	numScales = 3;

	lookahead[0] = NULL;
	lookahead[1] = NULL;

	// create a pCompiler for the purposes of compiling
	// incoming scripts

#ifdef WIN32
	compiler = new pCompiler("..\\byteCodes");
#else
	compiler = new pCompiler("byteCodes");
#endif
}

pKernel::~pKernel (void)
{
	if (debug)
	{
		fprintf(stderr, "Destroying pKernel object\n");
	}

	int i;

	for (i=0; i< MAXTHREADS && threads[i] != NULL; i++)
	{
		delete threads[i];
	}

	for (i=0; i<MAXSCALES && scales[i] != NULL; i++)
	{
		delete scales[i];
	}
}

void pKernel::load(char *inputFile)
{
	// load in the system

	FILE *in;
	char inbuf[MAXCHARS];
	char threadNumber[4];
	short threadIndex;

	in = fopen(inputFile, "r");

	if (in)
	{
		// first read the basic information like
		// name of system, key, etc.

		pfReadNext(in, name, MAXCHARS);
		pfReadNext(in, key, MAXCHARS);

		// Create a new note

		currentKey = new pNote(0.0, 0, atoi(key), -1);

		if (strchr(key, '-'))
		{
			currentScale = HARMONICMINOR;
		}
		else
		{
			currentScale = IONIAN;
		}

		// Read in the tempo

		pfReadNext(in, inbuf, MAXCHARS);
		tempo = atoi(inbuf);

		// create a new event

		pEvent *newEvent = new pEvent(PEVTEMPO, tempo);

		if (eventList == NULL)
		{
			eventList = newEvent;
		}
		else
		{
			eventList->add(newEvent);
		}

		// add the name

		eventList->add(new pEvent(PEVCOMMENT, name));

		if (debug)
		{
			fprintf(stderr, "=> Tempo is %d\n", tempo);
		}

		pfReadNext(in, inbuf, MAXCHARS);
		beat = atoi(inbuf);

		if (debug)
		{
			fprintf (stderr, "=> Beat is %dth notes\n", beat);
		}

		// now read the harmonic stuff and the threads themselves

		while (pfReadNext(in, inbuf, MAXCHARS))
		{
			if (!strncmp(inbuf, "harm", 4))
			{
				// read in harmonic info
				if (debug)
				{
					fprintf(stderr, "=> Reading new harmony\n");
				}

				pHarm *newHarm = new pHarm(in);

				//printf ("hStructure is %x\n", (long)hStructure);

				if (hStructure == NULL)
				{
					if (debug)
					{
						fprintf(stderr, "=>Initialising harmony list\n");
					}
					hStructure = newHarm;
				}
				else
				{
					if (debug)
					{
						fprintf(stderr, "=>Adding new harmony to list\n");
					}
					hStructure->add(newHarm);
				}

				if (debug)
				{
					newHarm->dump(0);
				}
			}
			else if (!strncmp(inbuf, "thread", 6))
			{
				// read in a thread
				if (debug)
				{
					fprintf(stderr, "=>Reading new thread...\n");
				}

				pThread *newThread = new pThread(in, compiler, (void *)this, numThreads);

				if (numThreads < MAXTHREADS)
				{
					threads[numThreads] = newThread;
					if (debug)
					{
						fprintf(stderr, "Setting kernel.\n");
					}
					numThreads++;
				}
			}
			else if (!strncmp(inbuf, "scale", 5))
			{
				// a scale
				if (debug)
				{
					fprintf(stderr, "=>Reading new scale...\n");
				}

				pScale *newScale = new pScale(in);

				if (debug)
				{
					newScale->dump();
				}

				if (numScales < MAXSCALES)
				{
					scales[numScales] = newScale;
					numScales++;
				}
			}
			else if (!strncmp(inbuf, "active", 6))
			{
				// Activates a thread initially

				sscanf(inbuf, "active %s", threadNumber);
				threadIndex = atoi(threadNumber);

				if (threads[threadIndex] != NULL)
				{
					threads[threadIndex]->activate();
				}
			}
		}

		fclose (in);
	}
	else
	{
		fprintf(stderr, "   => Error reading from input file %s\n", inputFile);
		exit (1);
	}
}

void pKernel::init(short beats, void (*oFunc)(pNote *), void (*eFunc)(pEvent *))
{
	// do nothing

	// This has to be called after loading the data in and
	// before the first go() call

	// fill the lookahead buffer

	laBuffer = 0;
	out = oFunc;
	event = eFunc;

	// set the initial values of the iRegisters

	for (int t=0; t<IREGISTERS; t++)
	{
		iRegisters[t] = 0;
	}

	// dump the harmony list

	//fprintf(stderr, "Harmony list is now:\n");

	//threads[0]->harmList->dump(1);

	if (threads[0] != NULL)
	{
		threads[0]->think(lookahead, laBuffer, beats, beat);
		laBuffer = 1;
	}
	else
	{
		exit (1);
	}

}

void pKernel::go(void)
{
	// main section of pKernel

	int i;

	for (i=0; i<MAXTHREADS && threads[i] != NULL; i++)
	{
		threads[i]->think(0, 1);
	}
}

//---------------------------------------
// go
// given a number of beats, pass
// that to all active threads and
// then return the number of
// microseconds to wait until the next
// call to "go" based on the current
// tempo
//---------------------------------------

long pKernel::go(short beats)
{
	// main section of pKernel

	int i;

	// first think about the harmonic motion:

	if (threads[0] != NULL)
	{
		// clear the old info
		delete lookahead[laBuffer];
		lookahead[laBuffer] = NULL;

		// get the new info
		threads[0]->think(lookahead, laBuffer, beats, beat);
		//lookahead[laBuffer]->dump(1);

		// switch buffers
		if (laBuffer == 0)
		{
			laBuffer = 1;
		}
		else
		{
			laBuffer = 0;
		}
	}

	memcpy (iRegistersOut, iRegisters, IREGISTERS*sizeof(short));

	for (i=1; i<MAXTHREADS && threads[i] != NULL; i++)
	{
		threads[i]->think(beats, beat);
	}

	// now the gathering phase

	gather();

	memcpy (iRegisters, iRegistersOut, IREGISTERS*sizeof(short));

	return (long) (float) (beats * (float)((60.0/tempo)) * 1000000.0);
}

//-----------------------------------------
// gather()
// takes the output from all the
// threads and assembles it into a noteList
// that is a bit more easy to manage
//-----------------------------------------

void pKernel::gather(void)
{
	short i;

	if (eventList != NULL)
	{
		event(eventList);
		delete eventList;
		eventList = NULL;
	}

	for (i=0; i<MAXTHREADS && threads[i] != NULL; i++)
	{
		if (debug)
		{
			fprintf(stderr, "Thread %d\n", i);
			fflush(stderr);
		}

		if (threads[i]->outList != NULL)
		{
			out(threads[i]->outList);
			delete (threads[i]->outList);
			threads[i]->outList = NULL;
		}
	}

	// and now the closer:

	out(NULL);
}
