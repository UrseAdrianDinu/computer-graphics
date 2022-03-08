#include "Player.h"


Player::Player()
{
	bodyRadius = 1.0f;
	handRadius = 0.5f;
	bodyCoord.x = 0;
	bodyCoord.y = 0;
}

float Player::getBodyRadius()
{
	return bodyRadius;
}

glm::vec3 Player::getBodyCoord()
{
	return bodyCoord;
}


glm::vec3 Player::getRightHandCoord()
{
	return righthandCoord;
}

glm::vec3 Player::getLeftHandCoord()
{
	return righthandCoord;
}

Mesh* Player::getBodyIn()
{
	return BodyIn;
}

Mesh* Player::getBodyOut()
{
	return BodyOut;
}

Mesh* Player::getLeftHandIn()
{
	return leftHandIn;
}

Mesh* Player::getLeftHandOut()
{
	return leftHandOut;
}

Mesh* Player::getRightHandIn()
{
	return rightHandIn;
}

Mesh* Player::getRightHandOut()
{
	return rightHandOut;
}

void Player::setPlayerCoord(float x, float y)
{
	bodyCoord.x = x;
	bodyCoord.y = y;
	righthandCoord.x = bodyCoord.x + cos(RADIANS(45)) * bodyRadius;
	righthandCoord.y = bodyCoord.y + sin(RADIANS(45)) * bodyRadius;
	lefthandCoord.x = bodyCoord.x + cos(RADIANS(135)) * bodyRadius;
	lefthandCoord.y = bodyCoord.y + sin(RADIANS(135)) * bodyRadius;
}

void Player::createPlayer()
{
	BodyIn = Objects2D::CreateCircle("BodyIn", glm::vec3(bodyCoord.x, bodyCoord.y, 0), bodyRadius - 0.1f, glm::vec3(1, 1, 0.2), true);
	BodyOut = Objects2D::CreateCircle("BodyOut", glm::vec3(bodyCoord.x, bodyCoord.y, 0), bodyRadius, glm::vec3(0, 0, 0), true);
	rightHandIn = Objects2D::CreateCircle("RightHandIn", glm::vec3(righthandCoord.x, righthandCoord.y, 0), handRadius - 0.1f, glm::vec3(1, 1, 0.2), true);
	rightHandOut = Objects2D::CreateCircle("RightHandOut", glm::vec3(righthandCoord.x, righthandCoord.y, 0), handRadius, glm::vec3(0, 0, 0), true);
	leftHandIn = Objects2D::CreateCircle("LeftHandIn", glm::vec3(lefthandCoord.x, lefthandCoord.y, 0), handRadius - 0.1f, glm::vec3(1, 1, 0.2), true);
	leftHandOut = Objects2D::CreateCircle("LeftHandOut", glm::vec3(lefthandCoord.x, lefthandCoord.y, 0), handRadius, glm::vec3(0, 0, 0), true);
}

Player:: ~Player()
{
	delete BodyIn;
	delete BodyOut;
	delete rightHandIn; 
	delete rightHandOut;
	delete leftHandIn;
	delete leftHandOut;
}