#include <stdlib.h>
#include "components/simple_scene.h"

class Track3
{
    public:
    Track3();
    ~Track3();


    void updateCenter(glm::vec3 center);
    glm::mat4 getTotalTransformation();

    glm::vec3 center;
    glm::mat4 translationsMatrix;
    glm::mat4 baseRotateMatrix;
};