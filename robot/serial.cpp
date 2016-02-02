#include "serial.h"
#include <stdio.h>
#include "Positioning.h"
#include <string>
#include <iostream>
#include <vector>
#include <windows.h>

extern bool volatile finished;
extern bool volatile onlySerial;

bool volatile canstart = false;

//#define OUTPUT_TO_STDOUT

std::vector<std::string> split(const std::string &text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

void serial() {
	Positioning *positioning = new Positioning();

	if (!onlySerial) {
		printf("SERIAL: waiting for camera start\n");
		while (!canstart) {}
	}
	printf("SERIAL: Starting serial\n");

	DCB config_;        

	config_.ByteSize = 8;            // Byte of the Data.
	config_.StopBits = ONESTOPBIT;   // Use one bit for stopbit.
	config_.Parity = NOPARITY;       // No parity bit
	config_.BaudRate = CBR_38400;     // Buadrate 38400 bit/sec
	config_.fBinary = TRUE;

	HANDLE handlePort_ = CreateFile(L"COM3",  // Specify port device: default "COM1"
									GENERIC_READ,       // Specify mode that open device.
									0,                                  // the devide isn't shared.
									NULL,                               // the object gets a default security.
									OPEN_EXISTING,                      // Specify which action to take on file. 
									0,                                  // default.
									NULL);         
	//GetCommState(handlePort_,&config_);
	SetCommState(handlePort_,&config_);

	// instance an object of COMMTIMEOUTS.
    COMMTIMEOUTS comTimeOut;        
	GetCommTimeouts(handlePort_,&comTimeOut);
    // Specify time-out between charactor for receiving.
    comTimeOut.ReadIntervalTimeout = 3;
    // Specify value that is multiplied 
    // by the requested number of bytes to be read. 
    comTimeOut.ReadTotalTimeoutMultiplier = 3;
    // Specify value is added to the product of the 
    // ReadTotalTimeoutMultiplier member
    comTimeOut.ReadTotalTimeoutConstant = 2;
    // Specify value that is multiplied 
    // by the requested number of bytes to be sent. 
    comTimeOut.WriteTotalTimeoutMultiplier = 3;
    // Specify value is added to the product of the 
    // WriteTotalTimeoutMultiplier member
    comTimeOut.WriteTotalTimeoutConstant = 2;
    // set the time-out parameter into device control.
    SetCommTimeouts(handlePort_,&comTimeOut);
	
	char inputData[256];
	char mpuData[256];
	int counter = 0;
	unsigned long length = 0;
	while (true) {
		if (ReadFile(handlePort_,  // handle of file to read
			inputData,               // handle of file to read
			1,              // number of bytes to read
			&length,                 // pointer to number of bytes read
			NULL) == 0) {              // pointer to structure for data
			continue;
		}
		if (length == 1) {
			if (inputData[0] == '\n') {
				break;
			}
		}
	}

	int errnum = 0;
	while(!finished) {
		for (int i = 0; i < 256; ++i) {
			mpuData[i] = 0;
		}
		while (true) {
			if (ReadFile(handlePort_,  // handle of file to read
				inputData,               // handle of file to read
				1,              // number of bytes to read
				&length,                 // pointer to number of bytes read
				NULL) == 0) {              // pointer to structure for data
				continue;
			}
			
			//continue;
			if (length == 1) {
				if (inputData[0] == '\n') {
					if (counter < 10) {
						counter = 0;
						continue;
					}
					break;
				}
				mpuData[counter++] = inputData[0];
			}
			 
		}
		mpuData[counter] = 0;
		std::vector<std::string> splitted = split(std::string(mpuData), ',');
		if (splitted.size() == 11) {
			int bla = 0; 
			double gyro0 = stod(splitted.at(bla++));
			double gyro1 = stod(splitted.at(bla++));
			double gyro2 = stod(splitted.at(bla++));
			double yaw = stod(splitted.at(bla++));
			double pitch = stod(splitted.at(bla++));
			double roll  = stod(splitted.at(bla++));
			double qw = stod(splitted.at(bla++));
			double qx = stod(splitted.at(bla++));
			double qy = stod(splitted.at(bla++));
			double qz = stod(splitted.at(bla++)); 
			unsigned long delta = stol(splitted.at(bla++));
			double deltaF = (double)delta / 1000.0f; // to seconds
			positioning->operateData(gyro0, gyro1, gyro2, yaw, pitch, roll, qw, qx, qy, qz, delta);
			
#ifdef OUTPUT_TO_STDOUT
			for (int i = 0; i < splitted.size(); ++i) {
				std::cout << splitted.at(i) << "\t";
			} 
			std::cout << "\n";
#endif
		} else {
			errnum++;
		}
		counter = 0;
	}
	return;
}