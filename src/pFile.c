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

/*-------------------------------
// pFile.c
// basic filing routines
//-------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/pSystem.h"
#include "../include/pFile.h"

/*-------------------------------
// pfReadNext
// reads in the next non-comment
// and non-blank line
//-------------------------------
*/

char *pfReadNext(FILE *in, char *inbuf, int maxchars)
{
	int done = FALSE;
	char *retVal;
	char *eol;

	while (!done)
	{
		retVal = fgets(inbuf, maxchars, in);
/* 		printf("=>%s", inbuf); */

		if (!retVal)
		{
			done = TRUE;
		}

/* 		// strip newlines */

		if ( (eol=(char *)strchr(inbuf, '\n')) != NULL)
		{
			*eol = '\0';
		}

		if (strlen(inbuf) > 0 && strncmp(inbuf, "#", 1))
		{
			done = TRUE;
		}
	}

	return retVal;
}
