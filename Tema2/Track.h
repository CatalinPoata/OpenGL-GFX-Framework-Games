#include <stdlib.h>
#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/gl_utils.h"

class Track {
public:
	Track();
	~Track();

	std::vector<glm::vec3> generateTrackPoints();
	Mesh* createTrackMesh(const std::string& name, glm::vec3 color);
	Mesh* createGrassMesh(const std::string& name, glm::vec3 color);
	Mesh* createTrunkMesh();
	Mesh* createLeavesMesh();
	void scaleTrack(float sx, float sz);


	std::vector<glm::vec3> trackPoints;
	std::vector<glm::vec3> midPoints;
	int size;
};