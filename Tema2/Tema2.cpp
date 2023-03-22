#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


T2::T2()
{
}


T2::~T2()
{
}


void T2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::CameraHW();
    //camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    float coef = 10;
    bottom = -0.5 * coef;
    left = -window->props.aspectRatio * coef / 2;
    zNear = 0.02;
    zFar = 200.0f;
    right = window->props.aspectRatio * coef / 2;
    top = 0.5 * coef;
    FOV = RADIANS(60);
    projType = "pers";
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);
    scaleFactor = 0.007;
    glm::vec3 initialPos = glm::vec3(0, 0.5, -8);

    playerCar = PlayerCar(initialPos, M_PI_2, 5, 2.5);
    AddMeshToList(playerCar.createCarMesh("player", glm::vec3(1, 1, 1)));
    camera = new implemented::CameraHW(initialPos, playerCar.center, glm::vec3(0, 1, 0), glm::vec3(0, 2, 3.5f));
    camera->RotateThirdPerson_OY(playerCar.initialTheta);
    minimapCamera = new implemented::CameraHW(initialPos, playerCar.center, glm::vec3(0, 0, -1), glm::vec3(0, 100, 0));
    
    this->track = Track();
    track.scaleTrack(40.0f, 40.0f);
    Mesh* trackMesh = track.createTrackMesh("track", glm::vec3(0, 0, 0));
    AddMeshToList(trackMesh);
    AddMeshToList(track.createGrassMesh("grass", glm::vec3(0, 1, 0)));
    AddMeshToList(track.createTrunkMesh());
    AddMeshToList(track.createLeavesMesh());


    botCars = std::vector<BotCar>();
    for (int i = 0; i < ENEMIES; i++) {
        botCars.push_back(BotCar(((rand() % 400) + 50) % track.midPoints.size(), ((i + 1) * 3) % 10, track.midPoints));
    }
    for (int i = 0; i < ENEMIES; i++) {
        AddMeshToList(botCars[i].createBotMesh(std::string("bot" + std::to_string(i)), glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX)));
    }
    Shader* shader = new Shader("HWShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

}


void T2::FrameStart()
{
}


void T2::RenderScene(float deltaTimeSeconds, int isMinimap) {
    implemented::CameraHW* usedCamera;
    if (isMinimap == SCREEN)
    {
        if (!isnan(window->props.aspectRatio)) {
            projectionMatrix = glm::perspective(FOV, window->props.aspectRatio, zNear, zFar);
            usedCamera = camera;
        }
        else {
            return;
        }
        
    }

    if (isMinimap == MINIMAP)
    {
        usedCamera = minimapCamera;
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix, camera);
    }

    RenderMesh(meshes["player"], shaders["VertexColor"], playerCar.getTotalTransformation(), usedCamera);
    RenderMesh(meshes["track"], shaders["HWShader"], glm::mat4(1), usedCamera);
    RenderMesh(meshes["grass"], shaders["HWShader"], glm::mat4(1), usedCamera);
    for (int i = 0; i < ENEMIES; i++) {
        RenderMesh(meshes[std::string("bot" + std::to_string(i))], shaders["HWShader"], botCars[i].getTotalTransformation(), usedCamera);
    }
    RenderForest(usedCamera);

    for (int i = 0; i < ENEMIES; i++) {
        botCars[i].move2(deltaTimeSeconds, track.trackPoints);
    }
}


void T2::Update(float deltaTimeSeconds)
{
    if (isnan(window->props.aspectRatio)) {
        return;
    }
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5294, 0.8078, 0.9216, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    RenderScene(deltaTimeSeconds, SCREEN);

    glViewport(0, 0, 200 * window->props.aspectRatio, 200);
    RenderScene(deltaTimeSeconds, MINIMAP);

}


void T2::FrameEnd()
{
}

void T2::RenderTree(glm::vec3 location, implemented::CameraHW* camera) {
    RenderMesh(meshes["trunk"], shaders["HWShader"], glm::translate(glm::mat4(1), location), camera);
    RenderMesh(meshes["leaves"], shaders["HWShader"], glm::translate(glm::mat4(1), location), camera);
}

