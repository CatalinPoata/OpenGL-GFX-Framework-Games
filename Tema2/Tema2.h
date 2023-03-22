#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/hw_camera.h"
#include "lab_m1/Tema2/PlayerCar.h"
#include "lab_m1/Tema2/Consts.h"
#include "lab_m1/Tema2/Track.h"

namespace m1
{
    class T2 : public gfxc::SimpleScene
    {
    public:
        T2();
        ~T2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::CameraHW *camera);
        void RenderScene(float deltaTimeSeconds, int isMinimap);
        void RenderTree(glm::vec3 location, implemented::CameraHW* camera);
        void RenderForest(implemented::CameraHW *camera);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::CameraHW* camera;
        implemented::CameraHW* minimapCamera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        PlayerCar playerCar;
        std::vector<BotCar> botCars;
        float scaleFactor;

        // TODO(student): If you need any other class variables, define them here.
        float left, right, top, bottom, zNear, zFar, FOV;
        std::string projType;
        Track track;

    };
}   // namespace m1
