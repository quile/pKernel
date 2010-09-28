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
// pScale.cc
// pScale implementation
//-----------------------------------

#include "../include/pScale.hpp"
#include "../include/pSystem.h"

extern "C" {
#include "../include/pFile.h"
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//-----------------------------------
// pScale::pScale
// most basic constructor for the
// pScale class
//-----------------------------------

pScale::pScale (char *scale, short direction)
{
	if (direction == UP)
	{
		tokenise(scale, degreesUp, &lengthUp);
	}
	else
	{
		tokenise(scale, degreesDown, &lengthDown);
	}
}

pScale::pScale (FILE *in)
{
	char inbuf[MAXCHARS];

	if (pfReadNext(in, inbuf, MAXCHARS))
	{
		if (!strncmp(inbuf, "endscale", 8))
		{
			return;
		}
		else
		{
			tokenise(inbuf, degreesUp, &lengthUp);
		}
	}

	if (pfReadNext(in, inbuf, MAXCHARS))
	{
		if (!strncmp(inbuf, "endscale", 8))
		{
			return;
		}
		else
		{
			tokenise(inbuf, degreesDown, &lengthDown);
		}
	}
}

//-----------------------------------
// pScale::~pScale
// destructor
//-----------------------------------

pScale::~pScale (void)
{
}

//-----------------------------------
// setScale
// sets the scale
//-----------------------------------

void pScale::setScale (char *s, short dir)
{
	if (dir == UP)
	{
		tokenise(s, degreesUp, &lengthUp);
	}
	else
	{
		tokenise(s, degreesDown, &lengthDown);
	}
}

//-----------------------------------
// degree
// returns the nth scale degree in
// pitch class
//-----------------------------------

short pScale::degree (short n, short direction)
{
	short deg=0;

	if (direction == UP)
	{
		if (n<lengthUp)
		{
			deg = degreesUp[n];
		}
	}
	else
	{
		if (n<lengthDown)
		{
			deg = degreesDown[n];
		}
	}

	return deg;
}

//---------------------------------------
// tokenise
// given a string of PCs separated by
// spaces, this reads them into a given
// array, sets the array size and then
// returns
//---------------------------------------

void pScale::tokenise (char *list, short *pcArray, short *arraySize)
{
	char strBuf[MAXCHARS];
	char *ptr;
	short index;

	strcpy(strBuf, list);		// because strtok is destructive

	index = 0;

	if (ptr = strtok(strBuf, " "))
	{
		do
		{
			pcArray[index] = atoi(ptr);
			index ++;

			if (index > 11)
			{
				break;
			}
		}
		while (ptr = strtok(NULL, " "));
	}

	*arraySize = index;
}

//--------------------------------------
// dump
//--------------------------------------

void pScale::dump(void)
{
	if (!debug)
	{
		return;
	}

	fprintf(stderr, "Dumping scale:\n");

	fprintf(stderr, "Ascending notes: ");

	int i;
	for (i=0; i<lengthUp; i++)
	{
		fprintf (stderr, "%d ", degreesUp[i]);
	}

	fprintf(stderr, "\n");

	if (lengthDown != 0)
	{
		fprintf(stderr, "Descending notes: ");

		for (i=0; i<lengthDown; i++)
		{
			fprintf (stderr, "%d ", degreesDown[i]);
		}
		fprintf(stderr, "\n");
	}
}

//
// getLength

short pScale::getLength(short dir)
{
	if (dir == UP)
	{
		return lengthUp;
	}
	else
	{
		return lengthDown;
	}
}
