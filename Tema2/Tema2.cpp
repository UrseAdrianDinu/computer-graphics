#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Objects3D.h"
#include "Transform3D.h"
#include "Transf2D.h"


using namespace std;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{

	LoadTextures();
	LoadShaders();
	window->DisablePointer();


	noEnemies = 20;
	// Loading meshes
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	const string sourceObjectsDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "objects");

	{
		Mesh* mesh = new Mesh("zombie");
		mesh->LoadMesh(sourceObjectsDir, "zombie.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("creeper");
		mesh->LoadMesh(sourceObjectsDir, "creeper.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}


	{
		Mesh* mesh = new Mesh("portal");
		mesh->LoadMesh(sourceObjectsDir, "NetherPortal.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("arrow");
		mesh->LoadMesh(sourceObjectsDir, "ArrowAll.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("TNT");
		mesh->LoadMesh(sourceObjectsDir, "Minecraft_TNT.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("pig");
		mesh->LoadMesh(sourceObjectsDir, "pig.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}



	Mesh* cube = Objects3D::CreateCube("cube", 1, glm::vec3(1, 1, 1), meshes);

	Mesh* para = Objects3D::CreatePara("para", 4, 1, 2, glm::vec3(0.7, 0.5, 1), meshes);

	Mesh* square = Objects3D::CreateSquare("square", glm::vec3(0), 5, meshes);

	Mesh* greenrec = Objects3D::CreateRectangle("greenrec", glm::vec3(0), 2, 0.2, glm::vec3(0, 1, 0), true);
	AddMeshToList(greenrec);

	Mesh* greenout = Objects3D::CreateRectangle("greenout", glm::vec3(0), 2.04, 0.22, glm::vec3(0, 1, 0), false);
	AddMeshToList(greenout);

	Mesh* whiterec = Objects3D::CreateRectangle("whiterec", glm::vec3(0), 2, 0.2, glm::vec3(1, 1, 1), true);
	AddMeshToList(whiterec);

	Mesh* whiteout = Objects3D::CreateRectangle("whiteout", glm::vec3(0), 2.04, 0.22, glm::vec3(1, 1, 1), false);
	AddMeshToList(whiteout);


	{
		lightPosition = glm::vec3(0, 1, 1);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
	}

	// Player initialization
	human = new Human();
	srand(time(NULL));
	startpos.x = rand() % 10;
	startpos.y = rand() % 10;
	human->setCoord(5 * startpos.y + 2.5, 0, 5 * startpos.x + 2.5);
	human->setCentre(5 * startpos.y + 2.5 + 0.55, 2, 5 * startpos.x + 2.5 + 0.25);
	
	// Maze initialization
	InitMaze();
	endpos.x = -1;
	endpos.y = 0;

	// Maze Generation
	GenerateMaze();

	// TNT initialization
	float tntx = rand() % 10;
	float tnty = rand() % 10;
	while ((tntx == startpos.x && tnty == startpos.y) || (tntx == endpos.x && tnty == endpos.y))
	{
		tntx = rand() % 10;
		tnty = rand() % 10;
	}
	tntpos.x = 5 * tnty + 2.5;
	tntpos.y = 0.9;
	tntpos.z = 5 * tntx + 2.5;

	float pigx = rand() % 10;
	float pigy = rand() % 10;

	// Pig initialization
	while ((pigx == startpos.x && pigy == startpos.y) || (pigx == endpos.x && pigy == endpos.y)
		|| (pigx == tntx && pigy == tnty))
	{
		pigx = rand() % 10;
		pigy = rand() % 10;
	}

	pigpos.x = 5 * pigy + 2.5;
	pigpos.y = 0.65;
	pigpos.z = 5 * pigx + 2.5;

	// Enemies initialization
	for (int i = 0; i < noEnemies; i++)
	{
		Foe* f = new Foe();
		int type = rand() % 2;
		int foex = rand() % 10;
		int foey = rand() % 10;

		while ((foex == endpos.x && foey == endpos.y) || (foex == startpos.x && foey == startpos.y) || maze[foey + 10 * foex]->getEnemy() == 1 ||
			(foex==tntpos.x && foey ==tntpos.y)|| (abs(foex-startpos.x)<=1 && abs(foey - startpos.y)<=1))
		{
			foex = rand() % 10;
			foey = rand() % 10;
		}
		maze[foey + 10 * foex]->setEnemy(1);
		f->setCoord(5 * foey + 1, 5 * foex + 1);
		f->setCellx(foex);
		f->setCelly(foey);
		f->setType(type);
		f->setSpeed(6);
		foes.push_back(f);
	}

	// Third-person camera initialization
	glm::mat4 thirdPersCameraPosition = glm::mat4(1);
	thirdPersCameraPosition = Transform3D::Translate(5 * startpos.y + 2.5 + 0.55 + -5, 2, 5 * startpos.x + 2.5 + 0.25);
	camera = new Camera();
	camera->Set(thirdPersCameraPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(5 * startpos.y + 2.5 + 0.55, 2, 5 * startpos.x + 2.5 + 0.25), glm::vec3(0, 1, 0));
	fov = 50.0f;

	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
	playertime = clock();

}


void Tema2::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{

	// Escape time update
	if ((float(clock() - playertime) / CLOCKS_PER_SEC) > 3.0f)
	{
		timescale--;
		playertime = clock();
		if (timescale == 0)
		{
			cout << "GAME OVER\n";
			exit(0);
		}
	}

	// Walls down time update
	if ((float(clock() - tnt_enabled) / CLOCKS_PER_SEC) > 4.0f)
	{
		walls_down = false;
	}

	// Setting player front, back, left, right points
	glm::vec3 coord = human->getCoord();
	float angle = human->getAngle();
	float posx = coord.x;
	float posy = coord.y;
	float posz = coord.z;
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(posx + 0.55, posy, posz);
	modelMatrix *= Transform3D::Translate(0, 0, 0.25);
	modelMatrix *= Transform3D::RotateOY(angle + M_PI / 2);
	modelMatrix *= Transform3D::Translate(0, 0, -0.25);
	human->setFront(modelMatrix[3][0], 0, modelMatrix[3][2]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(posx + 0.55, posy, posz + 0.5);
	modelMatrix *= Transform3D::Translate(0, 0, -0.25);
	modelMatrix *= Transform3D::RotateOY(angle + M_PI / 2);
	modelMatrix *= Transform3D::Translate(0, 0, 0.25);
	human->setBack(modelMatrix[3][0], 0, modelMatrix[3][2]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(posx , posy, posz + 0.25);
	modelMatrix *= Transform3D::Translate(0.55, 0, 0);
	modelMatrix *= Transform3D::RotateOY(angle + M_PI / 2);
	modelMatrix *= Transform3D::Translate(-0.55, 0, 0);
	human->setLeft(modelMatrix[3][0], 0, modelMatrix[3][2]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(posx + 1.1f, posy, posz + 0.25);
	modelMatrix *= Transform3D::Translate(-0.55, 0, 0);
	modelMatrix *= Transform3D::RotateOY(angle + M_PI / 2);
	modelMatrix *= Transform3D::Translate(+0.55, 0, 0);
	human->setRight(modelMatrix[3][0], 0, modelMatrix[3][2]);

	// Player - end portal collision
	glm::vec3 playerpos = human->getCentre();
	float disx =  playerpos.x - (5 * endpos.y + 2.5);
	float disz = playerpos.z - (5 * endpos.x + 2.5);
	if (sqrt(disx * disx + disz * disz) < 1)
	{
		cout << "YOU WON";
		exit(0);
	}

	// Player - TNT collision
	disx = playerpos.x - tntpos.x;
	disz = playerpos.z - tntpos.z;

	if (sqrt(disx * disx + disz * disz) < 1)
	{
		// Maze walls are down for 4 seconds
		walls_down = true;
		tnt_enabled = clock();
	}

	// PLAYER - Pig collision
	disx = playerpos.x - pigpos.x;
	disz = playerpos.z - pigpos.z;
	if (sqrt(disx * disx + disz * disz) < 1)
	{
		pigvis = false;
		// Restore full health
		playerhealth = 5;
	}

	// Not rendering the player in first person
	if (thirdperson) {
		RenderPlayer();
	}

	RenderEnvironment();

	if (!walls_down) {
		RenderMaze();
	}
	
	// Clearing hit enemies
	auto it = foes.begin();
	while (it != foes.end())
	{
		if ((*it)->getVis() == false)
		{
			it = foes.erase(it);
		}
		else
		{
			it++;
		}
	}

	// Enemies update
	for (int i = 0; i < foes.size(); i++)
	{

		float translateX = foes[i]->getTranslateX();
		float translateY = foes[i]->getTranslateY();
		int speed = foes[i]->getSpeed();
		
		float posxfoe = foes[i]->getX();
		float posyfoe = foes[i]->getY();

		// Player - enemy collision
		if (foes[i]->getIsHit() != true) {
			glm::vec3 playerpos = human->getCentre();
			float distx = posxfoe + translateX + -playerpos.x;
			float distz = posyfoe + translateY - playerpos.z;
			if (sqrt(distx * distx + distz * distz) < 1)
			{
				foes[i]->setVis(false);
				playerhealth--;
				if (playerhealth == 0)
				{
					cout << "GAME OVER\n";
					exit(0);
				}
				continue;
			}
		}

		Cell* playercell = getPlayerCell();
		if (foes[i]->getIsHit() != true) {
			
			// Follow mode
			if (playercell->getX() == foes[i]->getCellx() && playercell->getY() == foes[i]->getCelly())
			{
				foes[i]->setFollowMode(true);
				foes[i]->setTranlateY(translateY + cos(foes[i]->getAngle()) * speed / 2 * deltaTimeSeconds);
				foes[i]->setTranlateX(translateX + sin(foes[i]->getAngle()) * speed / 2 * deltaTimeSeconds);
			}
			else
			{
				if (foes[i]->getFollowMode())
				{
					foes[i]->setTranlateY(translateY + cos(foes[i]->getAngle()) * speed / 2 * deltaTimeSeconds);
					foes[i]->setTranlateX(translateX + sin(foes[i]->getAngle()) * speed / 2 * deltaTimeSeconds);
				}
				else
				{
					if (translateY < 3 && translateX <= 0)
					{
						foes[i]->setTranlateY(translateY + speed * deltaTimeSeconds);
					}


					if (translateY > 3 && translateX < 3)
					{
						foes[i]->setTranlateX(translateX + speed * deltaTimeSeconds);
					}

					if (translateY > 0 && translateX > 3)
					{
						foes[i]->setTranlateY(translateY - speed * deltaTimeSeconds);
					}

					if (translateY <= 0 && translateX >= 0)
					{
						foes[i]->setTranlateX(translateX - speed * deltaTimeSeconds);
					}
				}

			}
		}
		
		// Updating enemies angle
		float diffx = foes[i]->getX() + foes[i]->getTranslateX() - human->getFront().x;
		float diffz = foes[i]->getY() + foes[i]->getTranslateY() - human->getFront().z;
		float ang = atan2(-diffx, -diffz);
		foes[i]->setAngle(ang);
	}
	RenderFoes();

	// Clearing arrows
	auto iter = arrows.begin();
	while (iter != arrows.end())
	{
		if ((*iter)->getVis() == false)
		{
			iter = arrows.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	for (int i = 0; i < arrows.size(); i++)
	{
		// If arrow's time is above 1.5s, set visibility to false
		if ((float(clock() - arrows[i]->getTtl()) / CLOCKS_PER_SEC) > 1.5f)
		{
			arrows[i]->setVis(false);
			continue;
		}

		glm::vec3 position = arrows[i]->getPos();
		float posx = position.x;
		float posy = position.y;
		float posz = position.z;

		// Arrow - walls collision
		if (posx - 0.5 < 0 || posx + 0.5 > 50 || posz - 0.5 < 0 || posz + 0.5 > 50)
		{
			arrows[i]->setVis(false);
			continue;
		}

		Cell* c = maze[(int)posx / 5 + (int)posz / 5 * 10];

		if (c->getSouth() && posz - 0.5 <= 5 * c->getX()){
			arrows[i]->setVis(false);
			continue;
			
		}
		if (c->getNorth() && posz + 0.5 >= 5 * c->getX() + 5)
		{
			arrows[i]->setVis(false);
			continue;
		}

		if (c->getEast() && posx - 0.5 <= 5 * c->getY())
		{
			arrows[i]->setVis(false);
			continue;
		}

		if (c->getWest() && posx + 0.5 >= 5 * c->getY() + 5)
		{
			arrows[i]->setVis(false);
			continue;
		}

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(posx, posy, posz);
		modelMatrix *= Transform3D::Scale(0.2, 0.2, 0.2);
		modelMatrix *= Transform3D::RotateOY(arrows[i]->getAngle() + M_PI);
		modelMatrix *= Transform3D::RotateOZ(-arrows[i]->getDir().y);
		RenderSimpleMesh(meshes["arrow"], shaders["TextureShader"], modelMatrix, glm::vec3(1, 1, 1), mapTextures["arrow"], NULL);

		// Arrow - enemy collision
		for (int j = 0; j < foes.size(); j++)
		{
			float distx = posx - foes[j]->getX() - foes[j]->getTranslateX();
			float distz = posz - foes[j]->getY() - foes[j]->getTranslateY();
			if (sqrt(distx * distx + distz * distz) < 0.5)
			{
				arrows[i]->setVis(false);
				foes[j]->setIsHit(true);
				foes[j]->setTtl(clock());
			}
		}
		arrows[i]->setPos(arrows[i]->getPos() + arrows[i]->getDir() * deltaTimeSeconds * 5.0f);
	}

	Cell* humancell = getPlayerCell();

	// Nether Portal rendering
	if (abs(human->getCentre().x - 5 * endpos.y - 2.5) <= 2.5 && humancell->getX() != endpos.x) {
		portalangle = M_PI;
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(5 * endpos.y +2.5, 0, 5 * endpos.x + 2.5);
		modelMatrix *= Transform3D::Scale(0.5, 0.5, 0.5);
		modelMatrix *= Transform3D::Translate(2, 0, 0);

		modelMatrix *= Transform3D::RotateOY(portalangle);
		modelMatrix *= Transform3D::Translate(-2, 0, 0);
		RenderSimpleMesh(meshes["portal"], shaders["TextureShader"], modelMatrix, glm::vec3(0), mapTextures["portal"], NULL);
	}
	else
	{
		portalangle = -M_PI / 2;
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(5 * endpos.y +2.5, 0, 5 * endpos.x + 4.6);
		modelMatrix *= Transform3D::Scale(0.5, 0.5, 0.5);
		modelMatrix *= Transform3D::RotateOY(portalangle);
		RenderSimpleMesh(meshes["portal"], shaders["TextureShader"], modelMatrix, glm::vec3(0), mapTextures["portal"], NULL);
	}

	// TNT rendering
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tntpos.x, tntpos.y, tntpos.z);
	modelMatrix *= Transform3D::Scale(0.08, 0.08, 0.08);
	RenderSimpleMesh(meshes["TNT"], shaders["TextureShader"], modelMatrix, glm::vec3(1, 1, 1), mapTextures["TNT"], NULL);
	
	// Pig rendering
	if (pigvis) {
		float diffpigx = pigpos.x - human->getCentre().x;
		float diffpigz = pigpos.z - human->getCentre().z;
		float pigangle = atan2(-diffpigx, -diffpigz);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(pigpos.x, pigpos.y, pigpos.z);
		modelMatrix *= Transform3D::Scale(0.04, 0.04, 0.04);
		modelMatrix *= Transform3D::RotateOX(-M_PI / 2);
		modelMatrix *= Transform3D::RotateOZ(pigangle);
		RenderSimpleMesh(meshes["pig"], shaders["TextureShader"], modelMatrix, glm::vec3(1, 1, 1), mapTextures["pig"], NULL);
	}

	// HUD rendering
	if (thirdperson) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(human->getCentre().x + 0.5 * cos(angle - M_PI / 2), 2.7, human->getCentre().z + 0.5 * sin(angle - M_PI / 2));
		modelMatrix *= Transform3D::Scale(0.04 * playerhealth, 0.02, 0.04 * playerhealth);
		modelMatrix *= Transform3D::RotateOY(angle + M_PI / 2);
		RenderSimpleMesh(meshes["square"], shaders["ColorShader"], modelMatrix, glm::vec3(0, 1, 0), NULL, NULL);

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(human->getCentre().x + 0.5 * cos(angle - M_PI / 2), 2.8, human->getCentre().z + 0.5 * sin(angle - M_PI / 2));
		modelMatrix *= Transform3D::Scale(0.004 * timescale, 0.02, 0.004 * timescale);
		modelMatrix *= Transform3D::RotateOY(angle + M_PI / 2);
		RenderSimpleMesh(meshes["square"], shaders["ColorShader"], modelMatrix, glm::vec3(1, 1, 1), NULL, NULL);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		glm::mat3 mod = glm::mat3(1);
		mod *= Transf2D::Translate(1.2, 2);
		mod *= Transf2D::Scale(0.1* playerhealth, 0.7);
		RenderMesh2D(meshes["greenrec"], shaders["VertexColor"], mod);

		mod = glm::mat3(1);
		mod *= Transf2D::Translate(1.19, 1.99);
		mod *= Transf2D::Scale(0.5, 0.7);
		RenderMesh2D(meshes["greenout"], shaders["VertexColor"], mod);


		mod = glm::mat3(1);
		mod *= Transf2D::Translate(-2.2, 2);
		mod *= Transf2D::Scale(0.01 * timescale, 0.7);
		RenderMesh2D(meshes["whiterec"], shaders["VertexColor"], mod);

		mod = glm::mat3(1);
		mod *= Transf2D::Translate(-2.21, 1.99);
		mod *= Transf2D::Scale(0.5, 0.7);
		RenderMesh2D(meshes["whiteout"], shaders["VertexColor"], mod);
		glEnable(GL_DEPTH_TEST);
	}


}


// Maze initialization
void Tema2::InitMaze()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Cell* c = new Cell(i, j);
			if (i == 0)
			{
				c->setSouth(0);
			}
			if (i == 9)
			{
				c->setNorth(0);
			}
			if (j == 0)
			{
				c->setEast(0);
			}
			if (j == 9)
			{
				c->setWest(0);
			}
			maze.push_back(c);
		}
	}
}

void Tema2::LoadTextures()
{
	const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "textures");
	cout << sourceTextureDir << "\n";

	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "grass.png").c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "walls.jpg").c_str(), GL_REPEAT);
		mapTextures["walls"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "cobble.png").c_str(), GL_REPEAT);
		mapTextures["cobble"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "zombie.png").c_str(), GL_REPEAT);
		mapTextures["zombie"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "creeper.png").c_str(), GL_REPEAT);
		mapTextures["creeper"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "NetherPortal.png").c_str(), GL_REPEAT);
		mapTextures["portal"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "aarow.png").c_str(), GL_REPEAT);
		mapTextures["arrow"] = texture;
	}


	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "Minecraft_TNT_tex.png").c_str(), GL_REPEAT);
		mapTextures["TNT"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D(PATH_JOIN(sourceTextureDir, "pig.png").c_str(), GL_REPEAT);
		mapTextures["pig"] = texture;
	}

}

