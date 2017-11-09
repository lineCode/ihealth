#include "robot_test.h"

#include <iostream>
#include <windows.h>

#include "control_card.h"
#include "data_acquisition.h"

using namespace std;

int robot_test_start() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	
	while (true) {
		DataAcquisition::GetInstacne().AcquisiteTorqueData();
		std::cout << DataAcquisition::GetInstacne().ElbowTorque() << " " <<DataAcquisition::GetInstacne().ShoulderTorque() << std::endl;
		Sleep(200);
	}
	system("pause");
	return 0;
}