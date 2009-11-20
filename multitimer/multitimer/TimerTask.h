/*
  Filename: TimerTask.h
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
#ifndef TimerTask_h
#define TimerTask_h

class TimerTask
{
public:
	void (*FunctionToCall)(void);
	unsigned long msec;
	unsigned long NrOfTimes;
	volatile TimerTask *PrevTask;
	unsigned long WaitMsec;
	volatile TimerTask *NextTask;

	TimerTask();
};

#endif