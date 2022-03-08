#include "Bullet.h"

Bullet::Bullet(float coordx,float coordy,float angle)
{
	this->coordx = coordx;
	this->coordy = coordy;
	this->angle = angle;
}

float Bullet::getX()
{
	return coordx;
}
float Bullet::getY()
{
	return coordy;
}
float Bullet::getAngle()
{
	return angle;
}
bool Bullet::getVis()
{
	return visible;
}
float Bullet::getTtl()
{
	return ttl;
}
void Bullet::setX(float coordx)
{
	this->coordx = coordx;
}
void Bullet::setY(float coordy)
{
	this->coordy = coordy;
}
void Bullet::setVis(bool vis)
{
	this->visible = vis;
}
void Bullet::setTtl(float clock)
{
	this->ttl = clock;
}

void Bullet::setBottomLeft(float x, float y)
{
	this->bottomleft.x = x;
	this->bottomleft.y = y;
}

void Bullet::setBottomRight(float x, float y)
{
	this->bottomright.x = x;
	this->bottomright.y = y;
}

void Bullet::setTopLeft(float x, float y)
{
	this->topleft.x = x;
	this->topleft.y = y;
}

void Bullet::setTopRight(float x, float y)
{
	this->topright.x = x;
	this->topright.y = y;
}

Bullet::~Bullet()
{
}
