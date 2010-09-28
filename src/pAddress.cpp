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
// pAddress : used by pCompiler only
// on the first pass to store locations
// of labels that are to be resolved on
// the second pass
//--------------------------------------

#include "../include/pAddress.hpp"

pAddress::pAddress (char *lab, long off)
{
	strcpy (label, lab);
	offset = off;
	nextAddress = NULL;
}

pAddress::~pAddress (void)
{
	if (nextAddress != NULL)
		delete nextAddress;
}

void pAddress::add(pAddress *p)
{
	if (nextAddress == NULL)
	{
		p->nextAddress = NULL;
		nextAddress = p;
	}
	else
	{
		nextAddress->add(p);
	}
}

void pAddress::dump(short recursive)
{
	if (recursive && nextAddress != NULL)
	{
		nextAddress->dump(1);
	}
}

pAddress *pAddress::find(char *l)
{
#ifdef WIN32
	if (!strcmpi(label,l))
#else
	if (!strcasecmp(label, l))
#endif
	{
		return this;
	}
	else
	{
		if (nextAddress == NULL)
		{
			return NULL;
		}
		else
		{
			return nextAddress->find(l);
		}
	}
}
