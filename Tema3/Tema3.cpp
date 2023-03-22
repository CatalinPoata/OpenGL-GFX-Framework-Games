
#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Mesh* createCube(string name, glm::vec3 color){
    	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-0.5, 0,  0.5), color, glm::vec3(0.2, 0.8, 0.2)),
		VertexFormat(glm::vec3(0.5, 0,  0.5), color, glm::vec3(0.9, 0.4, 0.2)),
		VertexFormat(glm::vec3(-0.5,  1,  0.5), color, glm::vec3(0.7, 0.7, 0.1)),
		VertexFormat(glm::vec3(0.5,  1,  0.5), color, glm::vec3(0.7, 0.3, 0.7)),
		VertexFormat(glm::vec3(-0.5, 0, -0.5), color, glm::vec3(0.3, 0.5, 0.4)),
		VertexFormat(glm::vec3(0.5, 0, -0.5), color, glm::vec3(0.5, 0.2, 0.9)),
		VertexFormat(glm::vec3(-0.5,  1, -0.5), color, glm::vec3(0.7, 0.0, 0.7)),
		VertexFormat(glm::vec3(0.5,  1, -0.5), color, glm::vec3(0.1, 0.5, 0.8)),
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 2,        1, 3, 2,
		2, 3, 7,        2, 7, 6,
		1, 7, 3,        1, 5, 7,
		6, 7, 4,        7, 5, 4,
		0, 4, 1,        1, 4, 5,
		2, 6, 4,        0, 2, 4,
	};

	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Create the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section demonstrates how the GPU vertex shader program
	// receives data.

	// TODO(student): If you look closely in the `Init()` and `Update()`
	// functions, you will see that we have three objects which we load
	// and use in three different ways:
	// - LoadMesh   + LabShader (this lab's shader)
	// - CreateMesh + VertexNormal (this shader is already implemented)
	// - CreateMesh + LabShader (this lab's shader)
	// To get an idea about how they're different from one another, do the
	// following experiments. What happens if you switch the color pipe and
	// normal pipe in this function (but not in the shader)? Now, what happens
	// if you do the same thing in the shader (but not in this function)?
	// Finally, what happens if you do the same thing in both places? Why?

	// Set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// Set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// Set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// Set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	Mesh* botMesh = new Mesh(name);
	botMesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
	botMesh->vertices = vertices;
	botMesh->indices = indices;
	return botMesh;
}

float calculateAngle(int mouseX){
    if(mouseX <= 320){
        return -1 * M_PI_4;
    }
    if(mouseX >= 960){
        return M_PI_4;
    }

    float newMouseX = (float)mouseX;

    return ((((newMouseX - 320.0) / 640.0) * M_PI_2) - M_PI_4);
}


T3::T3()
{
}


T3::~T3()
{
}

