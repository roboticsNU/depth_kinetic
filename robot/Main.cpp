#include "camera.h"
#include "serial.h"
#include <iostream> 
#include "EasiiSDK/Iisu.h" 

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h" 
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/videoio.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2\core\core.hpp>
#include<time.h>   
#include <stdio.h>
#include <math.h>
 
#include <Windows.h>

using namespace SK::Easii;  

bool volatile finished = false;
bool volatile onlySerial = false;

DWORD WINAPI cameraStart(CONST LPVOID lpParam) {
	camera();
	ExitThread(0);
}

DWORD WINAPI serialStart(CONST LPVOID lpParam) {
	serial();
	ExitThread(0);
}

int main(int argc, char **argv) { 
	HANDLE camThread;
	HANDLE serialThread;
	HANDLE threads[2];
	camThread = CreateThread(NULL, 0, &cameraStart, NULL, 0, NULL);
	if(NULL == camThread) {
		printf("Failed to create thread: CAMERA.\r\n");
    }
	
	serialThread = CreateThread(NULL, 0, &serialStart, NULL, 0, NULL);
	if(NULL == camThread) {
		printf("Failed to create thread: SERIAL.\r\n");
    }

	threads[0] = camThread;
	threads[1] = serialThread;
	 
	char value = 0;
	scanf("%c", &value);
	finished = true;
	
	WaitForMultipleObjects(2, threads, TRUE, INFINITE);
	for(int i = 0; i < 2; i++) {
		CloseHandle(threads[i]);
	}
}
