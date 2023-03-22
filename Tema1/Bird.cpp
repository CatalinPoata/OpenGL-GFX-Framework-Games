#pragma once
#include "lab_m1/Tema1/Bird.h"


Bird::Bird() {
}


Bird::Bird(float speed, float scale) {
	this->theta = 0;
	this->speed = speed;
	this->scale = scale;
	this->wing1 = Wing(this->scale, 1, this->theta);
	this->wing2 = Wing(this->scale, 2, this->theta);
	this->head = Head(this->scale, this->theta);
	this->body = Body(this->scale, this->theta);
	this->meshes.clear();
	this->meshes.push_back(this->body.createBodyMesh("body", glm::vec3(0.588f, 0.294f, 0.0f)));
	this->meshes.push_back(this->head.createHeadMesh("head", glm::vec3(0, 1, 0)));
	this->meshes.push_back(this->wing1.createWingMesh("wing", glm::vec3(0.588f, 0.294f, 0.0f)));
	this->position = std::pair<float, float>(0, 0);
	this->status = 0;
	this->collisions = 0;
}


Bird::~Bird() {
}

void Bird::move(float deltaTimeSeconds) {
	this->position.first = cos(theta) * speed * deltaTimeSeconds;
	this->position.second = sin(theta) * speed * deltaTimeSeconds;
	
	glm::mat3 moveTransf = transform2D::Translate(position.first, position.second);
	
	this->body.moveTransform *= moveTransf;
	this->head.moveTransform *= moveTransf;
	this->wing1.moveTransform *= moveTransf;
	this->wing2.moveTransform *= moveTransf;
	this->wing1.flap(deltaTimeSeconds);
	this->wing2.flap(deltaTimeSeconds);

}

void Bird::rotateBird(int c) {
	if (c == 0) {
		return;
	}
	float newAngle;

	if (c == 1) {
		newAngle = M_PI - this->theta;
	}

	if (c == -1) {
		newAngle = -1 * this->theta;
	}

	if (c == 2) {
		newAngle = M_PI + this->theta;
	}

	this->body.rotateTransform = transform2D::Rotate(newAngle);
	this->head.rotateTransform = transform2D::Rotate(newAngle);
	this->wing1.rotateTransform = transform2D::Rotate(newAngle);
	this->wing2.rotateTransform = transform2D::Rotate(newAngle);
	this->theta = newAngle;
}

int Bird::hasHitEdge(int width, int height) {
	int partsHH = 0;
	int bHH = body.hasHitEdge(width, height);
	int hHH = head.hasHitEdge(width, height);
	int w1HH = wing1.hasHitEdge(width, height);
	int w2HH = wing2.hasHitEdge(width, height);

	if (bHH != 0) {
		partsHH++;
	}
	if (hHH != 0) {
		partsHH++;
	}
	if (w1HH != 0) {
		partsHH++;
	}
	if (w2HH != 0) {
		partsHH++;
	}
	if (this->status == 1 && partsHH == 1 && (0 > (bHH + hHH + w1HH + w2HH))) {
		this->status = 2;
	}
	if (partsHH > 1) {
		this->collisions++;
		return 2;
	}

	if (partsHH == 1) {
		this->collisions++;
		return bHH + hHH + w1HH + w2HH;
	}
	return 0;
}

void Bird::updateVertices() {
	this->head.updateVertices();
	this->body.updateVertices();
	this->wing1.updateVertices();
	this->wing2.updateVertices();
}

bool Bird::isMouseOnBird(int mouseX, int mouseY) {

	return (head.isMouseOnHead(mouseX, mouseY)) || (body.isMouseOnBody(mouseX, mouseY)) || (wing1.isMouseOnWing(mouseX, mouseY)) || (wing2.isMouseOnWing(mouseX, mouseY));
}
