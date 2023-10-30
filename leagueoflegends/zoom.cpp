#include "zoom.h"
#include "Awareness.h"
#include <Windows.h>
#include "stdafx.h"

//global vars
float zoomValue = 1.281169772;
float leftNright = 180;
float upNdown = 56;
float angle = 0;
float CurrentAngle = 0;
const float zoomValueReset = 1.281169772;
const float leftNrightReset = 180;
const float upNdownReset = 56;
const float rotationSpeed = 5.5;
const float Msensitivity = 1.5;
const float zoomSpeed = 0.05;

DWORD pID = GetCurrentProcessId();
HANDLE processHandle = GetCurrentProcess();

//get screen res
int x = GetSystemMetrics(SM_CXSCREEN);
int y = GetSystemMetrics(SM_CYSCREEN);


POINT p; //for cursor pos




void resetZoom()
{
    WriteProcessMemory(processHandle, (LPVOID)(zoomAddress), &zoomValueReset, sizeof(float), 0);
    WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 264), &leftNrightReset, sizeof(float), 0);
    WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 260), &leftNrightReset, sizeof(float), 0);
    leftNright = leftNrightReset;
    WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 268), &upNdownReset, sizeof(float), 0);
    upNdown = upNdownReset;
}

using namespace UPasta::SDK::Awareness;
void startZoom()
{
	//Crashes here
	while (!globals::eject) //main loop
	{
		Sleep(10);
		

		if (shouldInject)//probably crashes here 
		{
			float valueToWrite = Configs::Zoom::zoomValue->Value / 10.0f;
			WriteProcessMemory(processHandle, (LPVOID)(zoomAddress), &valueToWrite, sizeof(float), 0);
		}
		else
		{
			resetZoom();
		}

		if (isLeagueFocused)
		{
			if (shouldWrite)
			{
				if (angleCamera != 90.0f)
					WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 260), &angleCamera, sizeof(float), 0);

				float zero = 0.0f;
				WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 264), &zero, sizeof(float), 0);
			}

			if (useKeyboard)//keyboard
			{
				if (GetAsyncKeyState(VK_NUMPAD0)) //reset
					resetZoom();

				if (GetAsyncKeyState(VK_ADD)) //numpad +
				{
					if (zoomValue > 0.78)
					{
						zoomValue -= zoomSpeed;
						WriteProcessMemory(processHandle, (LPVOID)(zoomAddress), &zoomValue, sizeof(float), 0);
					}
				}

				if (GetAsyncKeyState(VK_SUBTRACT)) // numpad -
				{
					if (zoomValue < 2.7)
					{
						zoomValue += zoomSpeed;
						WriteProcessMemory(processHandle, (LPVOID)(zoomAddress), &zoomValue, sizeof(float), 0);
					}
				}

				if (GetAsyncKeyState(VK_LEFT)) // LEFT ARROW
				{
					leftNright += rotationSpeed;
					WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 264), &leftNright, sizeof(float), 0);
				}

				if (GetAsyncKeyState(VK_RIGHT)) // RIGHT ARROW
				{
					leftNright -= rotationSpeed;
					WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 264), &leftNright, sizeof(float), 0);
				}

				if (GetAsyncKeyState(VK_UP)) // UP ARROW
				{
					if (upNdown < 89)
					{
						upNdown += rotationSpeed;
						WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 268), &upNdown, sizeof(float), 0);
					}
				}

				if (GetAsyncKeyState(VK_DOWN)) // DOWN ARROW
				{
					if (upNdown > 17.5)
					{
						upNdown -= rotationSpeed;
						WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 268), &upNdown, sizeof(float), 0);
					}
				}

				if (useMouseWheel)
				{
					if (GetAsyncKeyState(VK_MBUTTON)) //lock mouse
					{
						if (GetCursorPos(&p))
						{
							if (p.x < x / 2)
							{
								leftNright += Msensitivity;
								WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 264), &leftNright, sizeof(float), 0);
							}

							if (p.x > x / 2)
							{
								leftNright -= Msensitivity;
								WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 264), &leftNright, sizeof(float), 0);
							}

							if (p.y < y / 2)
							{
								if (upNdown < 89)
								{
									upNdown += Msensitivity;
									WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 268), &upNdown, sizeof(float), 0);
								}
							}

							if (p.y > y / 2)
							{
								if (upNdown > 17.5)
								{
									upNdown -= Msensitivity;
									WriteProcessMemory(processHandle, (LPVOID)(zoomAddress - 268), &upNdown, sizeof(float), 0);
								}
							}
						}
					}
				}

			}
		}
	}
	if (globals::eject == true)
		resetZoom();
	


}


