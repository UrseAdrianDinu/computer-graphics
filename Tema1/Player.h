#pragma once

#include "components/simple_scene.h"
#include "Objects2D.h"
#include "Transform2D.h"
#include <string>
#include <Core/Engine.h>
#include <vector>

class Player
{
public:	
	Player();
	~Player();
	glm::vec3 getBodyCoord();
	glm::vec3 Player::getRightHandCoord();
	glm::vec3 getLeftHandCoord();
	void Player::setPlayerCoord(float x, float y);
	void createPlayer();
	Mesh* Player::getBodyIn();
	Mesh* Player::getBodyOut();
	Mesh* Player::getLeftHandIn();
	Mesh* Player::getLeftHandOut();
	Mesh* Player::getRightHandIn();
	Mesh* Player::getRightHandOut();
	float Player::getBodyRadius();

private:
	glm::vec3 bodyCoord, righthandCoord, lefthandCoord;
	Mesh* BodyIn;
	Mesh* BodyOut; 
	Mesh* leftHandIn;
	Mesh* leftHandOut;
	Mesh* rightHandIn;
	Mesh* rightHandOut;
	float health;
	float bodyRadius, handRadius;

};