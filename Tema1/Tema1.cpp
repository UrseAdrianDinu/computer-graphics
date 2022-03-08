#include "Tema1.h"

#include <vector>
#include <iostream>

#include "Objects2D.h"
#include "Transform2D.h"

using namespace std;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
    playertranslateX = 0;
    playertranslateY = 0;
    angularStep = 0;
    bulletAngle = 0;
    MAP_HEIGHT = MAP_WIDTH = 50;
    modelMatrix = glm::mat3(1);
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 40;   // logic width
    logicSpace.height = 40;  // logic height


    // Initialize tx and ty (the translation steps)
    playertranslateX = 0;
    playertranslateY = 0;


    Mesh* obstacle1 = Objects2D::CreateRectangle("obstacle1", glm::vec3(10, 4, 0), 10, 2, glm::vec3(0.23, 0.89, 0.41),true);
    AddMeshToList(obstacle1);
    obscoords.push_back(glm::vec3(10, 4, 0));
    obsdims.push_back(glm::vec3(10, 2, 0));

    Mesh* obstacle2 = Objects2D::CreateRectangle("obstacle2", glm::vec3(10, 4, 0), 2, 12, glm::vec3(0.23, 0.89, 0.41), true);
    AddMeshToList(obstacle2);
    obscoords.push_back(glm::vec3(10, 4, 0));
    obsdims.push_back(glm::vec3(2, 12, 0));

    Mesh* obstacle3 = Objects2D::CreateSquare("obstacle3", glm::vec3(40, 4, 0), 5, glm::vec3(0.23, 0.89, 0.41), true);
    AddMeshToList(obstacle3);
    obscoords.push_back(glm::vec3(40, 4, 0));
    obsdims.push_back(glm::vec3(5, 5, 0));

    Mesh* obstacle4 = Objects2D::CreateSquare("obstacle4", glm::vec3(35, 30, 0), 7, glm::vec3(0.23, 0.89, 0.41), true);
    AddMeshToList(obstacle4);
    obscoords.push_back(glm::vec3(35, 30, 0));
    obsdims.push_back(glm::vec3(7, 7, 0));
    
    Mesh* obstacle5 = Objects2D::CreateRectangle("obstacle5", glm::vec3(14, 40, 0), 10, 5, glm::vec3(0.23, 0.89, 0.41), true);
    AddMeshToList(obstacle5);
    obscoords.push_back(glm::vec3(14, 40, 0));
    obsdims.push_back(glm::vec3(10, 5, 0));

    Mesh* obstacle6 = Objects2D::CreateSquare("obstacle6", glm::vec3(6, 30, 0), 6, glm::vec3(0.23, 0.89, 0.41), true);
    AddMeshToList(obstacle6);
    obscoords.push_back(glm::vec3(6, 30, 0));
    obsdims.push_back(glm::vec3(6, 6, 0));

    Mesh* map1 = Objects2D::CreateSquare("map1", glm::vec3(1, 1, 0), MAP_HEIGHT, glm::vec3(0.56, 0.66, 0.86), true);
    AddMeshToList(map1);

    Mesh* map2 = Objects2D::CreateSquare("map2", glm::vec3(0, 0, 0), MAP_HEIGHT + 2, glm::vec3(0, 0, 0), true);
    AddMeshToList(map2);

    Mesh* bullet = Objects2D::CreateSquare("bullet", glm::vec3(0, 0, 0), 1, glm::vec3(0, 0, 0), true);
    AddMeshToList(bullet);

    Mesh* enemy = Objects2D::CreateSquare("enemy", glm::vec3(0, 0, 0), 2.5f, glm::vec3(0.73f, 0.10, 0.26), true);
    AddMeshToList(enemy);
    
    Mesh* enemyoutline = Objects2D::CreateSquare("enemyout", glm::vec3(0, 0, 0), 3.0f, glm::vec3(0, 0, 0), true);
    AddMeshToList(enemyoutline);

    Mesh* enemyhand = Objects2D::CreateSquare("enemyhand", glm::vec3(0, 0, 0), 1.2f, glm::vec3(0.73f, 0.10, 0.26), true);
    AddMeshToList(enemyhand);

    Mesh* enemyhandoutline = Objects2D::CreateSquare("enemyhandout", glm::vec3(0, 0, 0), 1.6f, glm::vec3(0, 0, 0), true);
    AddMeshToList(enemyhandoutline);

    Mesh* circle = Objects2D::CreateCircle("circle", glm::vec3(0, 0, 0), 0.3, glm::vec3(1, 1, 1), true);
    AddMeshToList(circle);

    Mesh* test = Objects2D::CreateSquare("test", glm::vec3(0, 0, 0), 3.0f, glm::vec3(0, 1, 1), true);
    AddMeshToList(test);

    Mesh* healthbar = Objects2D::CreateRectangle("healthbar", glm::vec3(0, 0, 0), 10, 3, glm::vec3(1, 0, 0), true);
    AddMeshToList(healthbar);

    Mesh* healthbarout = Objects2D::CreateRectangle("healthbarout", glm::vec3(0, 0, 0), 10.4, 3.4, glm::vec3(1,0,0 ), false);
    AddMeshToList(healthbarout);

    Mesh* potion = Objects2D::CreateSquare("potion", glm::vec3(0, 0, 0), 2.0f, glm::vec3(0, 1, 0), true);
    AddMeshToList(potion);

    Mesh *potionout = Objects2D::CreateSquare("potionout", glm::vec3(0, 0, 0), 2.4f, glm::vec3(0, 0, 0), true);
    AddMeshToList(potionout);

    Mesh* ver = Objects2D::CreateRectangle("ver", glm::vec3(0, 0, 0), 0.5, 2, glm::vec3(1, 1, 1), true);
    AddMeshToList(ver);

    Mesh* hor = Objects2D::CreateRectangle("hor", glm::vec3(0, 0, 0), 2, 0.5, glm::vec3(1, 1, 1), true);
    AddMeshToList(hor);

    Mesh *fireratepotion = Objects2D::CreateSquare("fireratepotion", glm::vec3(0, 0, 0), 2.0f, glm::vec3(0.34f, 0.03f, 0.43f), true);
    AddMeshToList(fireratepotion);

        
    player = new Player();
    player->setPlayerCoord(logicSpace.width/2, logicSpace.height/2);
    player->createPlayer();

    srand(time(NULL));

    Enemy* enemy1 = new Enemy();
    enemy1->setEnemyCoord(10.25, 20.25);
    float v1 = rand() % 7 + 1;
    enemy1->setSpeed(v1);
    enemies.push_back(*enemy1);

    Enemy* enemy2 = new Enemy();
    enemy2->setEnemyCoord(25.25, 30.25);
    float v2 = rand() % 7 + 1;
    enemy2->setSpeed(v2);
    enemies.push_back(*enemy2);

    Enemy* enemy3 = new Enemy();
    enemy3->setEnemyCoord(30.25, 5.25);
    float v3 = rand() % 7 + 1;
    enemy3->setSpeed(v3);
    enemies.push_back(*enemy3);

    Enemy* enemy4 = new Enemy();
    enemy4->setEnemyCoord(30, 25);
    float v4 = rand() % 7 + 1;
    enemy4->setSpeed(v4);
    enemies.push_back(*enemy4);

    Enemy* enemy5 = new Enemy();
    enemy5->setEnemyCoord(5, 5);
    float v5 = rand() % 7 + 1;
    enemy5->setSpeed(v5);
    enemies.push_back(*enemy5);
    noEnemies = 5;
    cout << "SCORE = " << score << '\n';
    
}




// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.53, 0.56, 0.56, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.53, 0.56, 0.56), true);

    float posx = player->getBodyCoord().x;
    float posy = player->getBodyCoord().y;
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= Transform2D::Translate(playertranslateX, playertranslateY);
    modelMatrix *= Transform2D::Translate(+posx , +posy);
    modelMatrix *= Transform2D::Rotate(angularStep);
    modelMatrix *= Transform2D::Translate(-posx, -posy);

    RenderMesh2D(player->getLeftHandIn(), shaders["VertexColor"], modelMatrix);
    RenderMesh2D(player->getLeftHandOut(), shaders["VertexColor"], modelMatrix);
    RenderMesh2D(player->getRightHandIn(), shaders["VertexColor"], modelMatrix);
    RenderMesh2D(player->getRightHandOut(), shaders["VertexColor"], modelMatrix);
    RenderMesh2D(player->getBodyIn(), shaders["VertexColor"], modelMatrix);
    RenderMesh2D(player->getBodyOut(), shaders["VertexColor"], modelMatrix);
    
    EnemyCol(playertranslateX, playertranslateY);

    // Stergere inamici din vectorul de inamici enemies
    auto it = enemies.begin();
    while (it != enemies.end())
    {
        if (it->getVis() == false)
        {
            it = enemies.erase(it);
            noEnemies--;
        }
        else
        {
            it++;
        }
    }

    // Adaugare inamici in vector pana cand sunt 5 inamici
    while (noEnemies < 5)
    {
        float initx = rand() % 47 + 3;
        float inity = rand() % 47 + 3;
        float difx = initx - posx - playertranslateX;
        float dify = inity - posy - playertranslateY;
        while (sqrt(difx*difx - dify*dify)<=4) {
            initx = rand() % 47 + 3;
            inity = rand() % 47 + 3;
            difx = initx - posx - playertranslateX;
            dify = inity - posy - playertranslateY;
        }
        Enemy* newenemy = new Enemy();
        float v;
        // Daca este activat fury mode, inamicii au viteza mai mare
        if(furymode)
            v = rand() % 7 + 3;
        else
            v = rand() % 7 + 1;
        newenemy->setSpeed(v);
        newenemy->setEnemyCoord(initx, inity);
        enemies.push_back(*newenemy);
        noEnemies++;
    }


    for (int i = 0; i < enemies.size(); i++)
    {

        float enemyx = enemies[i].getX();
        float enemyy = enemies[i].getY();
       
        // Centrul inamicului
        //Centre
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(enemyx + 1.5f, enemyy + 1.5f);
        float a = modelMatrix[2][0];
        float b = modelMatrix[2][1];
        enemies[i].setCentre(a, b);

        float difx = posx + playertranslateX - a;
        float dify = posy + playertranslateY - b;

        float ang = atan2(-difx, dify);
        enemies[i].setEnemyAngle(ang);
        
        // Colturi
        // Bottom Left
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(enemyx,enemyy);
        modelMatrix *= Transform2D::Translate(+1.5, +1.5);
        modelMatrix *= Transform2D::Rotate(ang);
        modelMatrix *= Transform2D::Translate(-1.5, -1.5);

         a = modelMatrix[2][0];
         b = modelMatrix[2][1];
        enemies[i].setBottomLeft(a, b);


        //Top Right
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(enemyx + 3, enemyy + 3);
        modelMatrix *= Transform2D::Translate(-1.5, -1.5);
        modelMatrix *= Transform2D::Rotate(ang);
        modelMatrix *= Transform2D::Translate(1.5, 1.5);
        a = modelMatrix[2][0];
        b = modelMatrix[2][1];
        enemies[i].setTopRight(a, b);

        // Coliziune inamic-margine harta
        if (a <= 2 || a >= MAP_HEIGHT || b <= 2 || b >= MAP_HEIGHT)
        {
            enemies[i].setVis(false);
        }

        // Top Left
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(enemyx, enemyy + 3);
        modelMatrix *= Transform2D::Translate(1.5, -1.5);
        modelMatrix *= Transform2D::Rotate(ang);
        modelMatrix *= Transform2D::Translate(-1.5, +1.5);
        a = modelMatrix[2][0];
        b = modelMatrix[2][1];
        enemies[i].setTopLeft(a, b);

        // Coliziune inamic-margine harta
        if (a <= 2 || a >= MAP_HEIGHT || b <= 2 || b >= MAP_HEIGHT)
        {
            enemies[i].setVis(false);
        }

        // Bottom Right
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(enemyx +3, enemyy);
        modelMatrix *= Transform2D::Translate(-1.5, 1.5);
        modelMatrix *= Transform2D::Rotate(ang);
        modelMatrix *= Transform2D::Translate(1.5, -1.5);
        a = modelMatrix[2][0];
        b = modelMatrix[2][1];
        enemies[i].setBottomRight(a, b);


        float speed = enemies[i].getSpeed();
        enemyx += (double)speed * deltaTimeSeconds * cos(ang + M_PI / 2);
        enemyy += (double)speed * deltaTimeSeconds * sin(ang + M_PI / 2);
        enemies[i].setEnemyCoord(enemyx , enemyy );
        RenderEnemy(enemies[i]);
    }

    // Stergere proiectile din vectorul de proiectile bullets
    auto iter = bullets.begin();
    while (iter != bullets.end())
    {
        if (iter->getVis() == false)
        {
            iter = bullets.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    // Daca a fost apasat un mouseButton
    if (shoot) {
        for (int i = 0; i < bullets.size(); i++)
        {
            bool vis = bullets[i].getVis();
            float ttl = bullets[i].getTtl();

            // Dupa 1.5s proiectilul este sters
            if ((float(clock() - ttl) / CLOCKS_PER_SEC) > 1.5f)
            {
                bullets[i].setVis(false);
                continue;
            }
            float bulletposx = bullets[i].getX();
            float bulletposy = bullets[i].getY();

            float bulletangle = bullets[i].getAngle();
            float futureposx = bulletposx + 10 * (double)deltaTimeSeconds * cos(bulletangle + M_PI / 2);
            float futureposy = bulletposy + 10 * (double)deltaTimeSeconds * sin(bulletangle + M_PI / 2);

            // Determinare colturi
            // Bottom Left
            glm::mat3 c = glm::mat3(1);
            c *= Transform2D::Translate(futureposx, futureposy);
            c *= Transform2D::Translate(0.5, 0.5);
            c *= Transform2D::Rotate(bulletangle);
            c *= Transform2D::Translate(-0.5, -0.5);
            float bottomleftx = c[2][0];
            float bottomlefty = c[2][1];
            bullets[i].setBottomLeft(bottomleftx, bottomlefty);

            //Bottom Right
            c = glm::mat3(1);
            c *= Transform2D::Translate(futureposx + 1, futureposy);
            c *= Transform2D::Translate(-0.5, 0.5);
            c *= Transform2D::Rotate(bulletangle);
            c *= Transform2D::Translate(0.5, -0.5);
            float bottomrightx = c[2][0];
            float bottomrighty = c[2][1];
            bullets[i].setBottomRight(bottomrightx, bottomrighty);

            // Top Right
            c = glm::mat3(1);
            c *= Transform2D::Translate(futureposx + 1, futureposy + 1);
            c *= Transform2D::Translate(-0.5, -0.5);
            c *= Transform2D::Rotate(bulletangle);
            c *= Transform2D::Translate(0.5, 0.5);
            float toprightx = c[2][0];
            float toprighty = c[2][1];
            bullets[i].setTopRight(toprightx, toprighty);


            // Top Left
            c = glm::mat3(1);
            c *= Transform2D::Translate(futureposx, futureposy + 1);
            c *= Transform2D::Translate(0.5, -0.5);
            c *= Transform2D::Rotate(bulletangle);
            c *= Transform2D::Translate(-0.5, 0.5);
            float topleftx = c[2][0];
            float toplefty = c[2][1];
            bullets[i].setTopLeft(topleftx, toplefty);


            // Coliziune proiectil-obstacole
            for (int j = 0; j < obsdims.size(); j++)
            {
                if (toprightx >= obscoords[j].x && toprightx <= obscoords[j].x + obsdims[j].x)
                    if (toprighty <= obscoords[j].y + obsdims[j].y && toprighty >= obscoords[j].y) {
                        bullets[i].setVis(false);
                        continue;
                    }

                if (topleftx >= obscoords[j].x && topleftx <= obscoords[j].x + obsdims[j].x)
                    if (toplefty <= obscoords[j].y + obsdims[j].y && toplefty >= obscoords[j].y) {
                        bullets[i].setVis(false);
                        continue;
                    }
            }

            // Coliziune proiectil-inamic
            // Daca un proiectil loveste un inamic, scorul si killing spree-ul cresc
            // Daca killing spree > 5 , scorul creste cu 2
            // Daca player-ul este lovit de un inamic, killing spree-ul este pierdut
            // Daca scorul este intre 50 si 100, se activeaza fury mode
            for (int j = 0; j < enemies.size(); j++)
            {
                glm::vec2 topleft = enemies[j].getTopLeft();
                float enemytopleftx = topleft.x;
                float enemytoplefty = topleft.y;
                glm::vec2 topright = enemies[j].getTopRight();
                float enemytoprightx = topright.x;
                float enemytoprighty = topright.y;
                glm::vec2 bottomleft = enemies[j].getBottomLeft();
                float enemybottomletftx = bottomleft.x;
                float enemybottomletfty = bottomleft.y;
                glm::vec2 bottomright = enemies[j].getBottomRight();
                float enemybottomrightx = bottomright.x;
                float enemybottomrighty = bottomright.y;

                //TOP LEFT
                if (topleftx <= enemytopleftx && toplefty <= enemytoplefty)
                {
                    if (topleftx >= enemybottomrightx && toplefty >= enemybottomrighty)
                    {
                        enemies[j].setVis(false);
                        bullets[i].setVis(false);
                        if (killingspree >= 5)
                            score += 2;
                        else
                            score++;
                        killingspree++;
                        cout << "SCORE = " << score << '\n';
                        cout << "KILLING SPREE = " << killingspree << '\n';
                        if (score == 50 || score == 51)
                        {
                            cout << "FURY MODE\n";
                            furymode = true;
                        }

                        if (score == 100 || score == 101)
                        {
                            cout << "FURY MODE OFF\n";
                            furymode = false;
                        }
                        continue;


                    }

                }

                if (topleftx <= enemytoprightx && toplefty <= enemytoprighty)
                {
                    if (topleftx >= enemybottomletftx && toplefty >= enemybottomletfty)
                    {
                        enemies[j].setVis(false);
                        bullets[i].setVis(false);
                        if (killingspree >= 5)
                            score += 2;
                        else
                            score++;
                        killingspree++;
                        cout << "SCORE = " << score << '\n';
                        cout << "KILLING SPREE = " << killingspree << '\n';
                        if (score == 50 || score == 51)
                        {
                            cout << "FURY MODE\n";
                            furymode = true;
                        }

                        if (score == 100 || score == 101)
                        {
                            cout << "FURY MODE OFF\n";
                            furymode = false;
                        }
                        continue;

                    }
                }

                if (topleftx >= enemytoprightx && toplefty >= enemytoprighty) 
                {
                    if (topleftx <= enemybottomletftx && toplefty <= enemybottomletfty)
                    {
                        enemies[j].setVis(false);
                        bullets[i].setVis(false);
                        if (killingspree >= 5)
                            score += 2;
                        else
                            score++;
                        killingspree++;
                        cout << "SCORE = " << score << '\n';
                        cout << "KILLING SPREE = " << killingspree << '\n';
                        if (score == 50 || score == 51)
                        {
                            cout << "FURY MODE\n";
                            furymode = true;
                        }

                        if (score == 100 || score == 101)
                        {
                            cout << "FURY MODE OFF\n";
                            furymode = false;
                        }
                        continue;
                    }
                }

                if (topleftx >= enemytopleftx && toplefty >= enemytoplefty)
                {
                    if (topleftx <= enemybottomrightx && toplefty <= enemybottomrighty)
                    {
                        enemies[j].setVis(false);
                        bullets[i].setVis(false);
                        if (killingspree >= 5)
                            score += 2;
                        else
                            score++;
                        killingspree++;
                        cout << "SCORE = " << score << '\n';
                        cout << "KILLING SPREE = " << killingspree << '\n';
                        if (score == 50 || score == 51)
                        {
                            cout << "FURY MODE\n";
                            furymode = true;
                        }

                        if (score == 100 || score == 101)
                        {
                            cout << "FURY MODE OFF\n";
                            furymode = false;
                        }
                        continue;
                    }
                }

                // TOP RIGHT
                if (toprightx <= enemytopleftx && toprighty <= enemytoplefty)
                {
                    if (toprightx >= enemybottomrightx && toprighty >= enemybottomrighty)
                    {
                        enemies[j].setVis(false);
                        bullets[i].setVis(false);
                        if (killingspree >= 5)
                            score += 2;
                        else
                            score++;
                        killingspree++;
                        cout << "SCORE = " << score << '\n';
                        cout << "KILLING SPREE = " << killingspree << '\n';
                        if (score == 50 || score == 51)
                        {
                            cout << "FURY MODE\n";
                            furymode = true;
                        }

                        if (score == 100 || score == 101)
                        {
                            cout << "FURY MODE OFF\n";
                            furymode = false;
                        }
                        continue;


                    }

                }

                if (toprightx <= enemytoprightx && toprighty <= enemytoprighty)
                {
                    if (toprightx >= enemybottomletftx && toprighty >= enemybottomletfty)
                    {
                        enemies[j].setVis(false);
                        bullets[i].setVis(false);
                        if (killingspree >= 5)
                            score += 2;
                        else
                            score++;
                        killingspree++;
                        cout << "SCORE = " << score << '\n';
                        cout << "KILLING SPREE = " << killingspree << '\n';
                        if (score == 50 || score == 51)
                        {
                            cout << "FURY MODE\n";
                            furymode = true;
                        }

                        if (score == 100 || score == 101)
                        {
                            cout << "FURY MODE OFF\n";
                            furymode = false;
                        }
                        continue;

                    }
                }

                if (toprightx >= enemytoprightx && toprighty >= enemytoprighty)
                {
                    if (toprightx <= enemybottomletftx && toprighty <= enemybottomletfty)
                    {
                        enemies[j].setVis(false);
                        bullets[i].setVis(false);
                        if (killingspree >= 5)
                            score += 2;
                        else
                            score++;
                        killingspree++;
                        cout << "SCORE = " << score << '\n';
                        cout << "KILLING SPREE = " << killingspree << '\n';
                        if (score == 50 || score == 51)
                        {
                            cout << "FURY MODE\n";
                            furymode = true;
                        }

                        if (score == 100 || score == 101)
                        {
                            cout << "FURY MODE OFF\n";
                            furymode = false;
                        }
                        continue;
                    }
                }

                if (toprightx >= enemytopleftx && toprighty >= enemytoplefty)
                {
                    if (toprightx <= enemybottomrightx && toprighty <= enemybottomrighty)
                    {
                        enemies[j].setVis(false);
                        bullets[i].setVis(false);
                        if (killingspree >= 5)
                            score += 2;
                        else
                            score++;
                        killingspree++;
                        cout << "SCORE = " << score << '\n';
                        cout << "KILLING SPREE = " << killingspree << '\n';
                        if (score == 50 || score == 51)
                        {
                            cout << "FURY MODE\n";
                            furymode = true;
                        }

                        if (score == 100 || score == 101)
                        {
                            cout << "FURY MODE OFF\n";
                            furymode = false;
                        }
                        continue;
                    }
                }

            }
            
            // Coliziune proiectil-margine harta
            if (futureposy >= MAP_HEIGHT || futureposy - 1.0f <= 0 || futureposx >= MAP_HEIGHT || futureposx - 1 <= 0)
            {
                bullets[i].setVis(false);
                continue;
            }
            else
            {
                bullets[i].setX(futureposx);
                bullets[i].setY(futureposy);

                modelMatrix = glm::mat3(1);
                modelMatrix *= visMatrix;
                modelMatrix *= Transform2D::Translate(futureposx, futureposy);
                modelMatrix *= Transform2D::Translate(0.5, 0.5);
                modelMatrix *= Transform2D::Rotate(bulletangle);
                modelMatrix *= Transform2D::Translate(-0.5, -0.5);
                RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

            }

        }
    }

    // Potiunea de viata apare dupa 5 secunde dupa ce a fost luata
    if ((float(clock() - potionclock) / CLOCKS_PER_SEC) > 5) {
        potionvis = true;
    }

    potionCol(playertranslateX, playertranslateY);

    if (potionvis) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= Transform2D::Translate(30, 10);
        RenderMesh2D(meshes["ver"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= Transform2D::Translate(-0.75, 0.75);
        RenderMesh2D(meshes["hor"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= Transform2D::Translate(0, -0.75);
        RenderMesh2D(meshes["potion"], shaders["VertexColor"], modelMatrix);
    }

    fireratePotionCol(playertranslateX, playertranslateY);

    // Potiunea de fire rate apare dupa 10 secunde dupa ce a fost luata
    if ((float(clock() - fireratepotionclock) / CLOCKS_PER_SEC) > 10) {
        fireratepotionvis = true;
    }

    if (fireratepotionvis)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= Transform2D::Translate(25, 35);
        RenderMesh2D(meshes["fireratepotion"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= Transform2D::Translate(-0.2, -0.2);
        RenderMesh2D(meshes["potionout"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= Transform2D::Translate(posx + playertranslateX + 20, posy + playertranslateY + 15);
    modelMatrix *= Transform2D::Scale(0.2f*healtbarscale,1);
    RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= Transform2D::Translate(posx + playertranslateX + 19.8, posy + playertranslateY + 14.8);
    RenderMesh2D(meshes["healthbarout"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["obstacle3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["obstacle4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["obstacle5"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["obstacle6"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["map1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderMesh2D(meshes["map2"], shaders["VertexColor"], modelMatrix);
    

}




void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Miscare jucator
    float posx = player->getBodyCoord().x;
    float posy = player->getBodyCoord().y;
    if (window->KeyHold(GLFW_KEY_W) && playertranslateY + deltaTime*6.0f <= MAP_HEIGHT - posy && 
        checkObstacleCol(playertranslateX,playertranslateY + deltaTime * 6.0f)!=-1) {
        playertranslateY += deltaTime * 6.0f;
        logicSpace.y += deltaTime * 6.0f;
    }
    if (window->KeyHold(GLFW_KEY_S) && playertranslateY - deltaTime * 6.0f >= 2 - posy
        && checkObstacleCol(playertranslateX, playertranslateY - deltaTime * 6.0f) != -1) {

        playertranslateY -= deltaTime * 6.0f;
        logicSpace.y -= deltaTime * 6.0f;
    }

    if (window->KeyHold(GLFW_KEY_A) && playertranslateX - deltaTime*6.0f >= 2- posx
        && checkObstacleCol(playertranslateX - deltaTime*6.0f, playertranslateY) != -1) {
        playertranslateX -= deltaTime * 6.0f;
        logicSpace.x -= deltaTime * 6.0f;
    }
    if (window->KeyHold(GLFW_KEY_D) && playertranslateX + deltaTime*6.0f <= MAP_HEIGHT - posx
        && checkObstacleCol(playertranslateX + deltaTime*6.0f, playertranslateY) != -1) {
        playertranslateX += deltaTime * 6.0f;
        logicSpace.x += deltaTime * 6.0f;
    }

}

// Coliziune jucator-margine harta
int Tema1::checkObstacleCol(float translateX, float translateY)
{
    float posx = player->getBodyCoord().x;
    float posy = player->getBodyCoord().y;
    float bodyRadius = player->getBodyRadius();
    for (int i = 0; i < obscoords.size(); i++)
    {
        float testX = posx + translateX;
        float testY = posy + translateY;

        if (posx + translateX < obscoords[i].x)
            testX = obscoords[i].x;
        else
            if (posx + translateX > obscoords[i].x + obsdims[i].x)
                testX = obscoords[i].x + obsdims[i].x;
        if (posy + translateY< obscoords[i].y)
            testY = obscoords[i].y;
        else
            if(posy + translateY > obscoords[i].y + obsdims[i].y)
                testY = obscoords[i].y + obsdims[i].y;

        float distX = posx + translateX - testX;
        float distY = posy + translateY - testY;
        float dist = sqrt((distX * distX) + (distY * distY));
        if (dist <= bodyRadius)
            return -1;
    }
    return 1;
}

// Coliziune jucator-potiune de viata
void Tema1::potionCol(float translateX, float translateY)
{
    float posx = player->getBodyCoord().x;
    float posy = player->getBodyCoord().y;
    float bodyRadius = player->getBodyRadius();

    float testX = posx + translateX;
    float testY = posy + translateY;

    if (posx + translateX < 29.25f)
        testX = 29.25;
    else
        if (posx + translateX > 29.25f + 2.0f)
            testX = 29.25 + 2;
    if (posy + translateY < 10)
        testY = 10;
    else
        if (posy + translateY > 10 + 2)
            testY = 10 + 2;

    float distX = posx + translateX - testX;
    float distY = posy + translateY - testY;
    float dist = sqrt((distX * distX) + (distY * distY));
    if (dist <= bodyRadius)
    {
        if (healtbarscale != 5 && potionvis)
        {
            healtbarscale++;
            potionvis = false;
            potionclock = clock();
        }
        else
        {
            if(potionvis)
            cout << "HEALTH FULL\n";
        }
    }
}

// Coliziune jucator-potiunde de fire rate
void Tema1::fireratePotionCol(float translateX, float translateY)
{
    float posx = player->getBodyCoord().x;
    float posy = player->getBodyCoord().y;
    float bodyRadius = player->getBodyRadius();

    float testX = posx + translateX;
    float testY = posy + translateY;

    if (posx + translateX < 24.8f)
        testX = 24.8;
    else
        if (posx + translateX > 24.8f + 2.0f)
            testX = 24.8 + 2;
    if (posy + translateY < 34.8f)
        testY = 34.8;
    else
        if (posy + translateY > 34.8f + 2.0f)
            testY = 34.8 + 2;

    float distX = posx + translateX - testX;
    float distY = posy + translateY - testY;
    float dist = sqrt((distX * distX) + (distY * distY));
    if (dist <= bodyRadius)
    {
        firerateinc = true;
        firerateincclock = clock();
        fireratepotionvis = false;
        fireratepotionclock = clock();

    }
}

// Colizune jucator-inamic
void Tema1::EnemyCol(float translateX, float translateY)
{
    float posx = player->getBodyCoord().x;
    float posy = player->getBodyCoord().y;
    float bodyRadius = player->getBodyRadius();

    for (int i = 0; i < enemies.size(); i++)
    {
        float enemyx = enemies[i].getX();
        float enemyy = enemies[i].getY();
        float testX = posx + translateX;
        float testY = posy + translateY;

        if (posx + translateX < enemyx)
            testX = enemyx;
        else
            if (posx + translateX > enemyx + 3)
                testX = enemyx + 3;
        if (posy + translateY < enemyy)
            testY = enemyy;
        else
            if (posy + translateY > enemyy + 3)
                testY = enemyy + 3;

        float distX = posx + translateX - testX;
        float distY = posy + translateY - testY;
        float dist = sqrt((distX * distX) + (distY * distY));
        if (dist <= bodyRadius)
        {
            enemies[i].setVis(false);
            healtbarscale --;
            killingspree = 0;
            cout << "KILLING SPREE LOST\n";
            if (healtbarscale == 0)
            {
                cout << "GAME OVER!!!\n";
                exit(0);
            }
        }
    }
}



void Tema1::RenderEnemy(Enemy enemy)
{
    float x = enemy.getX();
    float y = enemy.getY();
    float angle = enemy.getAngle();

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= Transform2D::Translate(x  - 0.6, y + 2.4);

    modelMatrix *= Transform2D::Translate(+2.1, -0.9);
    modelMatrix *= Transform2D::Rotate(angle);
    modelMatrix *= Transform2D::Translate(-2.1, +0.9);

    RenderMesh2D(meshes["enemyhand"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= Transform2D::Translate(-0.2, - 0.2);
    RenderMesh2D(meshes["enemyhandout"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= Transform2D::Translate(3.2f,  0.2f);
    RenderMesh2D(meshes["enemyhand"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= Transform2D::Translate(-0.2f,  -0.2f);
    RenderMesh2D(meshes["enemyhandout"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= Transform2D::Translate(-1.95, -1.95);
    RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= Transform2D::Translate(-0.25, -0.25);
    RenderMesh2D(meshes["enemyout"], shaders["VertexColor"], modelMatrix);



}

void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 resolution = window->GetResolution();
    float posx = mouseX - player->getBodyCoord().x/logicSpace.width*resolution.x;
    float posy = mouseY - player->getBodyCoord().y/logicSpace.height*resolution.y;
    angularStep = atan2(-posx,-posy);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (firerateinc && (float(clock() - firerateincclock) / CLOCKS_PER_SEC) <= 5)
    {
        shoot = true;
        bulletAngle = angularStep;
        bulletclock = clock();
        bulletx = player->getBodyCoord().x + playertranslateX;
        bullety = player->getBodyCoord().y + playertranslateY;
        Bullet* bullet = new Bullet(bulletx, bullety, bulletAngle);
        bullet->setVis(true);
        bullet->setTtl(bulletclock);
        bullets.push_back(*bullet);
    }
    else {
        if ((float(clock() - bulletclock) / CLOCKS_PER_SEC) > 1) {
            bulletclock = clock();
            shoot = true;
            bulletAngle = angularStep;
            bulletx = player->getBodyCoord().x + playertranslateX;
            bullety = player->getBodyCoord().y + playertranslateY;
            Bullet* bullet = new Bullet(bulletx, bullety, bulletAngle);
            bullet->setVis(true);
            bullet->setTtl(bulletclock);
            bullets.push_back(*bullet);

        }
    }
    
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
