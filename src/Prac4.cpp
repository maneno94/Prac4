
/*
 * Prac4.cpp
 *
 * Originall written by Stefan Schröder and Dillion Heald
 *
 * Adapted for EEE3096S 2019 by Keegan Crankshaw
 *
 * This file is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "Prac4.h"

using namespace std;

bool playing = true; // should be set false when paused
bool stopped = false; // If set to true, program should close
unsigned char buffer[2][BUFFER_SIZE][2];
int buffer_location = 0;
bool bufferReading = 0; //using this to switch between column 0 and 1 - the first column
bool threadReady = false; //using this to finish writing the first column at the start of the song, before the column is played


// Configure your interrupts here.
// Don't forget to use debouncing.
//int ki=wiringPiISR(PLAY_BUTTON,INT_EDGE_FALLING,&play_audio);
//int kj=wiringPiISR(STOP_BUTTON,INT_EDGE_FALLING,&stop_audio);//int ki=wiringPiISR(PLAY_BUTTON,INT_EDGE_FALLING,&play_audio);
//int kj=wiringPiISR(STOP_BUTTON,INT_EDGE_FALLING,&stop_audio);
long lastInterruptTime =0 ;

void play(void)
{
     long interrupt_time = millis();
     if ( interrupt_time - lastInterruptTime > 60 )
     { playing = !playing;}
     lastInterruptTime=interrupt_time;

}

void stop(void)
{
     long interrupt_time = millis();
     if ( interrupt_time - lastInterruptTime > 60 )
     {
        //cout<<"stopped"<<endl;
       stopped = true;
       exit(0);
     }
    lastInterruptTime=interrupt_time;

}
int setup_gpio(void){
    //Set up wiring Pi
     wiringPiSetup();
    //setting up the buttons
    pinMode(PLAY_BUTTON, INPUT);
Connection reset by fe80::ba27:ebff:fe22:f1fb%5 port 22

C:\Users\student>ol(PLAY_BUTTON, PUD_UP);
    pullUpDnControl(STOP_BUTTON, PUD_UP);
    wiringPiISR(PLAY_BUTTON,INT_EDGE_FALLING,play);
    wiringPiISR(STOP_BUTTON,INT_EDGE_FALLING,stop);

    //setting up the SPI interface
    wiringPiSPISetup(SPI_CHAN, SPI_SPEED);
    return 0;
}

/*
 * Thread that handles writing to SPI
 *
 * You must pause writing to SPI if not playing is true (the player is paused)
 * When calling the function to write to SPI, take note of the last argument.
 * You don't need to use the returned value from the wiring pi SPI function
 * You need to use the buffer_location variable to check when you need to switch buffers
 */
