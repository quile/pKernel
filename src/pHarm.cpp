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
// pHarm.cc
// pHarm implementation
//-----------------------------------

#include "../include/pHarm.hpp"
#include "../include/pKernel.hpp"
extern "C" {
#include "../include/pSystem.h"
#include "../include/pFile.h"
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//-----------------------------------
// pHarm::pHarm
// most basic constructor for the
// pHarm class
//-----------------------------------

pHarm::pHarm (char *d, short r, short i, short o)
{
	root = r;
	inversion = i;
	octave = o;
	setDuration(d);
	virt = FALSE;
	nextHarm = NULL;
	prevHarm = NULL;
	circle = NULL;
}

pHarm::pHarm (void)
{
	nextHarm = NULL;
	prevHarm = NULL;
	circle = NULL;
}

pHarm::pHarm (FILE *in)
{
	char inbuf[MAXCHARS];
	char *tokPtr;
	short tempRoot, tempQuality, tempAlteration;

	virt = FALSE;
	circle = NULL;
	nextHarm = NULL;
	prevHarm = NULL;
	octave = 0;
	inversion = 0;
	alteration = 0;
	root = 0;
	strcpy(duration, "");

	int i;
	for (i=0; i<12; i++)
	{
		chordTones[i] = NATURAL;
	}

	// set the third and fifth to defaults

	chordTones[1] = MAJOR;
	chordTones[2] = PERFECT;

	while (pfReadNext(in, inbuf, MAXCHARS))
	{
		if (!strncmp(inbuf, "endharm", 7))
		{
			if (debug)
			{
				fprintf(stderr, "=>End of harmony found\n");
			}
			break;
		}

		if (!strncmp(inbuf, "virtual", 7))
		{
			virt = TRUE;
			pfReadNext(in, inbuf, MAXCHARS);
		}

		// <duration> <inversion> <scale degree root> <fifth> <seventh> ...

		tokPtr = strtok(inbuf, " ");

		//fprintf(stderr, "First token: %s\n", tokPtr);

		// read duration

		setDuration(tokPtr);
		tokPtr = strtok(NULL, " ");

		//fprintf(stderr, "Second token: %s\n", tokPtr);
		// read inversion

		inversion = atoi(tokPtr);
		tokPtr = strtok(NULL, " ");
		//fprintf(stderr, "Third token: %s\n", tokPtr);

		// read root

		getChordInfo(tokPtr, &root, &(chordTones[1]), &alteration);

		size = 2;

		if (root == REST)
		{
			size = 1;
			return;
		}

		// read fifths, sevenths, ninths, etc.

		i = 2;

		while (tokPtr = strtok(NULL, " "))
		{
			if (!strcmp(tokPtr, "."))
			{
				if (i==2)
				{
					chordTones[i] = PERFECT;
				}
				else
				{
					chordTones[i] = MAJOR;
				}
			}
			else if (!strcmp(tokPtr, "-"))
			{
				chordTones[i] = MINOR;
			}
			else if (!strcmp(tokPtr, "+"))
			{
				chordTones[i] = AUGMENTED;
			}
			else if (!strcmp(tokPtr, "o"))
			{
				chordTones[i] = DIMINISHED;
			}
			else if (!strcmp(tokPtr, "++"))
			{
				chordTones[i] = DAUG;
			}
			else if (!strcmp(tokPtr, "oo"))
			{
				chordTones[i] = DDIM;
			}

			size++;
			i++;
		}
	}
}

//-----------------------------------
// pHarm::~pHarm
// destructor
//-----------------------------------

pHarm::~pHarm (void)
{
	if (debug)
	{
		fprintf(stderr, "Harmony is dying: ");
		dump(0);
	}

	if (nextHarm != NULL)
	{
		delete nextHarm;
	}

	if (circle != NULL)
	{
		delete circle;
	}
}

//-----------------------------------
// add
// adds a harmony to the list
//-----------------------------------

void pHarm::add(pHarm *newHarm)
{
	if (nextHarm == NULL)
	{
		if (debug)
		{
			fprintf(stderr, "   => Adding harmony to end of chain.\n");
		}

		nextHarm = newHarm;
		newHarm->nextHarm = NULL;
		newHarm->prevHarm = this;
	}
	else
	{
		if (debug)
		{
			fprintf(stderr, "   => Looking for end of chain.\n");
		}

		nextHarm->add(newHarm);
	}
}

//-------------------------------------
// setScale
// sets the scale pointer of this
// particular pHarm to point to one
// of the valid scales in the pKernel
//-------------------------------------

void pHarm::setScale (void *pk, short scaleIndex)
{
	pKernel *pkPtr = (pKernel *)pk;

	if (scaleIndex < pkPtr->numScales)
	{
		scale = pkPtr->scales[scaleIndex];
	}
}

//---------------------------------------
// getChordInfo
// given a token, it figures out the
// root of the chord and the quality
//---------------------------------------

void pHarm::getChordInfo (char *chordTok, short *r, short *q, short *a)
{
	// r is root (roman numeral)
	// q is quality of triad (major or minor)
	// a is alteration in semitones to root

	*a = 0;
	*r = 0;

	if (strchr(chordTok, 'I') || strchr(chordTok, 'V'))
	{
		*q = MAJOR;
	}
	else
	{
		*q = MINOR;
	}

	char *tempPtr;

	tempPtr = chordTok;

	while (*tempPtr)
	{
		switch (*tempPtr)
		{
			case 'i':
			case 'I':
				*r = *r + 1;
				break;
			case 'v':
			case 'V':
				if (*r == 0)
				{
					*r = 5;
				}
				else
				{
					*r = 5 - *r;
				}
				break;
			case '-':
				*a = *a - 1;
				break;
			case '#':
				*a = *a + 1;
				break;
			case '0':
				*r = REST;
				break;
			default:
				break;
		}
		tempPtr++;
	}
}

//--------------------------------------
// dump
//--------------------------------------

void pHarm::dump (short recursive)
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

	fprintf(output, "Duration (c) %s (f) %2f ", duration, fdur);

	fprintf(output, "Chord is ");

	if (root == REST)
	{
		fprintf(output, "a rest.\n");

		if (recursive && nextHarm)
		{
			nextHarm->dump(recursive);
		}

		return;
	}

	if (alteration <= -2 || alteration >= 2)
	{
		fprintf(output, "doubly ");
	}

	if (alteration < 0)
	{
		fprintf(output, "lowered ");
	}
	else if (alteration > 0)
	{
		fprintf(output, "raised ");
	}

	if (chordTones[1] == MAJOR)
	{
		fprintf(output, "major ");
	}
	else
	{
		fprintf(output, "minor ");
	}

	fprintf(output, "%d ", root);

	if (inversion == 0)
	{
		fprintf(output, "in root position.\n");
	}
	else if (inversion == 1)
	{
		fprintf(output, "in 1st inversion.\n");
	}
	else if (inversion == 2)
	{
		fprintf(output, "in 2nd inversion.\n");
	}
	else if (inversion == 3)
	{
		fprintf(output, "in 3rd inversion.\n");
	}
	else
	{
		fprintf(output, "in %dth inversion.\n", inversion);
	}

	if (size > 3)
	{
		int tc=0;

		for (tc=2;tc<size; tc++)
		{
			fprintf(output, "Interval %d is %d\n", tc, chordTones[tc]);
		}
	}

	if (nextHarm != NULL && recursive)
	{
		nextHarm->dump(recursive);
	}
}

