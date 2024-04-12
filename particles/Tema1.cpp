#include "lab_m2/Tema1/Tema1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m2;

static float angle = 0;


struct Particle
{
    glm::vec4 position;
    glm::vec4 speed;
    glm::vec4 initialPos;
    glm::vec4 initialSpeed;
    glm::vec4 color;
    glm::vec4 direction;
    glm::vec4 angle;
    glm::vec4 rotDirection;

    Particle() {}

    Particle(const glm::vec4& pos, const glm::vec4& speed, const glm::vec4& col, 
        const glm::vec4& dir, const glm::vec4& angle, const glm::vec4& rotDirection)
    {
        SetInitial(pos, speed, col, dir, angle, rotDirection);
    }

    void SetInitial(const glm::vec4& pos, const glm::vec4& speed, const glm::vec4& col, const glm::vec4& dir,
        const glm::vec4& angle, const glm::vec4& rotDirection)
    {
        position = pos;
        initialPos = pos;

        this->speed = speed;
        initialSpeed = speed;
        color = col;
        direction = dir;
        this->angle = angle;
        this->rotDirection = rotDirection;
    }
};

ParticleEffect<Particle>* particlEffect;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    projInfo = camera->GetProjectionInfo();
    camera->SetOrthographic(20, 20, 0.2, 20);
    camera->SetPositionAndRotation(glm::vec3(10, 10, 20), glm::quat(glm::vec3(0, 0, 0)));
    view0 = camera->GetViewMatrix();

    camera->SetPositionAndRotation(glm::vec3(0, 10, 10), glm::quat(glm::vec3(0, RADIANS(-90), 0)));
    view1 = camera->GetViewMatrix();

    camera->SetPositionAndRotation(glm::vec3(10, 10, 0), glm::quat(glm::vec3(0, RADIANS(180), 0)));
    view2 = camera->GetViewMatrix();

    camera->SetPositionAndRotation(glm::vec3(20, 10, 10), glm::quat(glm::vec3(0, RADIANS(90), 0)));
    view3 = camera->GetViewMatrix();

    camera->SetPositionAndRotation(glm::vec3(10, 20, 10), glm::quat(glm::vec3(RADIANS(-90), 0, 0)));
    view4 = camera->GetViewMatrix();

    camera->SetPositionAndRotation(glm::vec3(10, 0, 10), glm::quat(glm::vec3(RADIANS(90), 0, 0)));
    view5 = camera->GetViewMatrix();

    camera->SetPositionAndRotation(glm::vec3(10, 10, 10), glm::quat(glm::vec3(0, 0, 0)));
    camera->SetProjection(projInfo);
    camera->Update();

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "particle2.png");
    }

    LoadShader("Particle");

    unsigned int nrParticles = 10;
    particlEffect = new ParticleEffect<Particle>();
    particlEffect->Generate(nrParticles, true);

    auto particleSSBO = particlEffect->GetParticleBuffer();
    Particle* data = const_cast<Particle*>(particleSSBO->GetBuffer());

    int cubeSize = 20;
    int hSize = cubeSize / 2;

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        glm::vec4 pos(1);
        pos.x = 5 + rand() % 11;
        pos.y = 5 + rand() % 11;
        pos.z = 5 + rand() % 11;

        glm::vec4 speed(0);
        speed.x = rand() % 100 - 50;
        speed.z = rand() % 100 - 50;
        speed.y = rand() % 100 - 50;


        glm::vec4 dir(0);
        dir.x = rand() % 6 - 3;
        dir.y = rand() % 6 - 3;
        dir.z = rand() % 6 - 3;
        

        glm::vec4 pcolor = glm::vec4(NormalizedRGB((rand() % 255), (rand() % 255), (rand() % 255)), 1);

        glm::vec4 angle = glm::vec4((rand() % 360), (rand() % 360), (rand() % 360), 1);

        int rotDir[2] = {-1, 1};
        glm::vec4 rotDirection = glm::vec4(rotDir[rand() % 2]);
        data[i].SetInitial(pos, glm::vec4(0), pcolor, dir, angle, rotDirection);
    }

    particleSSBO->SetBufferData(data);


    // Create a shader program for rendering to texture
    {
        Shader* shader = new Shader("ShaderTema1");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    auto resolution = window->GetResolution();

    // TODO(student): Create a new framebuffer and generate attached textures
    frameBuffer = new FrameBuffer();
    frameBuffer->Generate(resolution.x, resolution.y, 6, false);

    no_of_generated_points = 10;            // number of points on a Bezier curve
    no_of_instances = 50;                   // number of instances (number of curves that contain the surface)
    max_translate = 8.0f;                   // for the translation surface, it's the distance between the first and the last curve
    max_rotate = glm::radians(360.0f);      // for the rotation surface, it's the angle between the first and the last curve

    control_p0 = glm::vec3(-4.0, -2.5, 1.0);
    control_p1 = glm::vec3(-2.5, 1.5, 1.0);
    control_p2 = glm::vec3(-1.5, 3.0, 1.0);
    control_p3 = glm::vec3(-4.0, 5.5, 1.0);


    {
        vector<VertexFormat> vertices
        {
            VertexFormat(control_p0, glm::vec3(0, 1, 1)),
            VertexFormat(control_p3, glm::vec3(0, 1, 0))
        };

        vector<unsigned int> indices =
        {
            0, 1
        };

        meshes["surface"] = new Mesh("generated initial surface points");
        meshes["surface"]->InitFromData(vertices, indices);
        meshes["surface"]->SetDrawMode(GL_LINES);
    }
}




