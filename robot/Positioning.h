 
#include <stdio.h>
#include <string>

class Positioning
{
private:
	FILE *file;
	double gx;
	double gy;
	double gz;
	double gyro0;
	double gyro1;
	double gyro2;
	double posX;
	double posY;
	double posZ;
	double vX;
	double vY;
	double vZ;
	double quatW, quatX, quatY, quatZ, quatDelta;
	double yaw, pitch, roll;
public:
	Positioning(void);
	~Positioning(void);
	std::string getTextData();
	void operateData(double yaw, double pitch, double roll, double qw, double qx, double qy, double qz, double delta);
	void operateData(double gyro0, double gyro1, double gyro2, double yaw, double pitch, double roll, double qw, double qx, double qy, double qz, double delta);
	void estimateGravity();
};

