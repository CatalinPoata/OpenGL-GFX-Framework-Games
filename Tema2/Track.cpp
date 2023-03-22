#pragma once
#include "lab_m1/Tema2/Track.h"

Track::Track() {
	this->size = 20;
	this->trackPoints = generateTrackPoints();
}

Track::~Track() {
}

Mesh* Track::createTrunkMesh() {
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-0.5, -0.2,  0.5), glm::vec3(0.396, 0.263, 0.129), glm::vec3(0.2, 0.8, 0.2)),
		VertexFormat(glm::vec3(0.5, -0.2,  0.5), glm::vec3(0.396, 0.263, 0.129), glm::vec3(0.9, 0.4, 0.2)),
		VertexFormat(glm::vec3(-0.5,  3,  0.5), glm::vec3(0.396, 0.263, 0.129), glm::vec3(0.7, 0.7, 0.1)),
		VertexFormat(glm::vec3(0.5,  3,  0.5), glm::vec3(0.396, 0.263, 0.129), glm::vec3(0.7, 0.3, 0.7)),
		VertexFormat(glm::vec3(-0.5, -0.2, -0.5), glm::vec3(0.396, 0.263, 0.129), glm::vec3(0.3, 0.5, 0.4)),
		VertexFormat(glm::vec3(0.5, -0.2, -0.5), glm::vec3(0.396, 0.263, 0.129), glm::vec3(0.5, 0.2, 0.9)),
		VertexFormat(glm::vec3(-0.5,  3, -0.5), glm::vec3(0.396, 0.263, 0.129), glm::vec3(0.7, 0.0, 0.7)),
		VertexFormat(glm::vec3(0.5,  3, -0.5), glm::vec3(0.396, 0.263, 0.129), glm::vec3(0.1, 0.5, 0.8)),
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

	Mesh* trunkMesh = new Mesh("trunk");
	trunkMesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
	trunkMesh->vertices = vertices;
	trunkMesh->indices = indices;
	return trunkMesh;
}
Mesh* Track::createLeavesMesh() {
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-2, 3,  2), glm::vec3(0, 0.5, 0), glm::vec3(0.2, 0.8, 0.2)),
		VertexFormat(glm::vec3(2, 3,  2), glm::vec3(0, 0.5, 0), glm::vec3(0.9, 0.4, 0.2)),
		VertexFormat(glm::vec3(-2,  5,  2), glm::vec3(0, 0.5, 0), glm::vec3(0.7, 0.7, 0.1)),
		VertexFormat(glm::vec3(2,  5,  2), glm::vec3(0, 0.5, 0), glm::vec3(0.7, 0.3, 0.7)),
		VertexFormat(glm::vec3(-2, 3, -2), glm::vec3(0, 0.5, 0), glm::vec3(0.3, 0.5, 0.4)),
		VertexFormat(glm::vec3(2, 3, -2), glm::vec3(0, 0.5, 0), glm::vec3(0.5, 0.2, 0.9)),
		VertexFormat(glm::vec3(-2,  5, -2), glm::vec3(0, 0.5, 0), glm::vec3(0.7, 0.0, 0.7)),
		VertexFormat(glm::vec3(2,  5, -2), glm::vec3(0, 0.5, 0), glm::vec3(0.1, 0.5, 0.8)),
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

	Mesh* leavesMesh = new Mesh("leaves");
	leavesMesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
	leavesMesh->vertices = vertices;
	leavesMesh->indices = indices;
	return leavesMesh;
}

Mesh* Track::createGrassMesh(const std::string& name, glm::vec3 color) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	for (int i = -250; i < 250; i++) {
		for (int j = -250; j < 250; j++) {
			vertices.push_back(VertexFormat(glm::vec3(i, -0.2, j), color));
			vertices.push_back(VertexFormat(glm::vec3(i, -0.2, j - 1), color));
			vertices.push_back(VertexFormat(glm::vec3(i + 1, -0.2, j), color));
			vertices.push_back(VertexFormat(glm::vec3(i + 1, -0.2, j + 1), color));

		}
	}

	for (int i = 0; i < vertices.size(); i++) {
		if (i % 3 == 0 && i > 0) {
			indices.push_back(i);
			indices.push_back(i - 3);
			indices.push_back(i - 2);
		}
		else {
			indices.push_back(i);
		}
	}

	Mesh* grassMesh = new Mesh(name);
	grassMesh->SetDrawMode(GL_TRIANGLE_STRIP);
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

	grassMesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
	grassMesh->vertices = vertices;
	grassMesh->indices = indices;
	return grassMesh;
}

