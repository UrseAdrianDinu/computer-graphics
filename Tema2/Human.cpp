#include "Human.h"

Human::Human()
{
	coord = glm::vec3(0);
	centre = glm::vec3(0);
	health = 100;
	angle = 0;
	cameraYpos = 2;
}

Human::~Human()
{
}

glm::vec3 Human::getCoord()
{
	return coord;
}
float Human::getAngle()
{
	return angle;
}

glm::vec3 Human::getCentre()
{
	return centre;
}

glm::vec3 Human::getFront()
{
	return front;
}

glm::vec3 Human::getBack()
{
	return back;
}

glm::vec3 Human::getRight()
{
	return right;
}

glm::vec3 Human::getLeft()
{
	return left;
}


void Human::setCoord(float x, float y, float z)
{
	coord.x = x;
	coord.y = y;
	coord.z = z;
}

float Human::getCameraPos()
{
	return cameraYpos;
}

void Human::setAngle(float angle)
{
	this->angle = angle;
}

void Human::setCentre(float x, float y, float z)
{
	centre.x = x;
	centre.y = y;
	centre.z = z;
}

void Human::setFront(float x, float y, float z)
{
	front.x = x;
	front.y = y;
	front.z = z;
}

void Human::setBack(float x, float y, float z)
{
	back.x = x;
	back.y = y;
	back.z = z;
}

void Human::setRight(float x, float y, float z)
{
	right.x = x;
	right.y = y;
	right.z = z;
}

void Human::setLeft(float x, float y, float z)
{
	left.x = x;
	left.y = y;
	left.z = z;
}

void Human::setCameraPos(float pos)
{
	cameraYpos = pos;
}