// Loading shaders
void Tema2::LoadShaders()
{
	// Color shader
	{
		Shader* shader = new Shader("ColorShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Texture shader
	{
		Shader* shader = new Shader("TextureShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TextVertShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TextFragShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Background shader
	{
		Shader* shader = new Shader("EnvironmentShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "BackVertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "BackFragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Noise shader
	{
		Shader* shader = new Shader("NoiseShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "NoiseVertShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "NoiseFragShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

// Maze generation 
// Depth first search implementation
void Tema2::GenerateMaze()
{
	int k = 0;
	stack.push(maze[startpos.y + 10 * startpos.x]);
	int diffmax = 0;
	int kmax = 0;
	while (!stack.empty())
	{
		Cell* c = stack.top();
		int currposx = c->getX();
		int currposy = c->getY();
		if (!c->getVisited()) {
			c->setVisited(1);
			k++;
		}
		std::vector<Cell*> adj = getAdj(c);
		if (adj.size() != 0)
		{
			Cell* next = adj[rand() % adj.size()];
			int nextposx = next->getX();
			int nextposy = next->getY();

			if (currposx + 1 == nextposx)
			{
				next->setSouth(0);
				c->setNorth(0);
			}

			if (nextposx + 1 == currposx)
			{
				next->setNorth(0);
				c->setSouth(0);
			}

			if (currposy + 1 == nextposy)
			{
				next->setEast(0);
				c->setWest(0);

			}

			if (nextposy + 1 == currposy)
			{
				next->setWest(0);
				c->setEast(0);
			}

			stack.push(next);

		}
		else
		{
		stack.pop();


		if (endpos.x == -1 && (currposx == 0 || currposx == 9 || currposy == 0 || currposy == 9))
		{
			endpos.x = currposx;
			endpos.y = currposy;
			kmax = k;
		}
		}
	}

}


// Get adjacent not visited cells
std::vector<Cell*> Tema2::getAdj(Cell* cell)
{
	std::vector<Cell*> adj;
	int posx = cell->getX();
	int posy = cell->getY();

	if (posx + 1 != 10 && !maze[posy + 10 * (posx + 1)]->getVisited())
	{
		adj.push_back(maze[posy + 10 * (posx + 1)]);
	}

	if (posx - 1 != -1 && !maze[posy + 10 * (posx - 1)]->getVisited())
	{
		adj.push_back(maze[posy + 10 * (posx - 1)]);
	}

	if (posy + 1 != 10 && !maze[posy + 1 + 10 * posx]->getVisited())
	{
		adj.push_back(maze[posy + 1 + 10 * posx]);
	}

	if (posy - 1 != -1 && !maze[posy - 1 + 10 * posx]->getVisited())
	{
		adj.push_back(maze[posy - 1 + 10 * posx]);
	}

	return adj;
}

// Render player
void Tema2::RenderPlayer()
{
	glm::vec3 coord = human->getCoord();
	float x = coord.x;
	float y = coord.y;
	float z = coord.z;
	float angle = human->getAngle();

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(x, y, z);
	modelMatrix *= Transform3D::Translate(0.55, 0, 0.25);
	modelMatrix *= Transform3D::RotateOY(angle + M_PI / 2);
	modelMatrix *= Transform3D::Translate(-0.55, 0, -0.25);

	modelMatrix *= Transform3D::Scale(0.5, 1, 0.5);
	RenderSimpleMesh(meshes["cube"], shaders["ColorShader"], modelMatrix, glm::vec3(0.22, 0.25, 0.58), NULL, NULL);
	modelMatrix *= Transform3D::Translate(1.2, 0, 0);
	RenderSimpleMesh(meshes["cube"], shaders["ColorShader"], modelMatrix, glm::vec3(0.22, 0.25, 0.58), NULL, NULL);
	modelMatrix *= Transform3D::Translate(-1.2, 1.05f, 0);
	modelMatrix *= Transform3D::Scale(2.2, 1, 1);
	RenderSimpleMesh(meshes["cube"], shaders["ColorShader"], modelMatrix, glm::vec3(0.28, 0.52, 0.53), NULL, NULL);
	modelMatrix *= Transform3D::Translate(-0.3, 0.4, 0);
	modelMatrix *= Transform3D::Scale(0.27, 0.6, 1);
	RenderSimpleMesh(meshes["cube"], shaders["ColorShader"], modelMatrix, glm::vec3(0.28, 0.52, 0.53), NULL, NULL);
	modelMatrix *= Transform3D::Translate(5, 0, 0);
	RenderSimpleMesh(meshes["cube"], shaders["ColorShader"], modelMatrix, glm::vec3(0.28, 0.52, 0.53), NULL, NULL);
	modelMatrix *= Transform3D::Scale(1, 0.4, 1);
	modelMatrix *= Transform3D::Translate(-5, -1.1, 0);
	RenderSimpleMesh(meshes["cube"], shaders["ColorShader"], modelMatrix, glm::vec3(0.86, 0.78, 0.67), NULL, NULL);
	modelMatrix *= Transform3D::Translate(5, 0, 0);
	RenderSimpleMesh(meshes["cube"], shaders["ColorShader"], modelMatrix, glm::vec3(0.86, 0.78, 0.67), NULL, NULL);
	modelMatrix *= Transform3D::Translate(-3, 3.8, 0);
	modelMatrix *= Transform3D::Scale(2, 1.7, 1);
	RenderSimpleMesh(meshes["cube"], shaders["ColorShader"], modelMatrix, glm::vec3(0.86, 0.78, 0.67), NULL, NULL);


}

// Render enemies
void Tema2::RenderFoes()
{
	for (int i = 0; i < foes.size(); i++)
	{
		float translateX = foes[i]->getTranslateX();
		float translateY = foes[i]->getTranslateY();
		float angle = foes[i]->getAngle();
		
		// type = 0 => zombie
		// type = 1 => creeper
		if (foes[i]->getType() == 0) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(foes[i]->getX() + translateX, 0.95, foes[i]->getY() + translateY);
			modelMatrix *= Transform3D::Scale(0.08, 0.08, 0.08);
			modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
			modelMatrix *= Transform3D::RotateOZ(angle);
			if (foes[i]->getIsHit() == true)
			{
				// If hit, render with noise shader for 1,5s
				if ((float(clock() - foes[i]->getTtl()) / CLOCKS_PER_SEC) < 1.5)
				{
					RenderSimpleMesh(meshes["zombie"], shaders["NoiseShader"], modelMatrix, glm::vec3(0), mapTextures["zombie"], NULL);
				}
				else
				{
					foes[i]->setVis(false);
				}
			}
			else
			{
				RenderSimpleMesh(meshes["zombie"], shaders["TextureShader"], modelMatrix, glm::vec3(0), mapTextures["zombie"], NULL);
			}

		}
		else {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(foes[i]->getX() + translateX, 0.5, foes[i]->getY() + translateY);
			modelMatrix *= Transform3D::Scale(0.08, 0.08, 0.08);
			modelMatrix *= Transform3D::RotateOX(RADIANS(-90));
			modelMatrix *= Transform3D::RotateOZ(angle);
			if (foes[i]->getIsHit() == true)
			{
				// If hit, render with noise shader for 1.5s
				if ((float(clock() - foes[i]->getTtl()) / CLOCKS_PER_SEC) < 1.5)
				{
					RenderSimpleMesh(meshes["creeper"], shaders["NoiseShader"], modelMatrix, glm::vec3(0), mapTextures["creeper"], NULL);
				}
				else
				{
					foes[i]->setVis(false);
				}
			}
			else
			{
				RenderSimpleMesh(meshes["creeper"], shaders["TextureShader"], modelMatrix, glm::vec3(0), mapTextures["creeper"], NULL);
			}

		}
	}
}

// Render environment
void Tema2::RenderEnvironment()
{
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(25, 0, 25);
	RenderSimpleMesh(meshes["plane"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["grass"], NULL);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0, 0, 25);
	modelMatrix *= Transform3D::RotateOZ(RADIANS(90));
	modelMatrix *= Transform3D::RotateOY(RADIANS(90));
	RenderSimpleMesh(meshes["plane"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["walls"], NULL);


	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(25, 0, 0);
	modelMatrix *= Transform3D::RotateOX(RADIANS(90));
	RenderSimpleMesh(meshes["plane"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["walls"], NULL);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(25, 0, 50);
	modelMatrix *= Transform3D::RotateOX(RADIANS(90));
	RenderSimpleMesh(meshes["plane"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["walls"], NULL);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(50, 0, 25);
	modelMatrix *= Transform3D::RotateOX(RADIANS(90));
	modelMatrix *= Transform3D::RotateOZ(RADIANS(90));
	RenderSimpleMesh(meshes["plane"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["walls"], NULL);


	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(25, 25, 25);
	RenderSimpleMesh(meshes["plane"], shaders["ColorShader"], modelMatrix, glm::vec3(0.51, 0.75, 0.89), NULL, NULL);
}

// Render maze
void Tema2::RenderMaze()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{

			if (maze[j + 10 * i]->getSouth() == 1)
			{
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(5 * j, 0, 5 * i);
				modelMatrix *= Transform3D::Scale(1, 0.8, 1);
				RenderSimpleMesh(meshes["square"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["cobble"], NULL);

			}
			if (maze[j + 10 * i]->getNorth() == 1)
			{
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(5 * j, 0, 5 * i);
				modelMatrix *= Transform3D::Translate(0, 0, 5);
				modelMatrix *= Transform3D::Scale(1, 0.8, 1);
				RenderSimpleMesh(meshes["square"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["cobble"], NULL);
			}

			if (maze[j + 10 * i]->getEast() == 1)
			{
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(5 * j, 0, 5 * i);
				modelMatrix *= Transform3D::Scale(1, 0.8, 1);
				modelMatrix *= Transform3D::RotateOY(RADIANS(90));
				RenderSimpleMesh(meshes["square"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["cobble"], NULL);
			}

			if (maze[j + 10 * i]->getWest() == 1)
			{
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(5 * j, 0, 5 * i);
				modelMatrix *= Transform3D::Translate(5, 0, 0);
				modelMatrix *= Transform3D::Scale(1, 0.8, 1);
				modelMatrix *= Transform3D::RotateOY(RADIANS(90));
				RenderSimpleMesh(meshes["square"], shaders["EnvironmentShader"], modelMatrix, glm::vec3(0), mapTextures["cobble"], NULL);
			}
		}
	}
}

void Tema2::FrameEnd()
{
	//DrawCoordinateSystem();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	GLint locLightPos = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(locLightPos, 1, glm::value_ptr(lightPosition));

	GLint locMaterial = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(locMaterial, materialShininess);

	GLint locMaterialKd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(locMaterialKd, materialKd);

	GLint locMaterialKs = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(locMaterialKs, materialKs);

	GLint locObject = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(locObject, 1, glm::value_ptr(color));

	GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(timeLocation, (GLfloat)Engine::GetElapsedTime());

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	// Bind projection matrix
	//glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
		glUniform1i(glGetUniformLocation(shader->program, "mixed"), false);

	}

	if (texture2)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
		glUniform1i(glGetUniformLocation(shader->program, "mixed"), true);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}




/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// Player's movement
	glm::vec3 coord = human->getCoord();
	float posx = coord.x;
	float posy = coord.y;
	float posz = coord.z;
	float angle = human->getAngle();
	glm::vec3 centre = human->getCentre();
	float centrex = centre.x;
	float centrey = centre.y;
	float centrez = centre.z;

	glm::vec3 front = human->getFront();
	float frontx = front.x;
	float fronty = front.y;
	float frontz = front.z;

	glm::vec3 back = human->getBack();
	float backx = back.x;
	float backy = back.y;
	float backz = back.z;

	glm::vec3 right = human->getRight();
	float rightx = right.x;
	float righty = right.y;
	float rightz = right.z;

	glm::vec3 left = human->getLeft();
	float leftx = left.x;
	float lefty = left.y;
	float leftz = left.z;

	if (window->KeyHold(GLFW_KEY_W)) {
		if (!walls_down) {
			if (!checkColWall(frontx + 5 * deltaTime * cos(angle), frontz + 5 * deltaTime * sin(angle))) {

				human->setCoord(posx + 5 * deltaTime * cos(angle), posy, posz + 5 * deltaTime * sin(angle));
				human->setCentre(centrex + 5 * deltaTime * cos(angle), centrey, centrez + 5 * deltaTime * sin(angle));

				if (thirdperson) {
					camera->Set(camera->position + glm::vec3(5 * deltaTime * cos(angle), 0, 5 * deltaTime * sin(angle)),
						glm::vec3(centrex + 5 * deltaTime * cos(angle), centrey, centrez + 5 * deltaTime * sin(angle)), glm::vec3(0, 1, 0));
				}
				else
				{
					camera->Set(glm::vec3(centrex + (5 * deltaTime) * cos(angle), 2, centrez + (5 * deltaTime) * sin(angle)),
						glm::vec3(centrex + (5 * deltaTime + 2) * cos(angle), human->getCameraPos(), centrez + (5 * deltaTime + 2) * sin(angle)), glm::vec3(0, 1, 0));
				}
			}
		}
		else
		{
			human->setCoord(posx + 5 * deltaTime * cos(angle), posy, posz + 5 * deltaTime * sin(angle));
			human->setCentre(centrex + 5 * deltaTime * cos(angle), centrey, centrez + 5 * deltaTime * sin(angle));

			if (thirdperson) {
				camera->Set(camera->position + glm::vec3(5 * deltaTime * cos(angle), 0, 5 * deltaTime * sin(angle)),
					glm::vec3(centrex + 5 * deltaTime * cos(angle), centrey, centrez + 5 * deltaTime * sin(angle)), glm::vec3(0, 1, 0));
			}
			else
			{
				camera->Set(glm::vec3(centrex + (5 * deltaTime) * cos(angle), 2, centrez + (5 * deltaTime) * sin(angle)),
					glm::vec3(centrex + (5 * deltaTime + 2) * cos(angle), human->getCameraPos(), centrez + (5 * deltaTime + 2) * sin(angle)), glm::vec3(0, 1, 0));
			}
		}

	}
	if (window->KeyHold(GLFW_KEY_S)) {
		if (!walls_down) {
			if (!checkColWall(backx - 5 * deltaTime * cos(angle), backz - 5 * deltaTime * sin(angle))) {

				human->setCoord(posx - 5 * deltaTime * cos(angle), posy, posz - 5 * deltaTime * sin(angle));
				human->setCentre(centrex - 5 * deltaTime * cos(angle), centrey, centrez - 5 * deltaTime * sin(angle));

				if (thirdperson) {
					camera->Set(camera->position + glm::vec3(-5 * deltaTime * cos(angle), 0, -5 * deltaTime * sin(angle)),
						glm::vec3(centrex - 5 * deltaTime * cos(angle), centrey, centrez - 5 * deltaTime * sin(angle)), glm::vec3(0, 1, 0));
				}
				else
				{
					glm::vec3 front = human->getFront();
					float angle = human->getAngle();
					camera->Set(glm::vec3(centrex + (-5 * deltaTime) * cos(angle), 2, centrez + (-5 * deltaTime) * sin(angle)),
						glm::vec3(centrex + (-5 * deltaTime + 2) * cos(angle), human->getCameraPos(), centrez + (-5 * deltaTime + 2) * sin(angle)), glm::vec3(0, 1, 0));
				}
			}
		}
		else
		{
			human->setCoord(posx - 3 * deltaTime * cos(angle), posy, posz - 3 * deltaTime * sin(angle));
			human->setCentre(centrex - 3 * deltaTime * cos(angle), centrey, centrez - 3 * deltaTime * sin(angle));

			if (thirdperson) {
				camera->Set(camera->position + glm::vec3(-3 * deltaTime * cos(angle), 0, -3 * deltaTime * sin(angle)),
					glm::vec3(centrex - 3 * deltaTime * cos(angle), centrey, centrez - 3 * deltaTime * sin(angle)), glm::vec3(0, 1, 0));
			}
			else
			{
				glm::vec3 front = human->getFront();
				float angle = human->getAngle();
				camera->Set(glm::vec3(centrex + (-3 * deltaTime) * cos(angle), 2, centrez + (-3 * deltaTime) * sin(angle)),
					glm::vec3(centrex + (-3 * deltaTime + 2) * cos(angle), human->getCameraPos(), centrez + (-3 * deltaTime + 2) * sin(angle)), glm::vec3(0, 1, 0));
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		if (thirdperson) {
			human->setAngle(angle - 2 * deltaTime);

			glm::vec3 newcentre = human->getCentre();
			float newcentrex = newcentre.x;
			float newcentrey = newcentre.y;
			float newcentrez = newcentre.z;
			float newangle = human->getAngle();
			camera->Set(camera->position, glm::vec3(newcentrex, newcentrey, newcentrez), glm::vec3(0, 1, 0));
		}
		else
		{
			if (!walls_down) {
				if (!checkColWall(leftx + 3 * deltaTime * cos(angle - M_PI / 2), leftz + 3 * deltaTime * sin(angle - M_PI / 2))) {
					human->setCoord(posx + 3 * deltaTime * cos(angle - M_PI / 2), posy, posz + 3 * deltaTime * sin(angle - M_PI / 2));
					human->setCentre(centrex + 3 * deltaTime * cos(angle - M_PI / 2), centrey, centrez + 3 * deltaTime * sin(angle - M_PI / 2));
					camera->Set(camera->position + glm::vec3(3 * deltaTime * cos(angle - M_PI / 2), 0, 3 * deltaTime * sin(angle - M_PI / 2)), camera->GetTargetPosition() + glm::vec3(3 * deltaTime * cos(angle - M_PI / 2), 0, 3 * deltaTime * sin(angle - M_PI / 2)), glm::vec3(0, 1, 0));
				}
			}
			else
			{
				human->setCoord(posx + 3 * deltaTime * cos(angle - M_PI / 2), posy, posz + 3 * deltaTime * sin(angle - M_PI / 2));
				human->setCentre(centrex + 3 * deltaTime * cos(angle - M_PI / 2), centrey, centrez + 3 * deltaTime * sin(angle - M_PI / 2));
				camera->Set(camera->position + glm::vec3(3 * deltaTime * cos(angle - M_PI / 2), 0, 3 * deltaTime * sin(angle - M_PI / 2)), camera->GetTargetPosition() + glm::vec3(3 * deltaTime * cos(angle - M_PI / 2), 0, 3 * deltaTime * sin(angle - M_PI / 2)), glm::vec3(0, 1, 0));
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		if (thirdperson) {
			human->setAngle(angle + 2 * deltaTime);
			glm::vec3 newcentre = human->getCentre();
			float newcentrex = newcentre.x;
			float newcentrey = newcentre.y;
			float newcentrez = newcentre.z;
			float newangle = human->getAngle();
			camera->Set(camera->position, glm::vec3(newcentrex, newcentrey, newcentrez), glm::vec3(0, 1, 0));
		}
		else
		{
			if (!walls_down) {
				if (!checkColWall(rightx + 3 * deltaTime * cos(angle - M_PI / 2), rightz + 3 * deltaTime * sin(angle - M_PI / 2))) {
					human->setCoord(posx + 3 * deltaTime * cos(angle + M_PI / 2), posy, posz + 3 * deltaTime * sin(angle + M_PI / 2));
					human->setCentre(centrex + 3 * deltaTime * cos(angle + M_PI / 2), centrey, centrez + 3 * deltaTime * sin(angle + M_PI / 2));
					camera->Set(camera->position + glm::vec3(3 * deltaTime * cos(angle + M_PI / 2), 0, 3 * deltaTime * sin(angle + M_PI / 2)), camera->GetTargetPosition() + glm::vec3(3 * deltaTime * cos(angle + M_PI / 2), 0, 3 * deltaTime * sin(angle + M_PI / 2)), glm::vec3(0, 1, 0));
				}
			}
			else
			{
				human->setCoord(posx + 3 * deltaTime * cos(angle + M_PI / 2), posy, posz + 3 * deltaTime * sin(angle + M_PI / 2));
				human->setCentre(centrex + 3 * deltaTime * cos(angle + M_PI / 2), centrey, centrez + 3 * deltaTime * sin(angle + M_PI / 2));
				camera->Set(camera->position + glm::vec3(3 * deltaTime * cos(angle + M_PI / 2), 0, 3 * deltaTime * sin(angle + M_PI / 2)), camera->GetTargetPosition() + glm::vec3(3 * deltaTime * cos(angle + M_PI / 2), 0, 3 * deltaTime * sin(angle + M_PI / 2)), glm::vec3(0, 1, 0));
			}
		}
	}

	// FOV update
	if (window->KeyHold(GLFW_KEY_Z)) {
		fov += deltaTime * 12;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
	}

	if (window->KeyHold(GLFW_KEY_X)) {
		fov -= deltaTime * 12;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
	}
}

Cell* Tema2::getPlayerCell()
{
	Cell* c = maze[(int)human->getCentre().x / 5 + (int)human->getCentre().z / 5 * 10];
	return c;
}

// Player - wall collision
bool Tema2::checkColWall(float x, float z)
{
	Cell* c;
	int cellx;
	int celly;

	c = getPlayerCell();
	cellx = c->getX();
	celly = c->getY();



	if (c->getSouth() == 1 && z <= 5 * cellx)
	{
		return true;
	}

	if (c->getNorth() == 1 && z >= 5 * cellx + 5)
	{
		return true;
	}

	if (c->getEast() == 1 && x <= 5 * celly)
	{
		return true;
	}

	if (c->getWest() == 1 && x >= 5 * celly + 5)
	{
		return true;
	}

	if (x < 0 || x>50 || z < 0 || z>50)
	{
		return true;
	}
	return false;
}


void Tema2::OnKeyPress(int key, int mods)
{
	// Camera switch
	if (key == GLFW_KEY_LEFT_CONTROL)
	{
		if (thirdperson)
		{
			thirdperson = false;
			glm::vec3 front = human->getFront();
			glm::vec3 centre = human->getCentre();
			float angle = human->getAngle();
			camera->Set(glm::vec3(centre.x, 2, centre.z), glm::vec3(centre.x + 2 * cos(angle), human->getCameraPos(), centre.z + 2 * sin(angle)), glm::vec3(0, 1, 0));
		}
		else {
			thirdperson = true;
			glm::vec3 back = human->getCentre();
			float posx = back.x;
			float posy = back.y;
			float posz = back.z;
			float angle = human->getAngle();
			glm::mat4 thirdPersCameraPosition = glm::mat4(1);
			thirdPersCameraPosition = Transform3D::Translate(posx + -5 * cos(angle), 3, posz - 5 * sin(angle));
			camera = new Camera();
			camera->Set(thirdPersCameraPosition * glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(posx, 2, posz), glm::vec3(0, 1, 0));
		}
	}


}


void Tema2::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event

	float sensivityOX = 0.001f;
	float sensivityOY = 0.001f;

	if (thirdperson) {
		camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
		camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
	}
	else
	{
		glm::vec3 front = human->getFront();
		float angle = human->getAngle();
		float camerapos = human->getCameraPos();
		camera->RotateFirstPerson_OX(sensivityOX * -deltaY);	
		camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
		human->setAngle(human->getAngle() + deltaX * sensivityOY);
		human->setCameraPos(camerapos + sensivityOX * -deltaY);
		glm::vec3 centre = human->getCentre();
		//camera->Set(glm::vec3(front.x + cos(angle), 2, front.z + sin(angle)), glm::vec3(front.x + 2 * cos(angle), human->getCameraPos(), front.z + 2 * sin(angle)), glm::vec3(0, 1, 0));

		camera->Set(glm::vec3(centre.x, 2, centre.z ), glm::vec3(centre.x + 2 * cos(angle), human->getCameraPos(), centre.z + 2 * sin(angle)), glm::vec3(0, 1, 0));
		//camera->Set(glm::vec3(front.x + cos(angle - deltaX * sensivityOY), 2, front.z + sin(angle - deltaX * sensivityOY)), camera->GetTargetPosition(), glm::vec3(0, 1, 0));
	}

}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
	// Arrow shoot
	if (button == 1 && (float(clock() - arrow_clock) / CLOCKS_PER_SEC) > 1)
	{
		arrow_clock = clock();
		Arrow* arrow = new Arrow(camera->GetTargetPosition() - glm::normalize(camera->forward) * 4.0f, human->getAngle());
		arrow->setDir(glm::normalize(camera->forward));
		arrow->setTtl(clock());
		arrows.push_back(arrow);
	}
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
