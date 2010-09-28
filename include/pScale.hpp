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


//------------------------------------------
// pScale.hpp
// header file for the pScale class
//------------------------------------------
// Defines an abstract scale class that
// essentially governs the validity of
// certain notes in a given context.
//------------------------------------------

#ifndef __PSCALE__
#define __PSCALE__

#define UP 0
#define DOWN 1

#include <stdio.h>

#include "../include/pSystem.h"

class pScale
{
	public:
		pScale(FILE *);
		pScale(char *, short);
		void setScale(char *, short);
		short degree(short, short);
		~pScale();
		void dump(void);
		short getLength(short);

	private:
		void tokenise(char *, short *, short *);
		short degreesUp[12];
		short degreesDown[12];
		short lengthUp;
		short lengthDown;
};

#endif
