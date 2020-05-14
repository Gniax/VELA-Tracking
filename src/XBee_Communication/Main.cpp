#include <iostream>
#include "grovepi.h"

#define XBEE1 3
#define XBEE2 4

using namespace std;
using namespace GrovePi;
bool start = true;

void setup();
void loop();

int main() {
	try {
		initGrovePi();
		while(start) loop();
	}
	catch(I2CError &error)
	{
		cout << "Erreur lors de l'initialisation de GrovePi+" << endl;
		return -1;
	}
	return 0;
}

void loop() {
	start = false;
}
