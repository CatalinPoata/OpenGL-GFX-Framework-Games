#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema1/objects2D.h"
#include "lab_m1/Tema1/functii.h"


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


T1::T1()
{
}


T1::~T1()
{
}


void T1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    crosshair = glm::mat3(1);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 1;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;
    bird = Bird(300.0f, 30.0f);

    float angle = (float)rand() / ((float)RAND_MAX / M_PI);

    bird.body.moveTransform = transform2D::Translate(640, 200);
    bird.head.moveTransform = transform2D::Translate(640, 200);
    bird.wing1.moveTransform = transform2D::Translate(640, 200);
    bird.wing2.moveTransform = transform2D::Translate(640, 200);

    

    bird.theta = angle;
    bird.body.rotateTransform = transform2D::Rotate(angle);
    bird.head.rotateTransform = transform2D::Rotate(angle);
    bird.wing1.rotateTransform = transform2D::Rotate(angle);
    bird.wing2.rotateTransform = transform2D::Rotate(angle);
    //bird.updateVertices();

    //bird.speed = 1.0f;

    printf("Bird direction is: %f %f\n", abs(cos(bird.theta)), sin(bird.theta));
    printf("Bird stats are: %f %f\n", bird.speed, bird.scale);
    printf("The angle is: %f\n", TO_DEGREES * bird.theta);

    Mesh* square1 = object2D::CreateSquare("square1", squareSide, glm::vec3(0, 1, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", squareSide, glm::vec3(1, 1, 1));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    for (int i = 0; i < bird.meshes.size(); i++) {
        AddMeshToList(bird.meshes[i]);
    }



    theta = 0;
    scaling = 1;
    currX = 0;
    currY = 0;
    alpha = 0;
    centre = glm::vec2(0, 0);
    score = 0;
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "joystix monospace.ttf"), 36);
    printf("Random direction is: %f, %f\n", cos(bird.theta), sin(bird.theta));
    width = resolution.x;
    height = resolution.y;
    lives = 3;
    bullets = 3;
}


void T1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void T1::Update(float deltaTimeSeconds)
{
    if (lives > 0) {
        word = "Score: " + std::to_string(score);
        glDisable(GL_DEPTH_TEST);
        theta += 1 / M_1_PI * deltaTimeSeconds;

        {
            if (bird.status != 2) {

                RenderMesh2D(meshes["body"], shaders["VertexColor"], bird.body.getTransform());
                RenderMesh2D(meshes["head"], shaders["VertexColor"], bird.head.getTransform());
                RenderMesh2D(meshes["wing"], shaders["VertexColor"], bird.wing1.getTransform());
                RenderMesh2D(meshes["wing"], shaders["VertexColor"], bird.wing2.getTransform());

                int hasHit = bird.hasHitEdge(width, height);
                bird.rotateBird(hasHit);
                //bird.rotateBird(bird.theta);

                bird.move(deltaTimeSeconds);
                //bird.updateVertices();
            }

            if (bird.status >= 2) {
                float angle = (float)rand() / ((float)RAND_MAX / M_PI);

                bird.body.moveTransform = transform2D::Translate(640, 200);
                bird.head.moveTransform = transform2D::Translate(640, 200);
                bird.wing1.moveTransform = transform2D::Translate(640, 200);
                bird.wing2.moveTransform = transform2D::Translate(640, 200);



                bird.theta = angle;
                bird.body.rotateTransform = transform2D::Rotate(angle);
                bird.head.rotateTransform = transform2D::Rotate(angle);
                bird.wing1.rotateTransform = transform2D::Rotate(angle);
                bird.wing2.rotateTransform = transform2D::Rotate(angle);
                bird.position = std::pair<float, float>(0, 0);
                if (bird.status == 3) {
                    bird.speed = 300;
                }
                if (bird.status == 2) {
                    bird.speed /= 10;
                    bird.speed *= 1.5;
                }
                bird.status = 0;
            }
        }

        if (bird.collisions > 5) {
            bird.status = 3;
            if (sin(bird.theta) <= 0) {
                bird.rotateBird(M_PI_2);
            }
            bird.collisions = 0;
            lives--;
        }
    }
    if (bullets == 0) {
        lives--;
        bullets = 3;
        bird.speed = 300;
    }
    if (lives <= 0) {
        textRenderer->RenderText("GAME OVER", (GLfloat)500, (GLfloat)360, (GLfloat)1, glm::vec3(1, 0, 0));
    }
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(640, 0);
    modelMatrix *= transform2D::Scale(1280, 200);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    textRenderer->RenderText(word, (GLfloat)0, (GLfloat)2, (GLfloat)1, glm::vec3(1, 1, 1));
    textRenderer->RenderText("Lives: " + std::to_string(lives), (GLfloat)0, (GLfloat)30, (GLfloat)1, glm::vec3(1, 0, 0));
    textRenderer->RenderText("Bullets: " + std::to_string(bullets), (GLfloat)0, (GLfloat)60, (GLfloat)1, glm::vec3(0, 1, 0));
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], crosshair * transform2D::Scale(10, 10));
    glEnable(GL_DEPTH_TEST);
}


void T1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void T1::OnInputUpdate(float deltaTime, int mods)
{
}


void T1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_R) {
        currX = 0;
        currY = 0;
        alpha = 0;
    }

    if (key == GLFW_KEY_I) {
        factor /= 4;
    }

    if (key == GLFW_KEY_K) {
        factor *= 4;
    }
}


void T1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void T1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    crosshair = transform2D::Translate(mouseX, (720 - mouseY));
}


void T1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{ 
    // Add mouse button press event
    if (button == 1 && bird.isMouseOnBird(mouseX, 720 - mouseY) && bird.status == 0)
    {
        bird.status = 1;
        if (sin(bird.theta) >= 0) {
            bird.rotateBird(-1 * M_PI_2);
        }
        bird.collisions = 0;
        bird.speed *= 10;
        score++;
        bullets = 3;
    }

    else if (button == 1 && !bird.isMouseOnBird(mouseX, 720 - mouseY) && bird.status == 0) {
        bullets--;
    }

}


void T1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void T1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void T1::OnWindowResize(int width, int height)
{
    
}
