#pragma once
#include "lab_m1/Tema2/PlayerCar.h"

PlayerCar::PlayerCar() {
	this->moveSpeed = 0;
	this->theta = 0;
	this->rotateSpeed = 0;
	this->forward = glm::vec3(0, 0, 0);
	this->rotationsMatrix = glm::mat4(1);
	this->translationsMatrix = glm::mat4(1);
	this->center = glm::vec3(0, 0, 0);
	this->initialPos = glm::vec3(0, 0, 0);
	this->initialTheta = 0;
}

PlayerCar::PlayerCar(glm::vec3 initialPos, float initialTheta, float moveSpeed, float rotateSpeed) {
	this->moveSpeed = moveSpeed;
	this->theta = initialTheta;
	this->rotateSpeed = rotateSpeed;
	this->forward = glm::vec3(glm::rotate(glm::mat4(1), this->theta, glm::vec3(0, 1, 0)) * glm::vec4(glm::vec3(0, 0, -1), 1));
	this->translationsMatrix = glm::mat4(1);
	this->translationsMatrix = glm::translate(this->translationsMatrix, initialPos);
	this->rotationsMatrix = glm::mat4(1);
	this->rotationsMatrix = glm::rotate(this->rotationsMatrix, this->theta, glm::vec3(0, 1, 0));
	this->center = initialPos;
	this->initialPos = initialPos;
	this->initialTheta = initialTheta;
}


PlayerCar::~PlayerCar() {
}

void PlayerCar::moveForward(float deltaTimeSeconds, int way) {
	glm::mat4 tempTrans = glm::translate(glm::mat4(1), this->moveSpeed * deltaTimeSeconds * way * this->forward);
	this->translationsMatrix *= tempTrans;
	this->center = glm::vec3(tempTrans * glm::vec4(this->center, 1));
}

void PlayerCar::rotate(float deltaTimeSeconds, int way) {
	glm::mat4 tempRot = glm::rotate(glm::mat4(1), deltaTimeSeconds * way * this->rotateSpeed, glm::vec3(0, 1, 0));
	this->theta += deltaTimeSeconds * way * this->rotateSpeed;
	this->rotationsMatrix *= tempRot;
	this->forward = glm::vec3(tempRot * glm::vec4(this->forward, 1));
}

glm::mat4 PlayerCar::getTotalTransformation() {
	return this->translationsMatrix * this->rotationsMatrix * glm::scale(glm::mat4(1), glm::vec3(1, 1, 2));
}

void PlayerCar::reset() {
	this->theta = 0;
	this->forward = glm::vec3(glm::rotate(glm::mat4(1), this->initialTheta, glm::vec3(0, 1, 0)) * glm::vec4(glm::vec3(0, 0, -1), 1));
	this->rotationsMatrix = glm::rotate(glm::mat4(1), this->initialTheta, glm::vec3(0, 1, 0));
	this->translationsMatrix = glm::translate(glm::mat4(1), this->initialPos);
	this->center = this->initialPos;

}

bool PlayerCar::isOnTrack(std::vector<glm::vec3> trackPoints, float deltaTimeSeconds) {
	glm::mat4 tempTrans = glm::translate(glm::mat4(1), this->moveSpeed * deltaTimeSeconds * this->forward);
	glm::vec3 nextPoint = glm::vec3(tempTrans * glm::vec4(this->center, 1));
	for (int k = 0; k < trackPoints.size() - 3; k += 2) {
		int intersections = 0;
		std::vector<glm::vec3> currPoly = { trackPoints[k], trackPoints[k + 1], trackPoints[k + 3], trackPoints[k+2] };
		for (int i = 0, j = currPoly.size() - 1; i < currPoly.size(); j = i++) {
			if (((currPoly[i].z > nextPoint.z) != (currPoly[j].z > nextPoint.z)) && (nextPoint.x < (currPoly[j].x - currPoly[i].x) * (nextPoint.z - currPoly[i].z) / (currPoly[j].z - currPoly[i].z) + currPoly[i].x)) {
				intersections += 1;
			}
		}

		if (intersections % 2 != 0) {
			return true;
		}
	}

	return false;
}

bool PlayerCar::hasHitCar(std::vector<BotCar> botCars) {
	for (auto car : botCars) {
		if (glm::distance(center, car.center) < 2.5) {
			return true;
		}
	}
	return false;
}

Mesh* PlayerCar::createCarMesh(const std::string& name, glm::vec3 color) {
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-0.5, 0,  0.5), color, glm::vec3(0.2, 0.8, 0.2)),
		VertexFormat(glm::vec3(0.5, 0,  0.5), color, glm::vec3(0.9, 0.4, 0.2)),
		VertexFormat(glm::vec3(-0.5,  1,  0.5), color, glm::vec3(0.7, 0.7, 0.1)),
		VertexFormat(glm::vec3(0.5,  1,  0.5), color, glm::vec3(0.7, 0.3, 0.7)),
		VertexFormat(glm::vec3(-0.5, 0, -0.5), color, glm::vec3(0.3, 0.5, 0.4)),
		VertexFormat(glm::vec3(0.5, 0, -0.5), color, glm::vec3(0.5, 0.2, 0.9)),
		VertexFormat(glm::vec3(-0.5,  1, -0.5), color, glm::vec3(0.7, 0.0, 0.7)),
		VertexFormat(glm::vec3(0.5,  1, -0.5), color, glm::vec3(0.1, 0.5, 0.8)),
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 2,        1, 3, 2,
		2, 3, 7,        2, 7, 6,
		1, 7, 3,        1, 5, 7,
		6, 7, 4,        7, 5, 4,
		0, 4, 1,        1, 4, 5,
		2, 6, 4,        0, 2, 4,
	};

	unsigned int VAO = 0;
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

	// ========================================================================
	// This section demonstrates how the GPU vertex shader program
	// receives data.

	// TODO(student): If you look closely in the `Init()` and `Update()`
	// functions, you will see that we have three objects which we load
	// and use in three different ways:
	// - LoadMesh   + LabShader (this lab's shader)
	// - CreateMesh + VertexNormal (this shader is already implemented)
	// - CreateMesh + LabShader (this lab's shader)
	// To get an idea about how they're different from one another, do the
	// following experiments. What happens if you switch the color pipe and
	// normal pipe in this function (but not in the shader)? Now, what happens
	// if you do the same thing in the shader (but not in this function)?
	// Finally, what happens if you do the same thing in both places? Why?

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

	Mesh* botMesh = new Mesh(name);
	botMesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
	botMesh->vertices = vertices;
	botMesh->indices = indices;
	return botMesh;
}

