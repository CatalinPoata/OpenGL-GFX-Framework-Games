#include <stdlib.h>
#include "components/simple_scene.h"

class Obstacle{
    public:

        Obstacle();
        Obstacle(glm::vec3 position, float radius);
        ~Obstacle();

        virtual glm::mat4 getMesh1Transform() = 0;
        virtual glm::mat4 getMesh2Transform() = 0;

        glm::vec3 position;
        float radius;
        std::string type;
        glm::vec3 actualPos;
};

class Tree : public Obstacle{
    public:

    Tree();
    Tree(glm::vec3 position, float radius);
    ~Tree();

    glm::mat4 getTrunkTransform();
    glm::mat4 getLeavesTransform();
    glm::mat4 getMesh1Transform();
    glm::mat4 getMesh2Transform();

    glm::mat4 baseRotateMatrix;

};

class Rock : public Obstacle{
    public:

    Rock();
    Rock(glm::vec3 position, float radius);
    ~Rock();

    glm::mat4 getRock1Transform();
    glm::mat4 getRock2Transform();
    glm::mat4 getMesh1Transform();
    glm::mat4 getMesh2Transform();

    glm::mat4 baseRotateMatrix;
};

class LampPost : public Obstacle {
    public:

    LampPost();
    LampPost(glm::vec3 position, float radius);
    ~LampPost();

    glm::mat4 getPostTransform();
    glm::mat4 getLampTransform();
    glm::mat4 getMesh1Transform();
    glm::mat4 getMesh2Transform();

    glm::mat4 baseRotateMatrix;
};

class Gift : public Obstacle {
public:
    Gift();
    Gift(glm::vec3 position, float radius);
    ~Gift();

    glm::mat4 getMesh1Transform();
    glm::mat4 getMesh2Transform();

    glm::mat4 baseRotateMatrix;
};