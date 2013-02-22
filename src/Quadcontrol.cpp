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
#include <sstream>

using namespace std;

#if defined (_WIN32) || defined( _WIN64)
#define         DEVICE_PORT             "COM1"                               // COM1 for windows
#endif

#ifdef __linux__
#define         DEVICE_PORT             "/dev/ttyACM0"                         // ttyS0 for linux
#endif

serialib Serial;
SDL_Joystick *stick = NULL;

//prototype functions
string readData();
void sendData(string output);
int joyToPWM(int value);

int main() {
	/*cout << "Setting serial port to low latency\n";
	stringstream setserial;
	setserial << "setserial " << DEVICE_PORT << " low_latency";
	system(setserial.str().c_str());*/
	//Start SDL
	SDL_Init( SDL_INIT_EVERYTHING );

	if (Serial.Open(DEVICE_PORT,115200)!=1) { // If an error occured...
		printf ("Error while opening port. Permission problem ?\n");
		return -1;
	}
	printf ("Serial port opened successfully !\n\n");

	//Check if there's any joysticks
	if( SDL_NumJoysticks() < 1 ) {
		cout << "No Joystick Found\n";
		return false;
	} //Open the joystick
	stick = SDL_JoystickOpen( 0 );
	//If there's a problem opening the joystick
	if( stick == NULL ) {
		cout << "Joystick couldn't be opened\n";
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
	const int ROLL_AXIS = 0;
	const int PITCH_AXIS = 1;
	const int YAW_AXIS = 5;
	const int THROTTLE_AXIS = 2;

	cout << "Waiting 10s for reboot...\n";
	usleep(10000000); //sleep so it has time to reboot
	cout << "Done!\n";

	while (true) {
		//sendData("s;");
		SDL_JoystickUpdate();

		stringstream joyCommands;

		joyCommands << 7;
		joyCommands << joyToPWM(SDL_JoystickGetAxis(stick, ROLL_AXIS)) << ";";
		joyCommands << joyToPWM(SDL_JoystickGetAxis(stick, PITCH_AXIS)) << ";";
		joyCommands << joyToPWM(SDL_JoystickGetAxis(stick, YAW_AXIS)) << ";";
		joyCommands << joyToPWM(-SDL_JoystickGetAxis(stick, THROTTLE_AXIS)) << ";";
		//cout << joyCommands.str() << "\n";
		sendData(joyCommands.str());

		usleep(50000);
		readData();

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
	Serial.FlushReceiver();
}

string readData(){
	char Buffer[256];

	//if (Serial.Peek() > 0) {
		// Read a string from the serial device
		int result=Serial.ReadString(Buffer,'\n',256);
		if (result>0) {
			cout << "read: " << Buffer;// If a string has been read from, print the string
			return Buffer;
		} else {
			//printf ("TimeOut reached. No data received !\n");               // If not, print a message.
			return "";
		}


		return Buffer;

	/*} else {
		return "";
	}*/
}

int joyToPWM(int value){
	return (value + 32768)/65.536+1000;
}


