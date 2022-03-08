#include "Objects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* Objects2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* Objects2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* Objects2D::CreateCircle(
    const std::string& name,
    glm::vec3 center, 
    float radius, 
    glm::vec3 color,
    bool fill)
{
    Mesh* circle = new Mesh(name);
    vector<VertexFormat> vertices_circle;
    vector<unsigned int> indices_circle;
    GLfloat arg;
    vertices_circle.emplace_back(glm::vec3(center.x, center.y, 0), color);
        for (unsigned int i = 0; i <= 25; i++) {
            arg = 2.0f * 3.14159f * i / 25;
            vertices_circle.emplace_back(glm::vec3(center.x + cos(arg) * radius, center.y + sin(arg) * radius, 0), color);
            indices_circle.push_back(i);
        }
        indices_circle.push_back(1);
        circle->InitFromData(vertices_circle, indices_circle);
        if (fill)
            circle->SetDrawMode(GL_TRIANGLE_FAN);
        else
            circle->SetDrawMode(GL_LINE_LOOP);
        return circle;
}