void T3::Init()
{
    renderCameraTarget = false;

    srand((unsigned)time(NULL));

    camera = GetSceneCamera();
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.02f, 200.0f);

    player = Player(glm::vec3(0, 0, 0), 1.0f, 0.0f, 0.5f);
    track = Track3();
    tree = Tree(glm::vec3(0, 0, 2), 1);
    rock = Rock(glm::vec3(0, 0, 0), 1);
    lamp = LampPost(glm::vec3(0, 0, 0), 1);

    materialShininess = 30;
    materialKd = 0.5;
    materialKs = 0.5;
    lightPositions[0] = glm::vec3(0, 100, 100);
    lightTypes[0] = 0;
    textureForward = glm::vec3(0, 0, 1);
    offset = glm::vec3(0, 0, 0);
    score = 0;

    for (int i = 0; i < 20; i++) {
        int n = rand()%4;
        glm::vec3 playerCoords = glm::vec3(glm::rotate(glm::mat4(1), RADIANS(-45), glm::vec3(1, 0, 0)) * glm::vec4(player.center, 0.0f));
        int posx = (float)(playerCoords.x - 10) + (float)rand() / ((float)(RAND_MAX / (((float)playerCoords.x + 10) - (float)(playerCoords.x - 10))));
        int posz = (float)(playerCoords.z -20) + (float)rand() / ((float)(RAND_MAX / ((float)(playerCoords.z + 20) - (float)(playerCoords.z - 20))));
        if (n == 0) {
            obstacles.push_back(new Tree(glm::vec3(posx, 0, posz), 1));
            lightPositions[i + 1] = obstacles[i]->actualPos;
            lightTypes[i + 1] = 2;
        }
        if(n == 1) {
            obstacles.push_back(new Rock(glm::vec3(posx, 0, posz), 1));
            lightPositions[i + 1] = obstacles[i]->actualPos;
            lightTypes[i + 1] = 2;
        }
        if (n == 2) {
            obstacles.push_back(new LampPost(glm::vec3(posx, 0, posz), 1));
            lightTypes[i + 1] = 1;
            lightPositions[i + 1] = obstacles[i]->actualPos;
        }
        if (n == 3) {
            obstacles.push_back(new Gift(glm::vec3(posx, 0, posz), 1));
            lightTypes[i + 1] = 3;
            lightPositions[i + 1] = obstacles[i]->actualPos;
        }
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = createCube("cube", glm::vec3(1, 0, 0));
        AddMeshToList(mesh);
    }

    {
        Mesh* mesh = createCube("track", glm::vec3(1, 1, 1));
        AddMeshToList(mesh);
    }

    {
        Mesh* mesh = createCube("ski", glm::vec3(0, 0, 1));
        AddMeshToList(mesh);
    }

    {
        Mesh* mesh = createCube("trunk", glm::vec3(0.545, 0.27, 0.074));
        AddMeshToList(mesh);
    }

    {
        Mesh* mesh = new Mesh("leaves");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "tree", "leaves.jpg").c_str(), GL_REPEAT);
        mapTextures["leaves"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "tree", "trunk.jpg").c_str(), GL_REPEAT);
        mapTextures["trunk"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props", "concrete.png").c_str(), GL_REPEAT);
        mapTextures["rock"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "snow", "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props", "gift.jpg").c_str(), GL_REPEAT);
        mapTextures["gift"] = texture;
    }

    {
        Shader* shader = new Shader("groundShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "T3GroundVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "T3GroundFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("objectShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "T3VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "T3FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("playerShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "T3PlayerVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "T3PlayerFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->props.resolution.x, window->props.resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "joystix monospace.ttf"), 36);
    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
}


void T3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void T3::RenderScene(float deltaTimeSeconds, int isMinimap) {
    
}


void T3::updateObstacles(){
    for(int i = 0; i < obstacles.size(); i++) {
        if (glm::length(player.center - obstacles[i]->actualPos) > 15) {
            int n = rand() % 4;
            glm::vec3 playerCoords = glm::vec3(glm::rotate(glm::mat4(1), RADIANS(-45), glm::vec3(1, 0, 0)) * glm::vec4(player.center, 1));
            int posx = (float)(playerCoords.x - 10) + (float)rand() / ((float)(RAND_MAX / (((float)playerCoords.x + 10) - (float)(playerCoords.x - 10))));
            int posz = (float)(playerCoords.z + 7) + (float)rand() / ((float)(RAND_MAX / ((float)(playerCoords.z + 15) - (float)(playerCoords.z + 3))));
            if (n == 0) {
                obstacles[i] = new Tree(glm::vec3(posx, 0, posz), 1);
                lightPositions[i + 1] = obstacles[i]->actualPos;
                lightTypes[i + 1] = 2;
            }
            if(n == 1) {
                obstacles[i] = new Rock(glm::vec3(posx, 0, posz), 1);
                lightPositions[i + 1] = obstacles[i]->actualPos;
                lightTypes[i + 1] = 2;
            }
            if (n == 2) {
                obstacles[i] = new LampPost(glm::vec3(posx, 0, posz), 1);
                lightTypes[i + 1] = 1;
                lightPositions[i + 1] = obstacles[i]->actualPos;
            }

            if (n == 3) {
                obstacles[i] = new Gift(glm::vec3(posx, 0, posz), 1);
                lightTypes[i + 1] = 3;
                lightPositions[i + 1] = obstacles[i]->actualPos;
            }

        }
    }
}

void T3::checkCollisions() {
    for (int i = 0; i < obstacles.size(); i++) {
        if (glm::length(player.center - obstacles[i]->actualPos) < 0.5 && obstacles[i]->type != "gift") {
            player.moveSpeed = 0;
            textRenderer->RenderText("Final score: " + to_string(score), 500, 360, 1, glm::vec3(1, 0, 0));
        }
        else if(glm::length(player.center - obstacles[i]->actualPos) < 0.5 && obstacles[i]->type == "gift") {
            score++;
            glm::vec3 playerCoords = glm::vec3(glm::rotate(glm::mat4(1), RADIANS(-45), glm::vec3(1, 0, 0)) * glm::vec4(player.center, 1));
            int posx = (float)(playerCoords.x - 10) + (float)rand() / ((float)(RAND_MAX / (((float)playerCoords.x + 10) - (float)(playerCoords.x - 10))));
            int posz = (float)(playerCoords.z + 7) + (float)rand() / ((float)(RAND_MAX / ((float)(playerCoords.z + 15) - (float)(playerCoords.z + 3))));
            obstacles[i] = new Gift(glm::vec3(posx, 0, posz), 1);
            lightTypes[i + 1] = 3;
            lightPositions[i + 1] = obstacles[i]->actualPos;
        }
    }
}


void T3::Update(float deltaTimeSeconds)
{
    textRenderer->RenderText("Score: " + to_string(score), 0, 0, 1, glm::vec3(1, 1, 1));
    player.move(deltaTimeSeconds);
    offset += player.actualForward * (player.moveSpeed / 50) * (deltaTimeSeconds / 2);
    //std::cout << "Player center = " << player.center << '\n';
    track.updateCenter(player.center);
    camera->SetPosition(player.center + glm::vec3(0, 2, 5));
    checkCollisions();
    RenderPlayerMesh(meshes["box"], shaders["playerShader"], player.getBodyTransFormation(), NULL, glm::vec3(1, 0, 0));
    RenderTrackMesh(meshes["box"], shaders["groundShader"], track.getTotalTransformation(), mapTextures["snow"]);
    RenderPlayerMesh(meshes["box"], shaders["playerShader"], player.getSki1Transformation(), NULL, glm::vec3(0, 0, 1));
    RenderPlayerMesh(meshes["box"], shaders["playerShader"], player.getSki2Transformation(), NULL, glm::vec3(0, 0, 1));
    for (int i = 0; i < obstacles.size(); i++) {
        if (obstacles[i]->type == "tree") {
            RenderMesh(meshes["trunk"], shaders["objectShader"], obstacles[i]->getMesh1Transform(), mapTextures["trunk"]);
            RenderMesh(meshes["leaves"], shaders["objectShader"], obstacles[i]->getMesh2Transform(), mapTextures["leaves"]);
        }

        if (obstacles[i]->type == "rock") {
            RenderMesh(meshes["sphere"], shaders["objectShader"], obstacles[i]->getMesh1Transform(), mapTextures["rock"]);
            RenderMesh(meshes["sphere"], shaders["objectShader"], obstacles[i]->getMesh2Transform(), mapTextures["rock"]);
        }
        if (obstacles[i]->type == "lamppost") {
            RenderMesh(meshes["box"], shaders["VertexColor"], obstacles[i]->getMesh1Transform(), mapTextures["rock"]);
            RenderMesh(meshes["box"], shaders["VertexColor"], obstacles[i]->getMesh2Transform(), mapTextures["rock"]);

        }
        if (obstacles[i]->type == "gift") {
            RenderMesh(meshes["box"], shaders["objectShader"], obstacles[i]->getMesh1Transform(), mapTextures["gift"]);
        }
    }

    updateObstacles();

}


void T3::FrameEnd()
{
    DrawCoordinateSystem();
}


void T3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
    if (!mesh || !shader || !shader->program)
        return;

    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glm::vec3 eyePosition = camera->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int light_position = glGetUniformLocation(shader->program, "lightPositions");
    glUniform3fv(light_position, 21, glm::value_ptr(lightPositions[0]));

    int light_types = glGetUniformLocation(shader->program, "lightTypes");
    glUniform1iv(light_types, 21, &lightTypes[0]);


    if (texture != NULL) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_t"), 0);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    mesh->Render();
}

void T3::RenderTrackMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
    if (!mesh || !shader || !shader->program)
        return;

    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glm::vec3 eyePosition = camera->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int light_position = glGetUniformLocation(shader->program, "lightPositions");
    glUniform3fv(light_position, 21, glm::value_ptr(lightPositions[0]));

    int light_types = glGetUniformLocation(shader->program, "lightTypes");
    glUniform1iv(light_types, 21, &lightTypes[0]);

    int forward = glGetUniformLocation(shader->program, "offset");
    glUniform3f(forward, -1 * offset.x, offset.y,  - 1 * offset.z);


    if (texture != NULL) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_t"), 1);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    mesh->Render();
}

void T3::RenderPlayerMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, glm::vec3 color)
{
    if (!mesh || !shader || !shader->program)
        return;

    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glm::vec3 eyePosition = camera->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int light_position = glGetUniformLocation(shader->program, "lightPositions");
    glUniform3fv(light_position, 21, glm::value_ptr(lightPositions[0]));

    int light_types = glGetUniformLocation(shader->program, "lightTypes");
    glUniform1iv(light_types, 21, &lightTypes[0]);

    int forward = glGetUniformLocation(shader->program, "objColor");
    glUniform3f(forward, color.x, color.y, color.z);


    if (texture != NULL) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_t"), 1);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    mesh->Render();
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void T3::OnInputUpdate(float deltaTime, int mods)
{

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.

}


void T3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_P) {
        if (player.moveSpeed < 1) {
            player.moveSpeed = 1;
        }
        else {
            player.moveSpeed = 0;
        }
    }
    
}


void T3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void T3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move events
    double alpha = calculateAngle(mouseX);
    player.rotate(alpha);
    textureForward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), (float)alpha, glm::vec3(0, 1, 0)) * glm::vec4(textureForward, 1)));
}


void T3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void T3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void T3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void T3::OnWindowResize(int width, int height)
{
}
