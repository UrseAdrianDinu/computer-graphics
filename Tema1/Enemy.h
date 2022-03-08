#pragma once
#include "components/simple_scene.h"
#include "Objects2D.h"
#include "Transform2D.h"
#include <string>
#include <Core/Engine.h>
#include <vector>

class Enemy {
public:
	Enemy();
	float getX();
	float getY();
	float getSpeed();
	glm::vec2 getTopLeft();
	glm::vec2 getTopRight();
	glm::vec2 getBottomRight();
	glm::vec2 getBottomLeft();
	bool getVis();
	void setEnemyCoord(float x, float y);
	void setEnemyAngle(float angle);
	void setBottomLeft(float x, float y);
	void setBottomRight(float x, float y);
	void setTopLeft(float x, float y);
	void setTopRight(float x, float y);
	void setCentre(float x, float y);
	void setSpeed(float speed);
	void setVis(bool vis);
	float Enemy::getAngle();
	~Enemy();

private:
	float bodyX;
	float bodyY;
	float rightHandX;
	float rightHandY;
	float leftHandX;
	float leftHandY;
	float angle;
	float speed;
	bool visible;

	// Colturile inamicului
	glm::vec2 bottomleft;
	glm::vec2 bottomright;
	glm::vec2 topleft;
	glm::vec2 topright;
	glm::vec2 centre;
};