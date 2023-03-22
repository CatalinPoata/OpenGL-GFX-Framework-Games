#pragma once
#include "lab_m1/Tema3/Obstacles.h"

Obstacle::Obstacle(){
    this->position = glm::vec3(0, 0, 0);
    this->radius = 0.0f;
}

Obstacle::Obstacle(glm::vec3 position, float radius){
    this->position = position;
    this->radius = radius;
}

Obstacle::~Obstacle(){

}


Tree::Tree(){
}

Tree::Tree(glm::vec3 position, float radius){
    this->position = position;
    this->radius = radius;
    this->baseRotateMatrix = glm::rotate(glm::mat4(1), RADIANS(45), glm::vec3(1, 0, 0));
    this->type = "tree";
    this->actualPos = glm::vec3(baseRotateMatrix * glm::vec4(position, 1));
}

Tree::~Tree(){

}

glm::mat4 Tree::getTrunkTransform(){
    return glm::translate(this->baseRotateMatrix, this->position) * glm::scale(glm::mat4(1), glm::vec3(0.15, 1, 0.15));
}
glm::mat4 Tree::getLeavesTransform(){
    return glm::translate(this->baseRotateMatrix, this->position) * glm::scale(glm::mat4(1) * glm::translate(glm::mat4(1), glm::vec3(0, 0.85, 0)), glm::vec3(0.25, 0.75, 0.25));
}

glm::mat4 Tree::getMesh1Transform() {
    return getTrunkTransform();
}

glm::mat4 Tree::getMesh2Transform() {
    return getLeavesTransform();
}

Rock::Rock(){
}

Rock::Rock(glm::vec3 position, float radius) {
    this->position = position;
    this->radius = radius;
    this->type = "rock";
    this->baseRotateMatrix = glm::rotate(glm::mat4(1), RADIANS(45), glm::vec3(1, 0, 0));
    this->actualPos = glm::vec3(baseRotateMatrix * glm::vec4(position, 1));
}

Rock::~Rock(){

}

glm::mat4 Rock::getRock1Transform(){
    return glm::translate(baseRotateMatrix, this->position + glm::vec3(0.5, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
}

glm::mat4 Rock::getRock2Transform(){
    return glm::translate(baseRotateMatrix, this->position + glm::vec3(-0.5, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
}

glm::mat4 Rock::getMesh1Transform() {
    return getRock1Transform();
}

glm::mat4 Rock::getMesh2Transform() {
    return getRock2Transform();
}

LampPost::LampPost() {

}
LampPost::LampPost(glm::vec3 position, float radius) {
    this->position = position;
    this->radius = radius;
    this->type = "lamppost";
    this->baseRotateMatrix = glm::rotate(glm::mat4(1), RADIANS(45), glm::vec3(1, 0, 0));
    this->actualPos = glm::vec3(baseRotateMatrix * glm::vec4(position, 1));
}

glm::mat4 LampPost::getPostTransform() {
    return glm::translate(baseRotateMatrix, this->position) * glm::scale(glm::mat4(1), glm::vec3(0.15, 6, 0.15));
}

glm::mat4 LampPost::getLampTransform() {
    return glm::translate(baseRotateMatrix, this->position + glm::vec3(0, 3, 0)) * glm::scale(glm::mat4(1), glm::vec3(2, 0.15, 0.15));
}
glm::mat4 LampPost::getMesh1Transform() {
    return getPostTransform();
}

glm::mat4 LampPost::getMesh2Transform() {
    return getLampTransform();
}

LampPost::~LampPost() {
}

Gift::Gift() {
}

Gift::Gift(glm::vec3 position, float radius) {
    this->position = position;
    this->radius = radius;
    this->type = "gift";
    this->baseRotateMatrix = glm::rotate(glm::mat4(1), RADIANS(45), glm::vec3(1, 0, 0));
    this->actualPos = glm::vec3(baseRotateMatrix * glm::vec4(position, 1));
}

glm::mat4 Gift::getMesh1Transform() {
    return glm::translate(baseRotateMatrix, this->position) * glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
}

glm::mat4 Gift::getMesh2Transform() {
    return glm::mat4(1);
}

Gift::~Gift() {

}