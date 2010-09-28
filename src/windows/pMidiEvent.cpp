// this is the code for the pMidiEvent class

#include "pMidiEvent.h"

pMidiEvent::pMidiEvent(void)
{
	absoluteTime = 0.0;

	next = NULL;
	prev = NULL;
}

pMidiEvent::pMidiEvent(pNote *note, float absTime, short type)
{
	unsigned char command, velocity, channel, noteNum;

	absoluteTime = absTime;

	// figure out midi info

	if (type == PME_START)
	{
		// it's a note on
		command = 0x90;
	}
	else
	{
		command = 0x80;
	}

	// get the channel

	command = command | note->thread;

	// get the note number
	// get the velocity

	if (note->pitchClass == REST)
	{
		velocity = 0;
		noteNum = 0;
	}
	else
	{
		noteNum = note->octave * 12 + note->pitchClass;
		velocity = (unsigned char) note->baggage[0];
	}

	midiEvent.dwEvent = command | (noteNum*0x100) | (velocity*0x10000) | MEVT_F_SHORT;

	next = NULL;
	prev = NULL;
}

pMidiEvent::~pMidiEvent(void)
{
	if (next != NULL)
	{
		delete next;
	}
}

void pMidiEvent::add (pMidiEvent *newEvent)
{
}

void pMidiEvent::insert(pMidiEvent *newEvent)
{
	if (newEvent->absoluteTime >= absoluteTime)
	{
		// event follows this one

		if (next != NULL)
		{
			// test if it's between this one and the next one

			if (next->absoluteTime >= newEvent->absoluteTime)
			{
				// add it here

				next->prev = newEvent;
				newEvent->next = next;
				next = newEvent;
				newEvent->prev = this;
			}
			else
			{
				// check the next one

				next->insert(newEvent);
			}
		}
		else
		{
			// nothing left afterwards so add it there

			next = newEvent;
			newEvent->prev = this;
			newEvent->next = NULL;
		}
	}
	else
	{
		// event precedes this one so there's a problem.
	}
}
