#include"Enemy.h"

// Clasa pentru inamici
Enemy::Enemy()
{
	this->bodyX = 0;
	this->bodyY = 0;
	this->angle = 0;
	this->visible = true;
}
float Enemy::getX()
{
	return bodyX;
}

float Enemy::getY()
{
	return bodyY;
}

float Enemy::getAngle()
{
	return angle;
}

float Enemy::getSpeed()
{
	return speed;
}

glm::vec2 Enemy::getTopLeft()
{
	return topleft;
}

glm::vec2 Enemy::getTopRight()
{
	return topright;
}

glm::vec2 Enemy::getBottomRight()
{
	return bottomright;
}

glm::vec2 Enemy::getBottomLeft()
{
	return bottomleft;
}

bool Enemy::getVis()
{
	return visible;
}


void Enemy::setEnemyCoord(float x, float y)
{
	bodyX = x;
	bodyY = y;
	rightHandX = bodyX + 1.5;
	rightHandY = bodyY + 1.5;
	leftHandX = bodyX - 1.5;
	leftHandY = bodyY + 1.5;
}

void Enemy::setBottomLeft(float x, float y)
{
	this->bottomleft.x = x;
	this->bottomleft.y = y;
}

void Enemy::setBottomRight(float x, float y)
{
	this->bottomright.x = x;
	this->bottomright.y = y;
}

void Enemy::setTopLeft(float x, float y)
{
	this->topleft.x = x;
	this->topleft.y = y;
}

void Enemy::setTopRight(float x, float y)
{
	this->topright.x = x;
	this->topright.y = y;
}
void Enemy::setCentre(float x, float y)
{
	this->centre.x = x;
	this->centre.y = y;
}
void Enemy::setEnemyAngle(float angle)
{
	this->angle = angle;
}
void Enemy::setSpeed(float speed)
{
	this->speed = speed;
}
void Enemy::setVis(bool vis)
{
	this->visible = vis;
}

Enemy::~Enemy()
{
}