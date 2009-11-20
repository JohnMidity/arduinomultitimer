/* Simple example of the MultiTimer2 Library */
/* Usage:
 *  To add a function to be run every "m" milliseconds, "count" number of times call
 *       MultiTimer2::AddTask(function, m, count) 
 *  MultiTimer2::IsRunning will be 0 if the multitimer is not running
 *  If it is not running, you'll need to start it by using:
 *       MultiTimer2::start();
 *
 *  Send your likes and dislikes to John.Zandbergen@gmail.com
 * 
 *  You can add tasks while other tasks are running.
 *
 *  Add as many tasks as long as memory lasts.
 *
 *  DON'T use delay in tasks. The interrupt system of the atMega does not like 
 *  that, me neither, use a different task instead.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Make sure you import Javier Valencia's MsTimer2 library first!
 * You can download and install it from here: http://www.arduino.cc/playground/Main/MsTimer2 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
#include <MsTimer2.h>

/* now include the MultiTimer2 library */
#include <MultiTimer2.h>

void LedOn() {
  digitalWrite(13, HIGH);
}
void LedOff() {
  digitalWrite(13, LOW);
}

void setup() {
  pinMode(13, OUTPUT);

  /* Put the led on 21 times, every 1000 msecs */
  MultiTimer2::AddTask(LedOn, 1000, 21);
  /* Put the led on 21 times, every 1050 msecs */
  MultiTimer2::AddTask(LedOff, 1050, 21);
  MultiTimer2::start();
}

void loop() {
  /* Keep checking if the multitimer is ready. If it is ready start a fast blinker */
  if (!MultiTimer2::IsRunning)
  {
    /* Put the led on/of endless times */
    MultiTimer2::AddTask(LedOn, 100, 0);
    MultiTimer2::AddTask(LedOff, 101, 0);
    MultiTimer2::start();
  }
}
