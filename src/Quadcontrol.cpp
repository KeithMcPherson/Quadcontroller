//============================================================================
// Name        : Quadcontrol.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "SDL/SDL.h"
#include "serialib.h"
#include <iostream>
using namespace std;
#if defined (_WIN32) || defined( _WIN64)
#define         DEVICE_PORT             "COM1"                               // COM1 for windows
#endif

#ifdef __linux__
#define         DEVICE_PORT             "/dev/ttyACM0"                         // ttyS0 for linux
#endif

serialib Serial;
//The joystick that will be used
SDL_Joystick *stick = NULL;

//prototype functions
string readData();
void sendData(string output);

int main() {
	//Start SDL
	SDL_Init( SDL_INIT_EVERYTHING );

	if (Serial.Open(DEVICE_PORT,115200)!=1) {                                                           // If an error occured...
		printf ("Error while opening port. Permission problem ?\n");        // ... display a message ...
		return -1;                                                         // ... quit the application
	}
	printf ("Serial port opened successfully !\n\n");

	//Check if there's any joysticks
	if( SDL_NumJoysticks() < 1 ) {
		cout << "No Joystick Found";
		return false;
	} //Open the joystick
	stick = SDL_JoystickOpen( 0 );
	//If there's a problem opening the joystick
	if( stick == NULL ) {
		cout << "Joystick couldn't be opened";
		return false;
	}
	cout << "Joystick Info:\n";
	cout << "Axes: " << SDL_JoystickNumAxes(stick) << "\n";
	cout << "Trackballs: " << SDL_JoystickNumBalls(stick) << "\n";
	cout << "Coolihats: " << SDL_JoystickNumHats(stick) << "\n";
	cout << "Buttons: " << SDL_JoystickNumButtons(stick) << "\n\n";
	//axis 0 roll
	//axis 1 pitch
	//axis 2 throttle
	//axis 3 lower knob (on throttle)
	//axis 4 upper knob (on throttle)
	//axis 5 yaw
	//axis 6 volume control
	//axis 7 mouse up/down
	//axis 8 mouse left/right

	while (true) {
		cout << "axis value: " << SDL_JoystickGetAxis(stick, 8) << "\n";
		SDL_JoystickUpdate();
		//sendData("s");
		//cout << "read: " << readData() << "\n";
		//sendData("71500;1500;1500;1500;");
		usleep(1000000);
	}

	Serial.Close();
	//Close the joystick
	SDL_JoystickClose( stick );
	//Quit SDL
	SDL_Quit();
	return 0;
}

// Write the AT command on the serial port
void sendData(string output){
	int result=Serial.WriteString((output+"\n").c_str());                                             // Send the command on the serial port
	if (result!=1) {                                                           // If the writting operation failed ...
		printf ("Error while writing data\n");                              // ... display a message ...
	}
}

string readData(){
	char Buffer[256];

	if (Serial.Peek() > 0) {
		// Read a string from the serial device
		int result=Serial.ReadString(Buffer,'\n',128,500);	// Read a maximum of 128 characters with a timeout of 5 seconds
															// The final character of the string must be a line feed ('\n')
		/*if (result>0)                                     // If a string has been read from, print the string
			printf ("String read from serial port : %s",Buffer);
		else
			printf ("TimeOut reached. No data received !\n");               // If not, print a message.
		*/return Buffer;

	} else {
		return "";
	}
}