void Tema1::FrameStart()
{
}


void Tema1::Update(float deltaTimeSeconds)
{

    static glm::vec3 mirrorPos(10, 10, 10);
    static glm::vec3 mirrorRotation = glm::vec3(0, RADIANS(180), 0);

    ClearScreen();


    // Save camera position and rotation
    auto camera = GetSceneCamera();


    // TODO(student): Render scene view from a mirrorred point of view. Use
    // `camera->SetPosition()` and `camera->SetRotation(glm::quat(euler_angles))`
    {
        frameBuffer->Bind();
        auto shader = shaders["Particle"];
        glLineWidth(3);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);
        if (shader->GetProgramID())
        {
            shader->Use();
            mirrorred = 1;
            particlEffect->Render(GetSceneCamera(), shader, 
                speed_mod, 
                stop, 
                reset,
                view0,
                view1,
                view2,
                view3,
                view4,
                view5,
                mirrorred);
            if (reset == 1)
                reset = 0;
        }
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        frameBuffer->BindDefault();
    }

    // Render the scene normally
    {
        camera->SetProjection(projInfo);
        auto shader = shaders["Particle"];
        if (shader->GetProgramID())
        {
            shader->Use();
            mirrorred = 0;
            particlEffect->Render(GetSceneCamera(),
                shader,
                speed_mod,
                stop,
                reset,
                view0,
                view1,
                view2,
                view3,
                view4,
                view5,
                mirrorred);
            if (reset == 1)
                reset = 0;
        }
    }

    // Render the mirrored scene
    {
        //frameBuffer->BindAllTextures();
        glDisable(GL_BLEND);
        auto shader = shaders["ShaderTema1"];

        shader->Use();

        frameBuffer->BindTexture(0, GL_TEXTURE0);
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(10,10,0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10));
        RenderMesh(meshes["quad"], shader, modelMatrix);

        frameBuffer->BindTexture(1, GL_TEXTURE0);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 10, 10));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(180.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10));
        RenderMesh(meshes["quad"], shader, modelMatrix);


        frameBuffer->BindTexture(2, GL_TEXTURE0);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 10, 20));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(180.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10));
        RenderMesh(meshes["quad"], shader, modelMatrix);


        frameBuffer->BindTexture(3, GL_TEXTURE0);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 10, 10));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10));
        RenderMesh(meshes["quad"], shader, modelMatrix);


        frameBuffer->BindTexture(4, GL_TEXTURE0);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 0, 10));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(180.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10));
        RenderMesh(meshes["quad"], shader, modelMatrix);

        frameBuffer->BindTexture(5, GL_TEXTURE0);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 20, 10));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10));
        RenderMesh(meshes["quad"], shader, modelMatrix);
    }

}


void Tema1::DrawScene()
{
    for (int i = 0; i < 16; i++)
    {
        float rotateAngle = (angle + i) * ((i % 2) * 2 - 1);
        glm::vec3 position = glm::vec3(-4 + (i % 4) * 2.5, 0, -2 + (i / 4) * 2.5);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position);
        modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));

        RenderMesh(meshes["bamboo"], shaders["ShaderTema1"], modelMatrix);
    }
}


void Tema1::FrameEnd()
{
    DrawCoordinateSystem();
}

void Tema1::LoadShader(const std::string& name, bool hasGeomtery)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders");

    // Create a shader program for particle system
    {
        Shader* shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);
        if (hasGeomtery)
        {
            shader->AddShader(PATH_JOIN(shaderPath, name + ".GS.glsl"), GL_GEOMETRY_SHADER);
        }

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void Tema1::RenderMeshInstanced(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int instances, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;


    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), no_of_instances);


    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), no_of_generated_points);
    glUniform1f(glGetUniformLocation(shader->program, "max_translate"), max_translate);
    glUniform1f(glGetUniformLocation(shader->program, "max_rotate"), max_rotate);


    // Render an object using the specified shader
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, instances);
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event

    if (key == GLFW_KEY_EQUAL)
    {
        speed_mod+=1;
    }

    if (key == GLFW_KEY_MINUS)
    {
        speed_mod-=1;
    }

    if (key == GLFW_KEY_Z)
    {
        if (stop == 1)
            stop = 0;
        else
            stop = 1;
    }

    if (key == GLFW_KEY_X)
    {
        if (reset == 1)
            reset = 0;
        else
            reset = 1;
    }

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}