Mesh* Track::createTrackMesh(const std::string& name, glm::vec3 color) {
	std::vector<VertexFormat> vertices;
	for (int i = 0; i < this->trackPoints.size() - 1; i++) {
		vertices.push_back(VertexFormat(this->trackPoints[i], color));
	}
	
	Mesh* trackMesh = new Mesh(name);
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < this->trackPoints.size() - 1; i++) {
		indices.push_back(i);
	}
	indices.push_back(0);
	indices.push_back(1);

	trackMesh->SetDrawMode(GL_TRIANGLE_STRIP);

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



	trackMesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
	trackMesh->vertices = vertices;
	trackMesh->indices = indices;
	return trackMesh;
}

std::vector<glm::vec3> Track::generateTrackPoints() {
	float alpha = RADIANS(236.3099324);
	float beta = RADIANS(303.6900675);
	float gamma = RADIANS(56.3099324);
	float theta = RADIANS(123.6900675);
	float radius1 = 1.3027756;
	float radius2 = 0.5;
	float iter;
	std::vector<glm::vec3> midPoints;
	std::vector<glm::vec3> trackPoints;
	float densitate = RADIANS(1);

	iter = gamma;
	while (iter <= beta) {
		midPoints.push_back(glm::vec3(radius2 * cos(iter) - 1.0, 0, radius2 * sin(iter)));
		iter += densitate;
	}


	iter = beta;
	while (iter >= alpha) {
		midPoints.push_back(glm::vec3(-radius1 * cos(iter), 0, -radius1 * sin(iter) - 1.5));
		iter -= densitate;
	}

	iter = alpha;
	while (iter <= RADIANS(360)) {
		midPoints.push_back(glm::vec3(radius2 * cos(iter) + 1.0, 0, radius2 * sin(iter)));
		iter += densitate;
	}

	iter = 0.0;
	while (iter <= theta) {
		midPoints.push_back(glm::vec3(radius2 * cos(iter) + 1.0, 0, radius2 * sin(iter)));
		iter += densitate;
	}

	iter = beta;
	while (iter >= alpha) {
		midPoints.push_back(glm::vec3(radius1 * cos(iter), 0, radius1 * sin(iter) + 1.5));
		iter -= densitate;
	}

	for (int i = 0; i < midPoints.size() - 1; i++) {
		glm::vec3 P1 = midPoints[i];
		glm::vec3 P2 = midPoints[i + 1];
		glm::vec3 D = P2 - P1;
		glm::vec3 P = glm::cross(D, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 normalizedP = glm::normalize(P);
		trackPoints.push_back(P1 + 0.1f * normalizedP);
		trackPoints.push_back(P1 - 0.1f * normalizedP);
	}

	glm::vec3 P1 = midPoints[midPoints.size() - 1];
	glm::vec3 P2 = midPoints[0];
	glm::vec3 D = P2 - P1;
	glm::vec3 P = glm::cross(D, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 normalizedP = glm::normalize(P);
	trackPoints.push_back(P1 + 0.1f * normalizedP);
	trackPoints.push_back(P1 - 0.1f * normalizedP);

	P1 = midPoints[0];
	P2 = midPoints[1];
	D = P2 - P1;
	P = glm::cross(D, glm::vec3(0.0f, 1.0f, 0.0f));
	normalizedP = glm::normalize(P);
	trackPoints.push_back(P1 + 0.1f * normalizedP);
	trackPoints.push_back(P1 - 0.1f * normalizedP);

	P1 = midPoints[1];
	P2 = midPoints[2];
	D = P2 - P1;
	P = glm::cross(D, glm::vec3(0.0f, 1.0f, 0.0f));
	normalizedP = glm::normalize(P);
	trackPoints.push_back(P1 + 0.1f * normalizedP);
	trackPoints.push_back(P1 - 0.1f * normalizedP);

	this->midPoints = midPoints;

	return trackPoints;
}

void Track::scaleTrack(float sx, float sz) {
	for (int i = 0; i < trackPoints.size(); i++) {
		this->trackPoints[i].x *= sx;
		this->trackPoints[i].z *= sz;
	}

	for (int i = 0; i < midPoints.size(); i++) {
		this->midPoints[i].x *= sx;
		this->midPoints[i].z *= sz;
	}
}