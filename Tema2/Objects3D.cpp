#include "Objects3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* Objects3D::CreateCube(const char* name, float length, glm::vec3 color, std::unordered_map<std::string, Mesh*> &meshes)
{

	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(length, 0, 0), color),
		VertexFormat(glm::vec3(length, length, 0), color),
		VertexFormat(glm::vec3(0, length, 0), color),
		VertexFormat(glm::vec3(0, 0, length), color),
		VertexFormat(glm::vec3(length, 0, length), color),
		VertexFormat(glm::vec3(length, length, length), color),
		VertexFormat(glm::vec3(0, length, length), color)
	};

    vector<unsigned int> indices =
    {
			0, 5, 1,	// indices for first triangle
			0, 4, 5,	// indices for second triangle
			0, 7, 4,	// etc.
			0, 3, 7,
			3, 6, 7,
			3, 2, 6,
			2, 5, 6,
			2, 1, 5,
			0, 1, 2,
			0, 2, 3,
			4, 6, 5,
			4, 7, 6
    };

    Mesh* cube = CreateMesh(name, vertices, indices, meshes);
    return cube;
}

Mesh* Objects3D::CreatePara(const char* name, float length, float width, float height,
    glm::vec3 color, std::unordered_map<std::string, Mesh*> &meshes)
{

	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0, width), color),
		VertexFormat(glm::vec3(length, 0, width), color),
		VertexFormat(glm::vec3(0, height, width), color),
		VertexFormat(glm::vec3(length, height, width), color),
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(length, 0, 0), color),
		VertexFormat(glm::vec3(0, height, 0), color),
		VertexFormat(glm::vec3(length, height, 0), color)
	};

	vector<unsigned int> indices =
	{
			0, 1, 2,    
			1, 3, 2,
			2, 3, 7,
			2, 7, 6,
			1, 7, 3,
			1, 5, 7,
			6, 7, 4,
			7, 5, 4,
			0, 4, 1,
			1, 4, 5,
			2, 6, 4,
			0, 2, 4,
	};

    Mesh* para = CreateMesh(name, vertices, indices, meshes);
	return para;
}

Mesh* Objects3D::CreateRectangle(
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


Mesh* Objects3D::CreateSquare(
    const char* name,
    glm::vec3 leftBottomCorner,
    float length,
    std::unordered_map<std::string, Mesh*>& meshes)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, glm::vec3(0), glm::vec3(0, 1, 1), glm::vec2(0, 0)),
        VertexFormat(corner + glm::vec3(length, 0, 0), glm::vec3(0), glm::vec3(1, 0, 1), glm::vec2(1, 0)),
        VertexFormat(corner + glm::vec3(length, length, 0), glm::vec3(0), glm::vec3(1,0,0),glm::vec2(1, 1)),
        VertexFormat(corner + glm::vec3(0, length, 0), glm::vec3(0), glm::vec3(0,1,0), glm::vec2(0, 1)),
    };


    std::vector<unsigned int> indices = { 0, 1, 2, 3 , 0 , 2};

    
    Mesh* square = CreateMesh(name,vertices,indices,meshes);
    return square;
}


Mesh* Objects3D::CreateMesh(const char* name,
    const std::vector<VertexFormat>& vertices, 
    const std::vector<unsigned int>& indices,
    std::unordered_map<std::string, Mesh*>& meshes)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);


    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}
