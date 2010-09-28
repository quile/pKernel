//--------------------------------
// pNote.hpp
// header file for the pNote class
// that defines the most basic
// unit in the system, the note
//--------------------------------

#ifndef __PNOTE__
#define __PNOTE__

#include <stdio.h>
#include "../include/pScale.hpp"
#include "../include/pSystem.h"

#define MAXBAGGAGE 10

class pNote
{
	public:
		pNote();
		pNote (char *, short, short, short);
		pNote (float, short, short, short);
		pNote (FILE *, short);
		~pNote (void);
		pNote *nextNote;
		pNote *prevNote;
		void clone(pNote *);
		void add(pNote *);
		void dump(void);
		void out(void);
		float getDurationF(void);
		char *getDurationC(void);
		void setDuration(char *);
		short degreePitchClass(void *, short, short);
		short degreePitchClass(pScale *, short);	// used to generate
																// a PC if the note is
																// specified as a DEG.
		short octave;
		short thread;
		short pitchClass;
		short degree;				// degree and alteration are an alternate
		short alteration[MAXSCALES];			
										// way of specifying pitch information
										// - each note can have a different alteration
										// for each scale.

		long	onset;				// not used inside the pKernel, but is provided
										// so that functions that are going to render
										// or otherwise process the note can
										// calculate it and store it here
		long offset;				// also not used inside the pKernel
		char duration[12];		// it should be a string so that dotted
										// durations and non-standard ones can be
										// dealt with
		float fdur;

		// this is important: a string that is passed around
		// that can be used to transfer extra information that can
		// be used by the scripts to communicate with the output mechanisms

		char baggage[10];
};

#endif
