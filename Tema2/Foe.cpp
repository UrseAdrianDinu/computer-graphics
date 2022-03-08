#include"Foe.h"

// Clasa pentru inamici
Foe::Foe()
{
	x = y = 0;
	angle = 0;
	visible = true;
	ishit = false;
	ttl = 0;
	translateX = translateY = 0;
}

Foe::~Foe()
{
}

float Foe::getX()
{
	return x;
}

float Foe::getY()
{
	return y;
}

float Foe::getAngle()
{
	return angle;
}

int Foe::getType()
{
	return type;
}

float Foe::getTtl()
{
	return ttl;
}

int Foe::getCellx()
{
	return cellx;
}

int Foe::getCelly()
{
	return celly;
}

bool Foe::getFollowMode()
{
	return followmode;
}

float Foe::getSpeed()
{
	return speed;
}

float Foe::getTranslateX()
{
	return translateX;
}

float Foe::getTranslateY()
{
	return translateY;
}

bool Foe::getVis()
{
	return visible;
}

bool Foe::getIsHit()
{
	return ishit;
}

void Foe::setIsHit(bool ishit)
{
	this->ishit = ishit;
}

void Foe::setCoord(float xx, float yy)
{
	x = xx;
	y = yy;
}

void Foe::setAngle(float angle)
{
	this->angle = angle;
}
void Foe::setSpeed(float speed)
{
	this->speed = speed;
}
void Foe::setVis(bool vis)
{
	this->visible = vis;
}

void Foe::setType(int type)
{
	this->type = type;
}

void Foe::setTranlateX(float translateX)
{
	this->translateX = translateX;
}

void Foe::setTranlateY(float translateY)
{
	this->translateY = translateY;
}

void Foe::setTtl(float ttl)
{
	this->ttl = ttl;
}

void Foe::setCellx(int cellx)
{
	this->cellx = cellx;
}

void Foe::setCelly(int celly)
{
	this->celly = celly;
}

void Foe::setFollowMode(bool followmode)
{
	this->followmode = followmode;
}
