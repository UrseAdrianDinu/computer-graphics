#pragma once
#include "components/simple_scene.h"
#include <Core/Engine.h>

class Dancer {
public:
	Dancer();
	Dancer(float x, float z);
	float getX();
	float getZ();
	float getTranslateX();
	float getTranslateY();
	float getTranslateZ();
	float getAngle();
	bool getJump();
	bool getGoUp();
	void setTranslateX(float translatex);
	void setTranslateY(float translatey);
	void setTranslateZ(float translatez);
	void setAngle(float angle);
	void setJump(bool jump);
	void setGoUp(bool goup);
	~Dancer();

private:
	float x, z;
	float translateX, translateY, translateZ;
	float angle;
	bool jump;
	bool go_up;
};