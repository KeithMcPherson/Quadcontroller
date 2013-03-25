//============================================================================
// Name        : Quadcontrol.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "SDL/SDL.h"
#include "serialib.h"
#include "Graphics.h"

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#if defined (_WIN32) || defined( _WIN64)
	#define DEVICE_PORT "COM1"
#endif

#ifdef __linux__
	#define DEVICE_PORT "/dev/ttyACM0"
#endif

serialib Serial;
SDL_Joystick *stick = NULL;
Quadcopter *quadcopter;

//prototype functions
string readData();
void sendData(string output);
int joyToPWM(int value);
vector<string> explode( const string &delimiter, const string &explodeme);
void sendJoyCommands();

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
Graphics* gfx;
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
	//const int THROTTLE_AXIS = 3;
	//const int YAW_AXIS = 2;

int main() {

	//Start SDL
	SDL_Init( SDL_INIT_EVERYTHING );
	gfx = new Graphics (SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ));


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


	quadcopter = new Quadcopter();

	cout << "Waiting 10s for reboot...\n";
	usleep(10000000); //sleep so it has time to reboot
	cout << "Done!\n";

	while (true) {
		sendJoyCommands();
		sendData("s;");
		//sendData("71500;1500;1500;1300;");

		usleep(50000);
		quadcopter->parseTelemetry(explode(",", readData()));
		gfx->renderMotors(quadcopter);

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
	int result=Serial.WriteString((output).c_str());                                             // Send the command on the serial port
	if (result!=1) {                                                           // If the writting operation failed ...
		printf ("Error while writing data\n");                              // ... display a message ...
	}
	Serial.FlushReceiver();
}

string readData(){
	char Buffer[256];

	//if (Serial.Peek() > 60) {
		// Read a string from the serial device
		int result=Serial.ReadString(Buffer,'\n',256);
		if (result>89) {
			//cout << "read amount: " << result << endl;
			//cout << "read: " << Buffer;// If a string has been read from, print the string
			return Buffer;
		} else {
			//printf ("TimeOut reached. No data received !\n");               // If not, print a message.
			return "";
		}


		return Buffer;

	//} else {
		//return "";
	//}
}

int joyToPWM(int value){
	return (value + 32768)/65.536+1000;
}

vector<string> explode( const string &delimiter, const string &str)
{
    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0;
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}

void sendJoyCommands(){
	SDL_JoystickUpdate();
	stringstream joyCommands;
	int roll = joyToPWM(SDL_JoystickGetAxis(stick, ROLL_AXIS));
	int pitch = joyToPWM(SDL_JoystickGetAxis(stick, PITCH_AXIS));
	int yaw = joyToPWM(SDL_JoystickGetAxis(stick, YAW_AXIS));
	int throttle = joyToPWM(-SDL_JoystickGetAxis(stick, THROTTLE_AXIS));
	joyCommands << 7;
	joyCommands << roll << ";";
	joyCommands << pitch << ";";
	joyCommands << yaw << ";";
	joyCommands << throttle << ";";
	//cout << joyCommands.str() << "\n";
	sendData(joyCommands.str());

}


