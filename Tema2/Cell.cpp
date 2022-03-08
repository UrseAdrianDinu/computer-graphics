
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "Objects3D.h"
#include "Transform3D.h"
#include "Cell.h"


Cell::Cell()
{
	x = y = 0;
	north = south = west = east = 1;
	visited = 0;
	enemy = 0;
}

Cell::Cell(int xx, int yy)
{
	x = xx;
	y = yy;
	north = south = west = east = 1;
	visited = 0;
	enemy = 0;
}

int Cell::getX()
{
	return x;
}


int Cell::getY()
{
	return y;
}

int Cell::getNorth()
{
	return north;
}

int Cell::getSouth()
{
	return south;
}

int Cell::getEast()
{
	return east;
}

int Cell::getWest()
{
	return west;
}

int Cell::getVisited()
{
	return visited;
}

int Cell::getEnemy()
{
	return enemy;
}

void Cell::setVisited(int vis)
{
	visited = vis;
}

void Cell::setNorth(int nor)
{
	north = nor;
}

void Cell::setSouth(int sou)
{
	south = sou;
}

void Cell::setEast(int eas)
{
	east = eas;
}

void Cell::setWest(int wes)
{
	west = wes;
}

void Cell::setEnemy(int en)
{
	enemy = en;
}

Cell::~Cell()
{
}
