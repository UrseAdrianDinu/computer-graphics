#pragma once
#include "components/simple_scene.h"
#include "Objects3D.h"
#include "Transform3D.h"
#include <string>
#include <Core/Engine.h>
#include <vector>

class Foe {
public:
	Foe();
	~Foe();
	float getX();
	float getY();
	float getSpeed();
	float getTranslateX();
	float getTranslateY();
	bool getVis();
	bool getIsHit();
	float getAngle();
	int getType();
	float getTtl();
	int getCellx();
	int getCelly();
	bool getFollowMode();
	void setIsHit(bool ishit);
	void setCoord(float x, float y);
	void setAngle(float angle);
	void setSpeed(float speed);
	void setVis(bool vis);
	void setType(int type);
	void setTranlateX(float translateX);
	void setTranlateY(float translateYS);
	void setTtl(float ttl);
	void setCellx(int cellx);
	void setCelly(int celly);
	void setFollowMode(bool followmode);

private:
	float x;
	float y;
	int cellx;
	int celly;
	int type;
	float angle;
	float speed;
	bool visible;
	bool ishit;
	float translateX;
	float translateY;
	float ttl;
	bool followmode = false;
};