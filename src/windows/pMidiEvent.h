// This is the include file for
// the pMidiEvent class

#include <afxwin.h>
#include <mmsystem.h>

#include "..\include\pNote.hpp"

#define PME_START 1
#define PME_FINISH 2

class pMidiEvent
{
	public:
		MIDIEVENT midiEvent;
		pMidiEvent(void);
		pMidiEvent(pNote *, float, short);
		~pMidiEvent(void);
		pMidiEvent *next;
		pMidiEvent *prev;
		float absoluteTime;

		void add(pMidiEvent *);
		void insert(pMidiEvent *);
};
