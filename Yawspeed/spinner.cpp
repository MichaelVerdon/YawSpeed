#include "spinner.h"
#include <Windows.h>
#include <chrono>
#include <thread>


// Mouse injection code from SamuelTulach on GitHub.
enum InjectedInputMouseOptions
{
    Absolute = 32768,
    HWheel = 4096,
    LeftDown = 2,
    LeftUp = 4,
    MiddleDown = 32,
    MiddleUp = 64,
    Move = 1,
    MoveNoCoalesce = 8192,
    None = 0,
    RightDown = 8,
    RightUp = 16,
    VirtualDesk = 16384,
    Wheel = 2048,
    XDown = 128,
    XUp = 256
};

typedef struct _InjectedInputMouseInfo
{
    int DeltaX;
    int DeltaY;
    unsigned int MouseData;
    InjectedInputMouseOptions MouseOptions;
    unsigned int TimeOffsetInMilliseconds;
    void* ExtraInfo;
} InjectedInputMouseInfo;

typedef bool (WINAPI* InjectMouseInput_t)(InjectedInputMouseInfo* inputs, int count);

void Spinner::initSpinner(Profile& profile) {

	yawSpeed = profile.yawSpeed;
	leftKey = profile.leftBind;
	rightKey = profile.rightBind;
    direction = 0;
}

void Spinner::spin(int& dir) {

    // Put in another file
    InjectMouseInput_t InjectMouseInput;

    const HMODULE user32 = LoadLibraryA("user32.dll");

    InjectMouseInput = reinterpret_cast<InjectMouseInput_t>(GetProcAddress(user32, "InjectMouseInput"));
    
	std::thread timerThread([&]() {

		while (spinning) {

            InjectedInputMouseInfo info = { 0 };
            info.DeltaX = 5 * yawSpeed * direction;
            InjectMouseInput(&info, 1);
            Sleep(1);

			}
		});

	timerThread.detach();
	
}

void Spinner::keyListener() {

    std::thread keyThread([&]() {
        while (spinnerOn) {
            if (GetAsyncKeyState('X') & 0x8000) {
                // Perform left turn logic
                if (direction == 1 && spinning) {

                    direction = 1;
                    spinning = false;
                }
                else {
      
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
                }
                else {

                    direction = -1;
                    spinning = true;
                    spin(direction);
                }
            }

            Sleep(1);
        }
        });

    keyThread.detach();
}
