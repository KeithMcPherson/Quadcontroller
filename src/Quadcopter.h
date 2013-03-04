#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

class Quadcopter
{
	private:
		int motor1;
		int motor2;
		int motor3;
		int motor4;


	public:
		Quadcopter();
		void parseTelemetry(vector<string> data);
};