//--------------------------------------
// getChordNotes
// given a key and a scale, this
// returns a linked list of notes
// that make up the chord.  Of course,
// the inversion information must be
// requested as well in order to fully
// represent this harmony
//--------------------------------------

pNote *pHarm::getChordNotes(pNote *currentKey, pScale *currentScale)
{
	short pitchClasses[12];
	short octaves[12];
	short rootIndex;

	// set up the root

	if (root == REST)
	{
		pitchClasses[0] = REST;
		octaves[0] = 0;
		size=1;
	}
	else
	{
		rootIndex = root-1;
		pitchClasses[0] = currentScale->degree(rootIndex, UP) + alteration;
		octaves[0] = 0;
	}

	// now set up the stacked thirds

	short i;
	short basePC;
	short baseDegree;

	for (i=1; i<size; i++)
	{
		// figure out the quality of the interval and
		// work from there

		if (!virt)
		{
			switch (i)
			{
				case 1:
					basePC = alter(pitchClasses[0] + 4, chordTones[i], 0);
					break;
				case 3:
					basePC = alter(pitchClasses[0] + 11, chordTones[i], 0);
					break;
				case 4:
					basePC = alter(pitchClasses[0] + 14, chordTones[i], 0);
					break;
				case 6:
					basePC = alter(pitchClasses[0] + 21, chordTones[i], 0);
					break;
				case 2:
					basePC = alter(pitchClasses[0] + 7, chordTones[i], 1);
					break;
				case 5:
					basePC = alter(pitchClasses[0] + 17, chordTones[i], 1);
					break;
				default:
					basePC = pitchClasses[0];
					break;
			}
		}
		else
		{
			baseDegree = (rootIndex + 2*i)%(currentScale->getLength(UP));
			basePC = currentScale->degree( baseDegree, UP);
		}

		pitchClasses[i] = basePC;


		if (pitchClasses[i] < pitchClasses[i-1])
		{
			octaves[i] = octaves[i-1] + 1;
		}
		else
		{
			octaves[i] = octaves[i-1];
		}
	}

	// and now create the notes from the pitchClass info

	// create the root:

	pNote *rootNote = new pNote(duration, octaves[0], pitchClasses[0], 0);
	pNote *newNote;

	for (i=1; i<size; i++)
	{
		newNote = new pNote(duration, octaves[i], pitchClasses[i], 0);

		rootNote->add(newNote);
	}

	return rootNote;
}

