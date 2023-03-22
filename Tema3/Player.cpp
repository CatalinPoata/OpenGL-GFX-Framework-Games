#pragma once
#include "lab_m1/Tema3/Player.h"

Player::Player(){

}

Player::Player(glm::vec3 position, float radius, float moveSpeed, float scale){
    this->theta = 0.0f;
    this->center = position;
    this->radius = radius;
    this->moveSpeed = moveSpeed;
    this->baseRotationMatrix = glm::rotate(glm::mat4(1), RADIANS(45), glm::vec3(1, 0, 0));
    this->forward = glm::normalize(glm::vec3(baseRotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
    this->scale = scale;
    this->bodyRotationMatrix = glm::mat4(1);
    this->bodyTranslationMatrix = glm::mat4(1);
    this->ski1TranslationMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.25, 0, 0));
    this->ski2TranslationMatrix = glm::translate(glm::mat4(1), glm::vec3(0.25, 0, 0));
    this->actualForward = forward;
}

Player::~Player(){

}

void Player::move(float deltaTimeSeconds){
    actualForward = glm::normalize(glm::vec3(bodyRotationMatrix * glm::vec4(this->forward, 1)));
    glm::mat4 tempTrans = glm::translate(glm::mat4(1), this->moveSpeed * deltaTimeSeconds * actualForward);
    this->bodyTranslationMatrix *= tempTrans;
    this->center = glm::vec3(tempTrans * glm::vec4(this->center, 1));
}

void Player::rotate(float theta){
    this->bodyRotationMatrix = glm::rotate(glm::mat4(1), theta, glm::vec3(0, 1, 1));
}

glm::mat4 Player::getBodyTransFormation(){
    return this->bodyTranslationMatrix * glm::scale(this->bodyRotationMatrix * baseRotationMatrix, glm::vec3(scale, scale, scale));
}

glm::mat4 Player::getSki1Transformation(){
    return this->bodyTranslationMatrix * glm::scale(this->bodyRotationMatrix * baseRotationMatrix * this->ski1TranslationMatrix, glm::vec3(0.1, 0.1, 1.5));
}

glm::mat4 Player::getSki2Transformation(){
    return this->bodyTranslationMatrix * glm::scale(this->bodyRotationMatrix * baseRotationMatrix * this->ski2TranslationMatrix, glm::vec3(0.1, 0.1, 1.5));
}

bool Player::hasHitObstacle(std::vector<Obstacle> obstacles){
    return true;
}

bool Player::hasCollectedGift(Gift gift){
    return true;
}
