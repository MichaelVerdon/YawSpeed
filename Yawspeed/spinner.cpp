#include "spinner.h"
#include <Windows.h>
#include <chrono>
#include <thread>

void Spinner::initSpinner(Profile& profile) {

	yawSpeed = profile.yawSpeed;
	leftKey = profile.leftBind;
	rightKey = profile.rightBind;
    direction = 0;
}

void Spinner::spin(int& dir) {

	//SetCursorPos(cursorPos.x, cursorPos.y);

	std::thread timerThread([&]() {

        POINT cursorPos;
        GetCursorPos(&cursorPos);

		while (spinning) {

            int x = cursorPos.x;

            if (dir == 1) {
                x += (yawSpeed * direction);
            }
            else if (dir == -1) {
                x -= (yawSpeed * direction);
            }
			
			GetCursorPos(&cursorPos);
			SetCursorPos(x, cursorPos.y);

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		});

	timerThread.detach();
	
}

void Spinner::centerCursor() {
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int centerX = screenWidth / 2;
	int centerY = screenHeight / 2;

	SetCursorPos(centerX, centerY);
}

void Spinner::keyListener() {

    std::thread keyThread([&]() {
        while (spinnerOn) {
            if (GetAsyncKeyState('X') & 0x8000) {
                // Perform left turn logic
                if (direction == 1 && spinning) {
                    direction = 1;
                    spinning = false;
                    centerCursor();
                }
                else {
                    centerCursor();
                    direction = 1;
                    spinning = true;
                    spin(direction);
                }
            }

            if (GetAsyncKeyState('Z') & 0x8000) {
                // Perform right turn logic
                if (direction == -1 && spinning) {
                    direction = -1;
                    spinning = false;
                    centerCursor();      
                }
                else {
                    centerCursor();
                    direction = -1;
                    spinning = true;
                    spin(direction);
                }
            }

            // Add a small delay to avoid high CPU usage
            Sleep(5);
        }
        });

    keyThread.detach();
}