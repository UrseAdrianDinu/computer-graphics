#include "Dancer.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/Engine.h>
using namespace std;

Dancer::Dancer()
{
	x = z = 0;
	translateX = translateY = translateZ = 0;
	angle = 0;
}
Dancer::Dancer(float x, float z)
{
	this->x = x;
	this->z = z;
	translateX = translateY = translateZ = 0;
	jump = false;
	go_up = true;
	angle = 0;
}

float Dancer::getX()
{
	return x;
}

float Dancer::getZ()
{
	return z;
}

float Dancer::getTranslateX()
{
	return translateX;
}

float Dancer::getTranslateY()
{
	return translateY;
}

float Dancer::getTranslateZ()
{
	return translateZ;
}
float Dancer::getAngle()
{
	return angle;
}
bool Dancer::getJump()
{
	return jump;
}
bool Dancer::getGoUp()
{
	return go_up;
}
void Dancer::setTranslateX(float translatex)
{
	translateX = translatex;
}

void Dancer::setTranslateY(float translatey)
{
	translateY = translatey;
}

void Dancer::setTranslateZ(float translatez)
{
	translateZ = translatez;
}

void Dancer::setAngle(float angle)
{
	this->angle = angle;
}

void Dancer::setJump(bool jump)
{
	this->jump = jump;
}

void Dancer::setGoUp(bool goup)
{
	go_up = goup;
}

Dancer::~Dancer()
{
	
}