void T2::RenderForest(implemented::CameraHW* camera) {
    for (int i = 0; i < track.midPoints.size() - 1; i += 25) {
        glm::vec3 P1 = track.midPoints[i];
        glm::vec3 P2 = track.midPoints[i + 1];
        glm::vec3 D = P2 - P1;
        glm::vec3 P = glm::cross(D, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 normalizedP = glm::normalize(P);
        RenderTree((P1 + 7.0f * normalizedP), camera);
        RenderTree((P1 - 7.0f * normalizedP), camera);
    }
}


void T2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::CameraHW *camera)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int location = glGetUniformLocation(shader->program, "carPosition");
    glUniform3fv(location, 1, glm::value_ptr(playerCar.center));

    location = glGetUniformLocation(shader->program, "scaleFactor");
    glUniform1f(location, scaleFactor);

    mesh->Render();
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void T2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 100.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->TranslateForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-1 * cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(-1 * cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-1 * cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }
    else {
        if (window->KeyHold(GLFW_KEY_W) && playerCar.isOnTrack(track.trackPoints, deltaTime) && !playerCar.hasHitCar(botCars)) {
            camera->MoveForward(playerCar.moveSpeed * deltaTime);
            playerCar.moveForward(deltaTime, FORWARD);
            minimapCamera->position = playerCar.center + glm::vec3(0, 100, 0);
        }

        if (window->KeyHold(GLFW_KEY_S) && playerCar.isOnTrack(track.trackPoints, deltaTime) && !playerCar.hasHitCar(botCars)) {
            camera->MoveForward(-1 * playerCar.moveSpeed * deltaTime);
            playerCar.moveForward(deltaTime, BACKWARDS);
            minimapCamera->position = playerCar.center + glm::vec3(0, 100, 0);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->RotateThirdPerson_OY(deltaTime * LEFT * playerCar.rotateSpeed);
            playerCar.rotate(deltaTime, LEFT);
            minimapCamera->position = playerCar.center + glm::vec3(0, 100, 0);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->RotateThirdPerson_OY(deltaTime * RIGHT * playerCar.rotateSpeed);
            playerCar.rotate(deltaTime, RIGHT);
            minimapCamera->position = playerCar.center + glm::vec3(0, 100, 0);
        }
    }

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.

    if (window->KeyHold(GLFW_KEY_K))
    {
        FOV -= RADIANS(50) * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_L))
    {
        FOV += RADIANS(50) * deltaTime;
    }

    if (projType == "orth")
    {
        if (window->KeyHold(GLFW_KEY_J))
        {
            right += 10 * deltaTime;
            left -= 10 * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_G) && right >= 0 && left <= 0)
        {
            right -= 10 * deltaTime;
            left += 10 * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_Y))
        {
            top += 10 * deltaTime;
            bottom -= 10 * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_H) && top >= 0 && bottom <= 0)
        {
            top -= 10 * deltaTime;
            bottom += 10 * deltaTime;
        }

    }

}


void T2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_O)
    {
        projType = "orth";
    }
    if (key == GLFW_KEY_P)
    {
        if (botCars[0].moveSpeed == 0.0f) {
            botCars[0].moveSpeed = 10;
        }
        else {
            botCars[0].moveSpeed = 0;
        }
    }
    if (key == GLFW_KEY_R)
    {
        playerCar.reset();
        delete camera;
        camera = new implemented::CameraHW(playerCar.initialPos, playerCar.center, glm::vec3(0, 1, 0), glm::vec3(0, 2, 3.5f));
        camera->RotateThirdPerson_OY(playerCar.initialTheta);
        delete minimapCamera;
        minimapCamera = new implemented::CameraHW(playerCar.initialPos, playerCar.center, glm::vec3(0, 0, -1), glm::vec3(0, 150, 0));
    }
}


void T2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void T2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            if (deltaX != 0) {
                camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
            }
            if (deltaY != 0) {
                camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
            }

        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            if (deltaX != 0) {
                camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
            }
            if (deltaY != 0) {
                camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            }
        }
    }
}


void T2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void T2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void T2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void T2::OnWindowResize(int width, int height)
{
}
