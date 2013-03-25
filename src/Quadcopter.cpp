#include "Quadcopter.h"

Quadcopter::Quadcopter(){
	this->motor1 = 1000;
	this->motor2 = 1000;
	this->motor3 = 1000;
	this->motor4 = 1000;
}

// sample: 0,0.00,0.01,1.92,0.75,0,1500,1500,1500,1000,2000,2000,0,0,1000,1000,1000,1000,0,0,0,0,0,1,
void Quadcopter::parseTelemetry(vector<string> data){
	//exploded string should have 25 values
	if (data.size() != 25) {
		return;
	}

	this->motor1 = atoi(data.at(14).c_str());
	this->motor2 = atoi(data.at(15).c_str());
	this->motor3 = atoi(data.at(16).c_str());
	this->motor4 = atoi(data.at(17).c_str());

	/*cout << "Motor 1: " << this->motor1 << endl;
	cout << "Motor 2: " << this->motor2 << endl;
	cout << "Motor 3: " << this->motor3 << endl;
	cout << "Motor 4: " << this->motor4 << endl;*/

}

int Quadcopter::getMotorPower(int motor){
	if (motor == 1) {
		return this->motor1;
	} else if (motor == 2) {
		return this->motor2;
	} else if (motor == 3) {
		return this->motor3;
	} else if (motor == 4) {
		return this->motor4;
	} else {
		return 0;
	}
}