//---------------------------------
// alter
// alters an interval
//---------------------------------

short pHarm::alter(short pc, short a, short itype)
{
	if (itype == 0)
	{
		// this interval type is used for thirds, sevenths,
		// ninths and thirteenths : intervals that are
		// major/minor/dim/aug

		switch (a)
		{
			case MAJOR:
				// do nothing
				break;
			case MINOR:
				// decrease it
				pc--;
				break;
			case DIM:
				// decrease it twice
				pc -=2;
				break;
			case DDIM:
				// decrease it thrice
				pc -=3;
				break;
			case AUG:
				// increase it once
				pc++;
				break;
			case DAUG:
				// increase it twice
				pc += 2;
				break;
			default:
				break;
		}
	}
	else
	{
		// this is for fifths and elevenths : intervals
		// that are perfect/dim/aug

		switch (a)
		{
			case PERFECT:
				// do nothing
				break;
			case AUG:
				// increase it once
				pc++;
				break;
			case DAUG:
				// increase it twice
				pc += 2;
				break;
			case DIM:
			case MINOR:
				// decrease it once
				pc --;
				break;
			case DDIM:
				// decrease it twice
				pc -= 2;
				break;
			default:
				break;
		}
	}

	return (pc%12);
}

//---------------------------------
// setDuration
//---------------------------------

void pHarm::setDuration(char *d)
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

	//fprintf(stderr, "Note duration is %2f\n", fdur);
	return;
}

//--------------------------------------
// setDurationF : this is the tricky
// one, where we need to figure out the
// string representation of the note
// from the float value
// and it only works for notes that are
// reciprocal powers of 2...  we need
// a better solution to this problem
// fast.
//--------------------------------------

void pHarm::setDurationF(float d)
{
	float remain = d;
	float nVal = 1.0;
	char dString[12];
	short sOffset = 0;
	short first = 0;

	while (remain > TOL)
	{
		if (d > nVal)
		{
			// value is the basic note value so
			// output it and increment

			if (first == 0)
			{
				sprintf(dString+sOffset, "%1.0f", (1.0/nVal));
				first = 1;
			}
			else
			{
				dString[sOffset] = '.';
			}

			sOffset += 1;
			remain = remain - nVal;
		}

		nVal = nVal / 2;
	}

	dString[sOffset] = '\0';

	strcpy(duration, dString);
}

//-----------------------------------
// getDuration
// Two versions of this: one returns
// the string and one returns the
// number of beats
//-----------------------------------

char *pHarm::getDurationC (void)
{
	return duration;
}

float pHarm::getDurationF (void)
{
	return fdur;
}

//---------------------------------------
// clone
// this should be done in a copy
// contructor but right now it seems not
// to work
//---------------------------------------

void pHarm::clone (pHarm *old)
{
	root = old->root;
	size = old->size;
	inversion = old->inversion;
	alteration = old->alteration;
	octave = old->octave;
	strcpy(duration, old->duration);
	fdur = old->fdur;
	quality = old->quality;

	int i;
	for (i=0; i<12; i++)
	{
		chordTones[i] = old->chordTones[i];
	}

	// have to copy the circle param. Do it later...

	circle = NULL;
	//nextHarm = old->nextHarm;
	//prevHarm = old->prevHarm;
	scale = old->scale;
}

//----------------------------------------
// set[]
// sets the [] of the chord
//----------------------------------------

void pHarm::setRoot(short r)
{
	if (r<12)
	{
		root = r;
	}
}

void pHarm::setInversion(short i)
{
	inversion = i;
}

void pHarm::setOctave(short o)
{
	octave = o;
}

void pHarm::setAlteration(short a)
{
	alteration = a;
}

short pHarm::getRoot(void)
{
	return root;
}

short pHarm::getOctave(void)
{
	return octave;
}

short pHarm::getInversion(void)
{
	return inversion;
}

short pHarm::getAlteration(void)
{
	return alteration;
}
