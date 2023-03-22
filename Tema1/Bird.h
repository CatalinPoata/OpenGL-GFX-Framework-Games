#pragma once
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema1/objects2D.h"
#include <stdlib.h>
#include "components/simple_scene.h"
#include "lab_m1/Tema1/Wing.h"
#include "lab_m1/Tema1/Body.h"
#include "lab_m1/Tema1/Head.h"

class Bird
{
public:

	Bird();
	Bird(float speed, float scale);
	~Bird();

	void move(float deltaTimeSeconds);
	void rotateBird(int c);
	int hasHitEdge(int width, int height);
	void updateVertices();
	bool isMouseOnBird(int mouseX, int mouseY);
	
	int status;
	float scale;
	float speed;
	float theta;
	Wing wing1;
	Wing wing2;
	Head head;
	Body body;
	std::pair<float, float> position;
	std::vector<Mesh*> meshes;
	int collisions;
};