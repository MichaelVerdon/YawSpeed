#pragma once
#include "Profile.h"
#include <Windows.h>
#include <chrono>
#include <thread>

class Spinner
{
public:
	bool spinning = false;
	bool spinnerOn = false;
	int yawSpeed;
	int direction; // Can take 1 or -1 for + and - directions.
	std::string leftKey;
	std::string rightKey;

	void initSpinner(Profile &profile);
	void spin(int& dir);
	void centerCursor();
	void keyListener();

};

