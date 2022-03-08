#pragma once

#include "components/simple_scene.h"
#include "Objects3D.h"
#include "Transform3D.h"
#include <string>
#include <Core/Engine.h>
#include <vector>

class Human
{
public:
	Human();
	~Human();
	glm::vec3 getCoord();

	float getAngle();

	glm::vec3 getCentre();

	glm::vec3 getFront();

	glm::vec3 getBack();

	glm::vec3 getRight();

	glm::vec3 getLeft();

	float getCameraPos();

	void setCoord(float x, float y, float z);

	void setAngle(float angle);

	void setCentre(float x, float y, float z);

	void setFront(float x, float y, float z);

	void setBack(float x, float y, float z);

	void setRight(float x, float y, float z);

	void setLeft(float x, float y, float z);

	void setCameraPos(float pos);

private:
	glm::vec3 coord;
	glm::vec3 centre;
	glm::vec3 front;
	glm::vec3 back;
	glm::vec3 left;
	glm::vec4 right;
	float health;
	float angle;
	float cameraYpos;
};