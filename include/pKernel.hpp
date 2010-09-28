//--------------------------------------
// pKernel.hpp
// header definitions for the pKernel
// class.  This class should be
// instantiated only once for a
// system (ie. game, plug-in, etc.)
//--------------------------------------

#ifndef __PKERNEL__
#define __PKERNEL__ 1

//#define __I_AM_PKERNEL__

#include "pSystem.h"

#include "pThread.hpp"
#include "pHarm.hpp"
#include "pNote.hpp"
#include "pEvent.hpp"
#include "pScale.hpp"
#include "pCompiler.hpp"

class pKernel
{
	public:
		pKernel (void);
		~pKernel (void);
		void load(char *);
		void init(short, void (*)(pNote *), void (*)(pEvent *));
		void go(void);
		long go(short);
		char name[80];
		pScale *scales[MAXSCALES];
		short numScales;
		short currentScale;			// The current tonal mode used by the
											// system for calculating chords,
											// scale degrees for melodies, and so-on.

		pHarm *lookahead[2];			// harmony lookahead buffers
		short	laBuffer;				// which lookahead buffer is current
		pThread *threads[MAXTHREADS];
		void gather(void);
		void in(unsigned char, short);

		pNote *currentKey;				// a pointer to a note containing the
											// the current tonal centre

		pEvent *eventList;

		// these are the input "knobs".
		// They are global to all threads so reside
		// on the pKernel object
		// There are 2 arrays because one is for the iRegisters themselves
		// and the other is for scripts to play with. After each think()
		// period, the pKernel updates the iRegisters from the Out array

		short iRegisters[IREGISTERS];
		short iRegistersOut[IREGISTERS];
		short beat;
		short numThreads;

	private:
		pCompiler *compiler;
		pHarm *hStructure;			// this is governing key area info
											// (may be deprecated)
		char key[16];
		short tempo;

		// pointers to external functions


		void (*out)(pNote *);	
		void (*event)(pEvent *);
};

#endif
