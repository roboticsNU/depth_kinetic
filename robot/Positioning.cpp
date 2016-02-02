#include "Positioning.h"  

Positioning::~Positioning(void) {
	fclose(file);
	fclose(fileEuler);
}
Positioning::Positioning(void)
{

	file = fopen ("data/MPU/mpu.txt","w");
	fileEuler = fopen ("data/MPU/mpu_ypr.txt","w");

	fprintf (file, "accelx, accely, accelz, yaw, pitch, roll, qw, qx, qy, qz, delta\n");
	posX = 0;
	posY = 0;
	posZ = 0;
	vX = 0;
	vY = 0;
	vZ = 0;
}

void Positioning::estimateGravity() {
    gx = 2 * (quatX * quatZ - quatW * quatY);
    gy = 2 * (quatW * quatX + quatY * quatZ);
    gz = quatW*quatW - quatX*quatX - quatY*quatY + quatZ*quatZ;
}

void Positioning::operateData(double gyro0, double gyro1, double gyro2, double yaw, double pitch, double roll, double qw, double qx, double qy, double qz, double delta) {
	 // todo
	this->gyro0 = gyro0;
	this->gyro1 = gyro1;
	this->gyro2 = gyro2;
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
	quatW = qw;
	quatX = qx;
	quatY = qy;
	quatZ = qz;
	quatDelta = delta;

	estimateGravity();
	
	/*Quaternion quat(quatW, quatX, quatY, quatZ);
	quat.normalize();
	Vectordouble v(gyro0, gyro1, gyro2);
	v.rotate(&quat);
	gyro0 = v.x;
	gyro1 = v.y;
	gyro2 = v.z;
	*/
	double g = 9.8;
	this->vX += delta * g * gyro0 / 16384.0;
	this->vY += delta * g * gyro1 / 16384.0;
	this->vZ += delta * g * gyro2 / 16384.0;
	
	this->posX += this->vX * delta;
	this->posY += this->vY * delta;
	this->posZ += this->vZ * delta;
	
	fprintf (file, "%e,%e,%e,%e,%e,%e,%e,%e,%e,%e,%e\n", 
																  gyro0, gyro1, gyro2, 
																  yaw, pitch, roll, 
																  qw, qx, qy, qz, delta);  
	fprintf (fileEuler, "%e,%e,%e\n",  
																  yaw, pitch, roll);  
}

void Positioning::operateData(double yaw, double pitch, double roll, double qw, double qx, double qy, double qz, double delta) {
	 // todo
	fprintf (file, "%e,%e,%e,%e,%e,%e,%e,%e\n", yaw, pitch, roll, qw, qx, qy, qz, delta);  
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
	quatW = qw;
	quatX = qx;
	quatY = qy;
	quatZ = qz;
	quatDelta = delta;
}


std::string Positioning::getTextData() {
	std::string str = "";
	/*str += gyro0 + " " + gyro1 + " " + gyro2 + " " + yaw + " " + pitch + " " + roll + " " + quatW + " " + quatX + " " + quatY + " " + quatZ + " " + quatDelta;*/
	//str += (gyro0/16384.0) + "\t" + gyro1/16384.0 + "\t" + gyro2/16384.0;
	return str;
}