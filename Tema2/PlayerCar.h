#include <stdlib.h>
#include "components/simple_scene.h"
#include "lab_m1/Tema2/BotCar.h"

class PlayerCar
{
public:

	PlayerCar();
	PlayerCar(glm::vec3 initialPos, float initialTheta, float moveSpeed, float rotateSpeed);
	~PlayerCar();

	void moveForward(float deltaTimeSeconds, int way);
	void rotate(float deltaTimeSeconds, int way);
	glm::mat4 getTotalTransformation();
	void reset();
	bool isOnTrack(std::vector<glm::vec3> trackPoints, float deltaTimeSeconds);
	bool hasHitCar(std::vector<BotCar> botCars);
	Mesh* createCarMesh(const std::string& name, glm::vec3 color);

	float theta;
	float initialTheta;
	float moveSpeed;
	float rotateSpeed;
	glm::vec3 forward;
	glm::mat4 translationsMatrix;
	glm::mat4 rotationsMatrix;
	glm::vec3 center;
	glm::vec3 initialPos;
};
