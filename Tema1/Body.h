#pragma once
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema1/objects2D.h"
#include <stdlib.h>
#include "components/simple_scene.h"

class Body
{
public:

	Body();
	Body(float birdScale, float theta);
	~Body();

	Mesh* createBodyMesh(const std::string& name, glm::vec3 color);
	glm::mat3 getTransform();
	int hasHitEdge(int width, int height);
	void updateVertices();
	bool isMouseOnBody(int mouseX, int mouseY);

	std::vector<std::pair<float, float>> verticesPos;
	glm::mat3 selfTransform;
	glm::mat3 moveTransform;
	glm::mat3 rotateTransform;
};