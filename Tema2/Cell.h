#pragma once

#include "components/simple_scene.h"
#include "Objects3D.h"
#include "Transform3D.h"
#include <string>
#include <Core/Engine.h>
#include <vector>


class Cell
{
public:
	Cell();

	Cell(int xx, int yy);

	int getX();

	glm::vec3 getColor();

	int getY();

	int getNorth();

	int getSouth();

	int getEast();

	int getWest();

	int getVisited();

	int getEnemy();



	~Cell();
	void setVisited(int vis);

	void setNorth(int nor);

	void setSouth(int sou);

	void setEast(int eas);

	void setWest(int wes);

	void setEnemy(int en);


private:
	int x, y;
	int north, south, east, west;
	int visited;
	int enemy;
};