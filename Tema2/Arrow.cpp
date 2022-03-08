#include "Arrow.h"

Arrow::Arrow()
{
	pos = glm::vec3(0);
	angle = 0;
}
Arrow::Arrow(glm::vec3 pos, float angle)
{
	this->pos = pos;
	this->angle = angle;
}
glm::vec3 Arrow::getPos()
{
	return pos;
}

float Arrow::getAngle()
{
	return angle;
}
bool Arrow::getVis()
{
	return visible;
}
float Arrow::getTtl()
{
	return ttl;
}
glm::vec3 Arrow::getDir()
{
	return dir;
}

void Arrow::setPos(glm::vec3 pos)
{
	this->pos = pos;
}

void Arrow::setVis(bool vis)
{
	this->visible = vis;
}
void Arrow::setTtl(float clock)
{
	this->ttl = clock;
}

void Arrow::setBottomLeft(float x, float y)
{
	this->bottomleft.x = x;
	this->bottomleft.y = y;
}

void Arrow::setBottomRight(float x, float y)
{
	this->bottomright.x = x;
	this->bottomright.y = y;
}

void Arrow::setTopLeft(float x, float y)
{
	this->topleft.x = x;
	this->topleft.y = y;
}

void Arrow::setTopRight(float x, float y)
{
	this->topright.x = x;
	this->topright.y = y;
}

void Arrow::setDir(glm::vec3 dir)
{
	this->dir = dir;
}

Arrow::~Arrow()
{
}