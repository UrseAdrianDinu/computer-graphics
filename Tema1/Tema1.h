#pragma once

#include "components/simple_scene.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <string>
#include <Core/Engine.h>
#include <vector>


class Tema1 : public gfxc::SimpleScene
{
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        int checkBorder(float translateX, float translateY, float deltaTime);
        int checkObstacleCol(float translateX, float translateY);
        void fireratePotionCol(float translateX, float translateY);
        void EnemyCol(float translateX, float translateY);
        void potionCol(float translateX, float translateY);
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderEnemy(Enemy enemy);

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
     
protected:
        Player* player;
        glm::mat3 modelMatrix, visMatrix;
        float playertranslateX, playertranslateY;
        float angularStep;

        float MAP_HEIGHT, MAP_WIDTH;
        std::vector<glm::vec3> obscoords;
        std::vector<glm::vec3> obsdims;
        
        std::vector<Bullet> bullets;
        std::vector<int> deleteBullets;

        float bulletmove = 1;
        float bulletAngle;
        float bulletx = 0;
        float bullety = 0;
        float bulletclock = 0;

        bool shoot = false;
        bool furymode = false;
        int noEnemies = 0;
        std::vector<Enemy> enemies;
        std::vector<int> deleteEnemies;

        float score = 0;
        float healtbarscale = 5;

        float killingspree = 0;

        bool potionvis = true;
        float potionclock;

        bool fireratepotionvis = true;
        float fireratepotionclock;
        bool firerateinc = false;
        float firerateincclock;

        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        
};

