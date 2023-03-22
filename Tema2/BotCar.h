#include <stdlib.h>
#include "components/simple_scene.h"

class BotCar {
public:
	BotCar();
	BotCar(int mpi, float moveSpeed, std::vector<glm::vec3> midPoints);
	~BotCar();


	void moveForward(float deltaTimeSeconds);
	void move(float deltaTimeSeconds, std::vector<glm::vec3> trackPoints, std::vector<glm::vec3> midPoints);
	std::pair<bool, int> isNextPointOnTrack(std::vector<glm::vec3> trackPoints, float deltaTimeSeconds);
	glm::mat4 getTotalTransformation();
	Mesh* createBotMesh(const std::string &name, glm::vec3 color);
	float getAngle(glm::vec3 p1, glm::vec3 p2);
	void move2(float deltaTimeSeconds, std::vector<glm::vec3> trackPoints);

	float moveSpeed;
	glm::vec3 forward;
	glm::mat4 translationsMatrix;
	glm::mat4 rotationsMatrix;
	glm::vec3 center;
	glm::vec3 initialPos;
	std::vector<glm::vec3> midPoints;
	int mpi;
};