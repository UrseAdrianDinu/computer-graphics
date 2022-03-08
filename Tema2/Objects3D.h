#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "components/simple_scene.h"
#include "core/engine.h"


namespace Objects3D
{
	Mesh* CreateCube(const char* name, float length, glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes);
	Mesh* CreatePara(const char* name, float length, float width, float height, glm::vec3 color, std::unordered_map<std::string, Mesh*>& meshes);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill);
    Mesh* CreateSquare(
        const char* name,
        glm::vec3 leftBottomCorner,
        float length,
        std::unordered_map<std::string, Mesh*>& meshes);

    Mesh* CreateMesh(const char* name,
        const std::vector<VertexFormat>& vertices,
        const std::vector<unsigned int>& indices,
        std::unordered_map<std::string, Mesh*>& meshes);
}