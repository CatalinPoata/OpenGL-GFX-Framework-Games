#include "objects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = glm::vec3(-length/2, -length/2, 0);

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

Mesh* object2D::CreateCircle(
    const std::string& name,
    int noOfVertices,
    float radius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    float angle = 0.0f;

    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
    for (int i = 0; i < noOfVertices; i++) {
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(angle), radius * sin(angle), 0), color));
        angle += 2 * M_PI / ((float) noOfVertices);
    }

    vertices[0].color = glm::vec3(1, 1, 1);
    vertices[1].color = glm::vec3(1, 1, 1);

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 0; i < noOfVertices; i++) {
        indices.push_back(i + 1);
    }

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(1);
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateBirdBody(
    const std::string& name,
    int noOfVertices,
    float radius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    float angle = -1 * M_PI_2;

    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
    for (int i = 0; i < noOfVertices; i++) {
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(angle), radius * sin(angle), 0), color));
        angle += M_PI / ((float)noOfVertices);
    }
    vertices.push_back(VertexFormat(glm::vec3(-1 * radius, 0, 0), color));


    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 0; i < noOfVertices; i++) {
        indices.push_back(i + 1);
    }
    indices.push_back(noOfVertices + 1);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(1);
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}
