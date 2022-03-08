#pragma once

#include "components/simple_scene.h"
#include "Objects2D.h"
#include "Transform2D.h"
#include <string>
#include <Core/Engine.h>
#include <vector>

// Clasa pentru proiectile
class Bullet {
public:
	Bullet();
	Bullet(float coordx, float coordy, float angle);
	float getX();
	float getY();
	float getAngle();
	bool getVis();
	float getTtl();
	void setX(float coordx);
	void setY(float coordy);
	void setVis(bool vis);
	void setTtl(float clock);
	void setBottomLeft(float x, float y);
	void setBottomRight(float x, float y);
	void setTopLeft(float x, float y);
	void setTopRight(float x, float y);
	~Bullet();
private:
	float coordx; 
	float coordy;
	float angle;
	bool visible;
	float ttl; 

	// Colturile proiectilului
	glm::vec2 bottomleft;
	glm::vec2 bottomright;
	glm::vec2 topleft;
	glm::vec2 topright;


};