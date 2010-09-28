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

//------------------------------------
// pThread.h
// Class definition file for the
// main play thread class.
//------------------------------------

#ifndef __PTHREAD__
#define __PTHREAD__  1

#include "pNote.hpp"
#include "pHarm.hpp"
#include "pScript.hpp"
#include "pCompiler.hpp"

#include <stdio.h>

class pThread
{
	public:
		pThread (FILE *, pCompiler *, void *, short);
		pThread (char *, short);
		~pThread (void);
		void think(short, short); // the main "think" function that is
								  // called when the pThread needs to act
		void think(pHarm **, short, short, short);
								  // the main "think" function that is
								  // called to generate harmonic info
		void activate (void);	  // activates the thread
		void deactivate (void);	  // deactivates the thread
		void reset(void);		  // resets the thread
		void setKernel(void *);	  // sets the thread's parent pKernel
		void out(pNote *);		  // sends a note to the out buffer
		pHarm *harmList;		  // pointer to harmony list for this pThread
		pScript *scriptList;      // scripts
		pCompiler *compiler;      // a pointer to the compiler on the pKernel
		char name[MAXCHARS];	  // the name of the pThread
		pNote *noteList;		  // input material
		pNote *outList;		      // this thread's output buffer
		short threadId;
	private:
		pHarm *harmOffset;		  // pointer into the harmony list

		pNote *timeOffset;	      // a pointer into the pThread that is increased
								  // during every "think" operation and wraps
								  // around when it reaches the end

		float totalBeats;		  // total beats that have been thunk so far in
								  // this thinking period, plus any carry-over
								  // from the last (necessary to keep note
								  // alignment consistent)

	    short active;			  // whether this pThread is active (ie. allowed
								  // to think)

		void *pK;				  // pointer to the parent pKernel
};

#endif
