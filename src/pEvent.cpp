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
// pEvent.cc
// pEvent implementation
//-----------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/pEvent.hpp"

extern "C" {
#include "../include/pSystem.h"
#include "../include/pFile.h"
}

// constructor

pEvent::pEvent(short t, short tmp)
{
	type = t;
	tempo = tmp;
	next = NULL;
	prev = NULL;
}

pEvent::pEvent(short t, char *text)
{
	strcpy(comment, text);
	type = t;
	next = NULL;
	prev = NULL;
}

//-----------------------------------
// adds an event to the list
//-----------------------------------

void pEvent::add(pEvent *newEvent)
{
	if (next == NULL)
	{
		next = newEvent;
		newEvent->prev = this;
		newEvent->next = NULL;
	}
	else
	{
		next->add(newEvent);
	}
}

//-----------------------------------
// dump
//-----------------------------------

void pEvent::dump (void)
{
	fprintf(stderr, "Event %d\n", type);

	if (next != NULL)
	{
		next->dump();
	}
}
