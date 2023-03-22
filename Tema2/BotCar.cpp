#pragma once
#include "lab_m1/Tema2/BotCar.h"
#include "lab_m1/Tema2/Consts.h"
#include <iostream>

BotCar::BotCar() {
	this->moveSpeed = 0;
	this->forward = glm::vec3(0, 0, 0);
	this->rotationsMatrix = glm::mat4(1);
	this->translationsMatrix = glm::mat4(1);
	this->center = glm::vec3(0, 0, 0);
	this->initialPos = glm::vec3(0, 0, 0);
	this->mpi = 0;
	this->midPoints = std::vector<glm::vec3>();
}

BotCar::BotCar(int mpi, float moveSpeed, std::vector<glm::vec3> midPoints) {
	this->midPoints = midPoints;
	this->moveSpeed = moveSpeed;
	this->center = midPoints[mpi];
	this->initialPos = midPoints[mpi];
	this->mpi = mpi;
	this->translationsMatrix = glm::mat4(1);
	this->translationsMatrix = glm::translate(this->translationsMatrix, initialPos);
	this->forward = glm::vec3(0, 0, -1);
	this->rotationsMatrix = glm::mat4(1);
	float delta = getAngle(glm::normalize(this->midPoints[(this->mpi + 1) % this->midPoints.size()] - center), forward);
	this->rotationsMatrix = glm::rotate(this->rotationsMatrix, delta, glm::vec3(0, 1, 0));
	this->forward = glm::normalize(glm::vec3(this->rotationsMatrix * glm::vec4(glm::normalize(this->forward), 1)));
}

BotCar::~BotCar() {

}

void BotCar::moveForward(float deltaTimeSeconds) {
	glm::mat4 tempTrans = glm::translate(glm::mat4(1), this->moveSpeed * deltaTimeSeconds * this->forward);
	this->translationsMatrix *= tempTrans;
	this->center = glm::vec3(tempTrans * glm::vec4(this->center, 1));
}


glm::mat4 BotCar::getTotalTransformation() {
	return this->translationsMatrix * this->rotationsMatrix * glm::scale(glm::mat4(1), glm::vec3(1, 1, 2));
}

std::pair<bool, int> BotCar::isNextPointOnTrack(std::vector<glm::vec3> trackPoints, float deltaTimeSeconds) {
	glm::mat4 tempTrans = glm::translate(glm::mat4(1), this->moveSpeed * this->moveSpeed * deltaTimeSeconds * this->forward);
	glm::vec3 nextPoint = glm::vec3(tempTrans * glm::vec4(this->center, 1));
	int mpi;
	for (int k = 0; k < trackPoints.size() - 3; k += 2) {
		int intersections = 0;
		int intersections2 = 0;
		std::vector<glm::vec3> currPoly = { trackPoints[k], trackPoints[k + 1], trackPoints[k + 3], trackPoints[k + 2] };
		for (int i = 0, j = currPoly.size() - 1; i < currPoly.size(); j = i++) {
			if (((currPoly[i].z > nextPoint.z) != (currPoly[j].z > nextPoint.z)) && (nextPoint.x < (currPoly[j].x - currPoly[i].x) * (nextPoint.z - currPoly[i].z) / (currPoly[j].z - currPoly[i].z) + currPoly[i].x)) {
				intersections += 1;
			}
		}
		for (int i = 0, j = currPoly.size() - 1; i < currPoly.size(); j = i++) {
			if (((currPoly[i].z > center.z) != (currPoly[j].z > center.z)) && (center.x < (currPoly[j].x - currPoly[i].x) * (center.z - currPoly[i].z) / (currPoly[j].z - currPoly[i].z) + currPoly[i].x)) {
				intersections2 += 1;
			}
		}

		if (intersections2 % 2 != 0) {
			mpi = ((k / 2) + 20) % this->midPoints.size();
		}

		if (intersections % 2 != 0) {
			return std::pair<bool, int>(true, ((k / 2) + 1) % this->midPoints.size());
		}
	}
	return std::pair<bool, int>(false, mpi);
}

