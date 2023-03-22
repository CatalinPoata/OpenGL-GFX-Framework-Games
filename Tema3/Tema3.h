
#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema3/Player.h"
#include "lab_m1/Tema3/Track.h"
#include <time.h>
#include "components/transform.h"
#include "lab_extra/basic_text/basic_text.h"

namespace m1

{
    class T3 : public gfxc::SimpleScene
    {
    public:
        T3();
        ~T3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture);
        void RenderTrackMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture);
        void RenderPlayerMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, glm::vec3 color);
        void checkCollisions();
        void RenderScene(float deltaTimeSeconds, int isMinimap);
        void updateObstacles();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
        bool renderCameraTarget;
        Player player;
        Track3 track;

        // TODO(student): If you need any other class variables, define them here.
        float left, right, top, bottom, zNear, zFar, FOV;
        std::string projType;
        gfxc::Camera* camera;
        Tree tree;
        Rock rock;
        LampPost lamp;
        std::vector<Obstacle*> obstacles;
        glm::vec3 lightPositions[21];
        int lightTypes[21];
        float materialKd;
        float materialKs;
        float materialShininess;
        std::unordered_map<std::string, Texture2D*> mapTextures;
        glm::vec3 textureForward;
        glm::vec3 offset;
        int score;
        gfxc::TextRenderer* textRenderer;
    };
}   // namespace m1
