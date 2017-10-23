#include "robot_test.hpp"

#include <iostream>
#include <windows.h>

using namespace std;

int robot_test_start() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	
	ControlCard cc;
	cc.Initial();
	cc.ResetPosition();
	system("pause");
	return 0;
}