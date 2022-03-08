#pragma once
#include "components/simple_scene.h"
#include "Objects3D.h"
#include "Transform3D.h"
#include <string>
#include <Core/Engine.h>
#include <vector>

class Arrow {
public:
	Arrow();
	Arrow(glm::vec3 pos, float angle);
	glm::vec3 Arrow::getPos();
	float getAngle();
	bool getVis();
	float getTtl();
	glm::vec3 getDir();
	void setPos(glm::vec3 pos);
	void setVis(bool vis);
	void setTtl(float clock);
	void setBottomLeft(float x, float y);
	void setBottomRight(float x, float y);
	void setTopLeft(float x, float y);
	void setTopRight(float x, float y);
	void setDir(glm::vec3 dir);
	~Arrow();
private:
	glm::vec3 pos;
	float angle;
	bool visible;
	float ttl;

	glm::vec3 dir;

	// Colturile proiectilului
	glm::vec2 bottomleft;
	glm::vec2 bottomright;
	glm::vec2 topleft;
	glm::vec2 topright;


};