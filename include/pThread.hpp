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
		void think(short, short);		// the main "think" function that is
												// called when the pThread needs to act
		void think(pHarm **, short, short, short);		
												// the main "think" function that is
												// called to generate harmonic info
		void activate (void);			// activates the thread
		void deactivate (void);			// deactivates the thread
		void reset(void);					// resets the thread
		void setKernel(void *);			// sets the thread's parent pKernel
		void out(pNote *);				// sends a note to the out buffer
		pHarm *harmList;		// pointer to harmony list for this pThread
		pScript *scriptList;	// scripts
		pCompiler *compiler; // a pointer to the compiler on the pKernel
		char name[MAXCHARS];			// the name of the pThread
		pNote *noteList;		// input material
		pNote *outList;		// this thread's output buffer
		short threadId;
	private:
		pHarm *harmOffset;		// pointer into the harmony list

		pNote *timeOffset;	// a pointer into the pThread that is increased
									// during every "think" operation and wraps
									// around when it reaches the end

		float totalBeats;		// total beats that have been thunk so far in
									// this thinking period, plus any carry-over
									// from the last (necessary to keep note
									// alignment consistent)
		
		short active;			// whether this pThread is active (ie. allowed
									// to think)

		void *pK;				// pointer to the parent pKernel
};

#endif
