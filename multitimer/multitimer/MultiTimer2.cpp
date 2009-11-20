/*
  Filename: MultiTimer2.cpp
  Copyright (c) 2009 John Zandbergen.  All right reserved.

  Send your likes and dislikes to:
  John Zandbergen <john.zandbergen@gmail.com>
  
  Versions:
	20091030 - V1.0

  PLEASE NOTE:
    This file is part of the MultiTimer2 library which needs the MsTimer2 
	library, excellent work of Javier Valencia <javiervalencia80@gmail.com>.
	It is downloadable at: http://www.arduino.cc/playground/Main/MsTimer2

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <stdlib.h>
#include "MultiTimer2.h"
#include "TimerTask.h"

#define NULL 0

/* The TaskList will hold all the interrupt vectors */
volatile TimerTask *MultiTimer2::TaskList = NULL;

/* The IsRunning will be set when there are Tasks in the queue */
volatile int MultiTimer2::IsRunning = 0;

/* This is the one and only interrupt routine, it will in turn call the usefull interrupt vector */
void MultiTimer2::Interrupt()
{
	if (MultiTimer2::TaskList == NULL)
	{
		/* No Tasks, Why interrupt????? */
		/* Stop interrupting */
		MsTimer2::stop();
		return;
	}

	/* take the next Task from the list */
	volatile TimerTask *aTask = MultiTimer2::TaskList;

	/* make the Task list ready for the next Task */
	MultiTimer2::TaskList = aTask->NextTask;

	// Execute the Task
	aTask->FunctionToCall();

	switch(aTask->NrOfTimes){
		case 1:
			/* This Task is running the last time, don't add it to the Tasklist again */
			free((void *)aTask);
			break;
		case 0:
			/* This Task is running forever, add it to the Task list again */
			MultiTimer2::AddTask(aTask);
			break;
		default:
			/* This Task is running some more times, add it to the Task list again, 
			 * but decrease the number of time it should run
			 */
			aTask->NrOfTimes--;
			MultiTimer2::AddTask(aTask);
	}

	if (MultiTimer2::TaskList == NULL)
	{
		/* There are no more Tasks. */
		/* Stop the timer */
		MsTimer2::stop();
		/* Set a 'public' variable so everyone can see the Task is ready */
		MultiTimer2::IsRunning = 0;

		return;
	}

	if (MultiTimer2::TaskList->WaitMsec == 0)
		/* If there are more Tasks waiting to run on this same instant, run them */
		MultiTimer2::Interrupt();
	else
	{
		/* Set the new timer to the delay for the next Task */
		MsTimer2::stop();
		/* Reset the timer for the next Task */
		MsTimer2::set(MultiTimer2::TaskList->WaitMsec, Interrupt);
		/* And start the timer */
		MsTimer2::start();
	}
}

void MultiTimer2::AddTask(void (*FunctionToCall)(void), unsigned long msec, unsigned long NrOfTimes)
{
	volatile TimerTask *theTask = (volatile TimerTask *)(malloc(sizeof(volatile TimerTask)));
	theTask->FunctionToCall = FunctionToCall;
	theTask->msec = msec;
	theTask->NrOfTimes = NrOfTimes;

	MultiTimer2::AddTask(theTask);
}

void MultiTimer2::AddTask(volatile TimerTask *theTask)
{
	unsigned long passed = 0;

	if (MultiTimer2::TaskList==NULL)
	{
		theTask->WaitMsec=theTask->msec;
		theTask->PrevTask = NULL;
		theTask->NextTask = NULL;
		MultiTimer2::TaskList = theTask;
		return;
	}

	volatile TimerTask *aTask = MultiTimer2::TaskList;

	// Insert the Task in front if it is to be run sooner than the first item
	if (theTask->msec < aTask->WaitMsec)
	{
		theTask->WaitMsec=theTask->msec;
		theTask->NextTask = MultiTimer2::TaskList;
		theTask->NextTask->PrevTask = theTask;
		theTask->NextTask->WaitMsec = theTask->NextTask->WaitMsec - theTask->msec;
		theTask->PrevTask = NULL;
		MultiTimer2::TaskList = theTask;
	}else{
		bool done = false;
		while (!done)
		{
			if (passed + aTask->WaitMsec > theTask->msec)
			{
				// insert the Task before aTask
				theTask->WaitMsec = theTask->msec - passed;
				aTask->WaitMsec -= theTask->WaitMsec;
				theTask->NextTask = aTask;
				theTask->PrevTask = aTask->PrevTask;
				aTask->PrevTask->NextTask = theTask;
				aTask->PrevTask = theTask;
				done = true;
			}else {
				if (aTask->NextTask == NULL)
				{
					// We have reached the last item
					theTask->WaitMsec = theTask->msec - (passed + aTask->WaitMsec);
					theTask->PrevTask = aTask;
					theTask->NextTask = NULL;
					aTask->NextTask = theTask;
					done = true;
				}else{
					passed += aTask->WaitMsec;
					aTask = aTask->NextTask;
				}
			}
		}
	}
}
void MultiTimer2::start(){
	/* Only start if there are tasks and we are not running yet */
	if ((MultiTimer2::TaskList != NULL) && (MultiTimer2::IsRunning == 0))
	{
		if (MultiTimer2::TaskList->WaitMsec > 0)
		{
			MsTimer2::set(MultiTimer2::TaskList->WaitMsec, Interrupt);
			MsTimer2::start();
			MultiTimer2::IsRunning = 1;
		}
	}
}