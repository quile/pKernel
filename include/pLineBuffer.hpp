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
// pLineBuffer : used by pCompiler only
// on the first pass to store lines
//--------------------------------------

#ifndef __PLINEBUFFER__
#define __PLINEBUFFER__

#include <stdio.h>
#include <string.h>

extern "C" {
#include "../include/pSystem.h"
}

class pLineBuffer
{
	public:
		pLineBuffer (char *l);
		~pLineBuffer (void);
		void add (pLineBuffer *p);
		void dump (short recursive);
		char line[80];

		pLineBuffer *nextLineBuffer;
};

#endif
