#include "lab_m1/Tema3/Track.h"

Track3::Track3(){
    this->center = glm::vec3(0, 0, 0);
    this->translationsMatrix = glm::mat4(1);
    baseRotateMatrix = glm::rotate(glm::mat4(1), RADIANS(45), glm::vec3(1, 0, 0));
}

Track3::~Track3(){

}

void Track3::updateCenter(glm::vec3 center){
    this->translationsMatrix = glm::translate(glm::translate(glm::mat4(1), glm::vec3(0, -0.2, 0)), center);
}

glm::mat4 Track3::getTotalTransformation(){
    return this->translationsMatrix * baseRotateMatrix * glm::scale(glm::mat4(1), glm::vec3(60, 0.1, 60));
}