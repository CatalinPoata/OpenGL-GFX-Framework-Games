#include <stdlib.h>
#include "components/simple_scene.h"
#include "lab_m1/Tema3/Obstacles.h"

class Player{
    public:
    Player();
    Player(glm::vec3 position, float radius, float moveSpeed, float scale);
    ~Player();

    void move(float deltaTimeSeconds);
    void rotate(float theta);
    glm::mat4 getBodyTransFormation();
    glm::mat4 getSki1Transformation();
    glm::mat4 getSki2Transformation();
    bool hasHitObstacle(std::vector<Obstacle> obstacles);
    bool hasCollectedGift(Gift gift);


    float theta;
    float radius;
    float scale;
    glm::vec3 forward;
    float moveSpeed;
    glm::mat4 baseRotationMatrix;
    glm::mat4 bodyTranslationMatrix;
    glm::mat4 bodyRotationMatrix;
    glm::mat4 ski1TranslationMatrix;
    glm::mat4 ski2TranslationMatrix;
    glm::vec3 center;
    glm::vec3 actualForward;
};
