#pragma once
#pragma once

#include "components/simple_scene.h"
#include "camera.h"
#include <stack>
#include "Human.h"
#include "Cell.h"
#include "Foe.h"
#include "Arrow.h"
#include "Transf2D.h"


class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void LoadTextures();
        void LoadShaders();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture1, Texture2D* texture2);
        void RenderPlayer();
        void RenderEnvironment();
        void RenderFoes();
        void RenderMaze();
        void InitMaze();
        void GenerateMaze();
        std::vector<Cell*> getAdj(Cell* cell);
        Cell* getPlayerCell();
        bool Tema2::checkColWall(float x, float z);

    protected:
        
        Camera *camera;
        float fov;
        float Z_FAR = 200.0f;
        float Z_NEAR = 0.01f;
        glm::mat4 projectionMatrix;
        bool thirdperson = true;


        glm::mat4 modelMatrix;
        std::unordered_map<std::string, Texture2D*> mapTextures;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        Human* human;
        float playerhealth = 5;
        float timescale = 50;
        float playertime;

        std::vector<Cell*> maze;
        std::stack<Cell*> stack;
        glm::vec3 startpos;
        glm::vec3 endpos;

        std::vector<Foe*> foes;
        int noEnemies;

        std::vector<Arrow*> arrows;
        float arrow_clock = 0;

        glm::vec3 tntpos;
        float tnt_enabled;
        bool walls_down = false;

        float portalangle;

        glm::vec3 pigpos;
        bool pigvis = true;
    };