float BotCar::getAngle(glm::vec3 p1, glm::vec3 p2) {
	float t1 = atan2(p1.z, p1.x);
	float t2 = atan2(p2.z, p2.x);
	float dt = t2 - t1;

	while (dt < -M_PI) {
		dt += 2 * M_PI;
	}

	while (dt > M_PI) {
		dt -= 2 * M_PI;
	}
	return dt;
}


void BotCar::move(float deltaTimeSeconds, std::vector<glm::vec3> trackPoints, std::vector<glm::vec3> midPoints) {
	glm::mat4 tempTrans = glm::translate(glm::mat4(1), this->moveSpeed * deltaTimeSeconds * this->forward);
	glm::vec3 nextPoint = glm::vec3(tempTrans * glm::vec4(this->center, 1));
	//std::cout << center << " " << nextPoint << " " << forward << " " << glm::distance(nextPoint, center) << " " << glm::distance(this->midPoints[(mpi + 1) % this->midPoints.size()], center) << " " << this->midPoints[(mpi + 1) % this->midPoints.size()] << '\n';
	if (glm::distance(nextPoint, center) > glm::distance(this->midPoints[(mpi + 1) % this->midPoints.size()], center)) {
		mpi = (mpi + 1) % this->midPoints.size();
		this->translationsMatrix = glm::translate(glm::mat4(1), this->midPoints[mpi]);
		this->center = this->midPoints[mpi];
		/*float cosinus = glm::dot(glm::normalize(this->midPoints[(mpi + 1) % this->midPoints.size()] - this->midPoints[mpi]), glm::normalize(this->forward)) / (glm::length(glm::normalize(this->forward)) * glm::length(glm::normalize(this->midPoints[(mpi + 1) % this->midPoints.size()] - this->midPoints[mpi])));
		float sinus = glm::length(glm::cross(glm::normalize(this->midPoints[(mpi + 1) % this->midPoints.size()] - this->midPoints[mpi]), glm::normalize(this->forward)) / (glm::length(glm::normalize(this->forward)) * glm::length(glm::normalize(this->midPoints[(mpi + 1) % this->midPoints.size()] - this->midPoints[mpi]))));*/
		/*this->rotationsMatrix = glm::mat4(cosinus, 0, sinus, 0,
			0, 1, 0, 0,
			-sinus, 0, cosinus, 0,
			0, 0, 0, 1);*/
		float delta = getAngle(glm::normalize(this->midPoints[(mpi + 1) % this->midPoints.size()] - center), forward);
		this->rotationsMatrix = glm::rotate(this->rotationsMatrix, delta, glm::vec3(0, 1, 0));
		/*this->forward = glm::normalize(glm::vec3(this->rotationsMatrix * glm::vec4(glm::normalize(this->forward), 1)));*/
		this->forward = glm::normalize(this->midPoints[(mpi + 1) % this->midPoints.size()] - center);
	}
	else {
		this->moveForward(deltaTimeSeconds);
	}
}

void BotCar::move2(float deltaTimeSeconds, std::vector<glm::vec3> trackPoints) {
	glm::mat4 tempTrans = glm::translate(glm::mat4(1), this->moveSpeed * deltaTimeSeconds * this->forward);
	glm::vec3 nextPoint = glm::vec3(tempTrans * glm::vec4(this->center, 1));
	std::pair<bool, int> check = isNextPointOnTrack(trackPoints, deltaTimeSeconds);
	if (!check.first) {
		float delta = getAngle(glm::normalize(this->midPoints[check.second % this->midPoints.size()] - center), forward);
		this->rotationsMatrix = glm::rotate(this->rotationsMatrix, delta, glm::vec3(0, 1, 0));
		this->forward = glm::normalize(this->midPoints[(check.second) % this->midPoints.size()] - center);
	}
	else {
		this->moveForward(deltaTimeSeconds);
	}
}

Mesh* BotCar::createBotMesh(const std::string &name, glm::vec3 color) {
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