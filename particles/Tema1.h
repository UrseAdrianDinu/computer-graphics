#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "core/gpu/frame_buffer.h"
#include "core/gpu/particle_effect.h"


namespace m2
{
	class Tema1 : public gfxc::SimpleScene
	{
		public:
			Tema1();
			~Tema1();

			void Init() override;
        private:
            void FrameStart() override;
            void Update(float deltaTimeSeconds) override;
            void FrameEnd() override;

            void DrawScene();
            void LoadShader(const std::string& name, bool hasGeomtery = true);
            void RenderMeshInstanced(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int instances, const glm::vec3& color = glm::vec3(1));


            void OnInputUpdate(float deltaTime, int mods) override;
            void OnKeyPress(int key, int mods) override;
            void OnKeyRelease(int key, int mods) override;
            void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
            void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
            void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
            void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
            void OnWindowResize(int width, int height) override;

            FrameBuffer* frameBuffer;

            glm::vec3 control_p0, control_p1, control_p2, control_p3;
            unsigned int no_of_generated_points, no_of_instances;
            int speed_mod = 0, stop = 0, reset = 0;
            float max_translate, max_rotate;
            glm::mat4 view0, view1, view2, view3, view4, view5;
            int mirrorred = 1;
            gfxc::ProjectionInfo projInfo;
	};
}