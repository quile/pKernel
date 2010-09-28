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
