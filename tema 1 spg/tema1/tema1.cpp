#include "lab_m2/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <cmath>
#include "stb/stb_image.h"

using namespace std;
using namespace m2;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

struct Particle
{
    glm::vec4 position;
    glm::vec4 speed;
    glm::vec4 initialPos;
    glm::vec4 initialSpeed;

    Particle() {}

    Particle(const glm::vec4 &pos, const glm::vec4 &speed)
    {
        SetInitial(pos, speed);
    }

    void SetInitial(const glm::vec4 &pos, const glm::vec4 &speed)
    {
        position = pos;
        initialPos = pos;

        this->speed = speed;
        initialSpeed = speed;
    }
};

ParticleEffect<Particle> *particleEffect;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    t = new Table();
    // debug_table = Table::init();
    // debug_table = t->getTable();

    possible_moves_table = Table::init();

    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(4, 5, 9), glm::quat(glm::vec3(TO_RADIANS * -45, TO_RADIANS * 0, 0)));
    camera->Update();

    std::string texturePath = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube");
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "tema1", "shaders");
    // Load textures

    {
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "particle2.png");
    }

    {
        Shader *shader = new Shader("Particle");
        shader->AddShader(PATH_JOIN(shaderPath, "Particle.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Particle.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Particle.GS.glsl"), GL_GEOMETRY_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Mesh *mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("cube");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for rendering to texture
    {
        Shader *shader = new Shader("CubeMap");
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader *shader = new Shader("CubeMapRed");
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMapRed.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader *shader = new Shader("CubeMapRedPiece");
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMapRedPiece.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader *shader = new Shader("CubeMapPiece");
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMapPiece.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for rendering to texture
    {
        Shader *shader = new Shader("ShaderNormal");
        shader->AddShader(PATH_JOIN(shaderPath, "Normal.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Normal.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    cubeMapTextureID = UploadCubeMapTexture(
        PATH_JOIN(texturePath, "pos_x.png"),
        PATH_JOIN(texturePath, "pos_y.png"),
        PATH_JOIN(texturePath, "pos_z.png"),
        PATH_JOIN(texturePath, "neg_x.png"),
        PATH_JOIN(texturePath, "neg_y.png"),
        PATH_JOIN(texturePath, "neg_z.png"));
}

void Tema1::FrameStart()
{
}

void Tema1::Update(float deltaTimeSeconds)
{
    float frame_number = 0.55 / deltaTimeSeconds;

    if (animation_done == false)
    {
        animating += deltaTimeSeconds;
        if (animating > 0.55)
        {
            animation_done = true;
            t->moveFromTo(animation_start_x, animation_start_y, animation_stop_x, animation_stop_y, t->getPlayer());
            if (animation_type == 2)
            {
                cout << animation_start_x << " " << animation_start_y << " " << animation_stop_x << " " << animation_stop_y;
                int x_attack;
                int y_attack;
                if (t->getPlayer() == 1)
                {
                    x_attack = animation_start_x + 1;
                    if ((animation_start_y - animation_stop_y) < 0)
                    {
                        y_attack = animation_start_y + 1;
                    }
                    else
                    {
                        y_attack = animation_start_y - 1;
                    }
                }
                else
                {
                    x_attack = animation_start_x - 1;
                    if ((animation_start_y - animation_stop_y) < 0)
                    {
                        y_attack = animation_start_y + 1;
                    }
                    else
                    {
                        y_attack = animation_start_y - 1;
                    }
                }
                cout << x_attack << " " << y_attack;
                particleInit(x_attack, y_attack, 0.5);
                showParticle = true;
                particleAnimation = 0;
            }
        }
    }

    ClearScreen();

    auto camera = GetSceneCamera();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the cubemap
    {
        Shader *shader = shaders["ShaderNormal"];
        shader->Use();

        glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
        int loc_texture = shader->GetUniformLocation("texture_cubemap");
        glUniform1i(loc_texture, 0);

        meshes["cube"]->Render();
    }

    float center = 0.5f;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            {
                Shader *shader;
                if ((i + j) % 2)
                {
                    shader = shaders["CubeMapRed"];
                    shader->Use();
                }
                else
                {
                    shader = shaders["CubeMap"];
                    shader->Use();
                }

                glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(1, 0.1, 1));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(center + j, 0, center + i));

                glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
                glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

                auto cameraPosition = camera->m_transform->GetWorldPosition();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
                int loc_texture = shader->GetUniformLocation("texture_cubemap");
                glUniform1i(loc_texture, 0);

                int loc_camera = shader->GetUniformLocation("camera_position");
                glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);

                int loc_id = shader->GetUniformLocation("id");
                glUniform1i(loc_id, t->getTableId(i, j));

                int loc_highlight = shader->GetUniformLocation("highlight");
                glUniform1i(loc_highlight, possible_moves_table[i][j]);

                meshes["box"]->Render();
            }
            {
                if (t->getTable(i, j) == -1)
                {
                    Shader *shader = shaders["CubeMapPiece"];
                    shader->Use();
                    glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.25, 0.5, 0.25));
                    if (i == animation_start_x && j == animation_start_y && animation_done == false)
                    {
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(4 * (j + center + (animation_stop_y - animation_start_y) * animating / 0.55),
                                                                            0.66 + 1.5 * sin(PI * (animating / 0.55)),
                                                                            4 * (i + center + (animation_stop_x - animation_start_x) * animating / 0.55)));
                    }
                    else
                    {
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(4 * (j + center), 0.66, 4 * (i + center)));
                    }

                    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
                    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
                    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

                    auto cameraPosition = camera->m_transform->GetWorldPosition();

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
                    int loc_texture = shader->GetUniformLocation("texture_cubemap");
                    glUniform1i(loc_texture, 0);

                    int loc_camera = shader->GetUniformLocation("camera_position");
                    glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);

                    int loc_id = shader->GetUniformLocation("id");
                    glUniform1i(loc_id, t->getpiecesId(i, j));

                    int loc_highlight = shader->GetUniformLocation("highlight");

                    if (t->getpiecesId(i, j) == selected_piece)
                    {
                        glUniform1i(loc_highlight, 1);
                    }
                    else
                    {
                        glUniform1i(loc_highlight, 0);
                    }

                    meshes["box"]->Render();
                }

                if (t->getTable(i, j) == 1)
                {
                    Shader *shader = shaders["CubeMapRedPiece"];
                    shader->Use();
                    glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.25, 0.5, 0.25));

                    if (i == animation_start_x && j == animation_start_y && animation_done == false)
                    {
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(4 * (j + center + (animation_stop_y - animation_start_y) * animating / 0.55),
                                                                            0.66 + 1.5 * sin(PI * (animating / 0.55)),
                                                                            4 * (i + center + (animation_stop_x - animation_start_x) * animating / 0.55)));
                    }
                    else
                    {
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(4 * (j + center), 0.66, 4 * (i + center)));
                    }
                    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
                    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
                    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

                    auto cameraPosition = camera->m_transform->GetWorldPosition();

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
                    int loc_texture = shader->GetUniformLocation("texture_cubemap");
                    glUniform1i(loc_texture, 0);

                    int loc_camera = shader->GetUniformLocation("camera_position");
                    glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);

                    int loc_id = shader->GetUniformLocation("id");
                    glUniform1i(loc_id, t->getpiecesId(i, j));

                    int loc_highlight = shader->GetUniformLocation("highlight");

                    if (t->getpiecesId(i, j) == selected_piece)
                    {
                        glUniform1i(loc_highlight, 1);
                    }
                    else
                    {
                        glUniform1i(loc_highlight, 0);
                    }

                    meshes["box"]->Render();
                }
            }
        }
    }

    if (showParticle == true)
    {
        particleExplode();
        // cout << particleAnimation << endl;
        if (particleAnimation > 1.5)
            showParticle = false;
        particleAnimation += deltaTimeSeconds;
    }
}

void Tema1::particleInit(float x, float y, float center)
{
    unsigned int nrParticles = 50000;

    particleEffect = new ParticleEffect<Particle>();
    particleEffect->Generate(nrParticles, true);

    auto particleSSBO = particleEffect->GetParticleBuffer();
    Particle *data = const_cast<Particle *>(particleSSBO->GetBuffer());

    int cubeSize = 7;
    int hSize = cubeSize / 2;

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        glm::vec4 pos(1);
        pos.x = (rand() % cubeSize - hSize) / 10.0f + y + center;
        pos.y = (rand() % cubeSize - hSize) / 10.0f;
        pos.z = (rand() % cubeSize - hSize) / 10.0f + x + center;

        glm::vec4 speed(0);
        speed.x = (rand() % 20 - 10) / 120.0f;
        speed.z = (rand() % 20 - 10) / 120.0f;
        speed.y = 1.7; // rand() % 2 + 2.0f;

        data[i].SetInitial(pos, speed);
    }

    particleSSBO->SetBufferData(data);
}

void Tema1::particleExplode()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    {
        auto shader = shaders["Particle"];
        if (shader->GetProgramID())
        {
            shader->Use();
            TextureManager::GetTexture("particle2.png")->BindToTextureUnit(GL_TEXTURE0);
            particleEffect->Render(GetSceneCamera(), shader);
        }
    }

    glDisable(GL_BLEND);
}

void Tema1::FrameEnd()
{
    // DrawCoordinateSystem();
}

unsigned int Tema1::UploadCubeMapTexture(const std::string &pos_x, const std::string &pos_y, const std::string &pos_z, const std::string &neg_x, const std::string &neg_y, const std::string &neg_z)
{
    int width, height, chn;

    unsigned char *data_pos_x = stbi_load(pos_x.c_str(), &width, &height, &chn, 0);
    unsigned char *data_pos_y = stbi_load(pos_y.c_str(), &width, &height, &chn, 0);
    unsigned char *data_pos_z = stbi_load(pos_z.c_str(), &width, &height, &chn, 0);
    unsigned char *data_neg_x = stbi_load(neg_x.c_str(), &width, &height, &chn, 0);
    unsigned char *data_neg_y = stbi_load(neg_y.c_str(), &width, &height, &chn, 0);
    unsigned char *data_neg_z = stbi_load(neg_z.c_str(), &width, &height, &chn, 0);

    // TODO(student): Create the texture
    unsigned int textureID = 0;
    glGenTextures(1, &textureID);
    // TODO(student): Bind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (GLEW_EXT_texture_filter_anisotropic)
    {
        float maxAnisotropy;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // TODO(student): Load texture information for each face

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_z);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_z);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
    }

    // Free memory
    SAFE_FREE(data_pos_x);
    SAFE_FREE(data_pos_y);
    SAFE_FREE(data_pos_z);
    SAFE_FREE(data_neg_x);
    SAFE_FREE(data_neg_y);
    SAFE_FREE(data_neg_z);

    return textureID;
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}

void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_Q)
    {
        t->PrintTable(t->getTableId());
        t->PrintTable(t->getpiecesId());
    }
    if (key == GLFW_KEY_Z)
    {
        particleInit(1, 1, 0.5);
        showParticle = true;
        particleAnimation = 0;
    }
    // Add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

    if (button == 1 && animation_done == true)
    {
        unsigned char data[4];

        int y = window->props.resolution.y - mouseY;

        glReadPixels(mouseX, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        int id = data[3];
        if (!t->isIdFromPiece(id))
        {
            // cout << "Click pe tabla!";
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (possible_moves_table[i][j] == 1 && t->getTableId(i, j) == id)
                    {
                        // cout << "Miscare!";
                        pair<int, int> coords = t->getCoordsFromPiece(selected_piece);

                        animation_start_x = (float)coords.first;
                        animation_start_y = (float)coords.second;
                        animation_stop_x = (float)i;
                        animation_stop_y = (float)j;

                        animation_type = t->typeMoveFromTo(coords.first, coords.second, i, j, t->getPlayer());

                        if (animation_type > 0)
                        {
                            animation_done = false;
                            animating = 0;
                        }
                    }
                }
            }
        }

        // one block
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                possible_moves_table[i][j] = 0;
            }
        }
        t->getPossibleMovesFromId(id, possible_moves_table);
        // end block

        selected_piece = id;
    }

    // Add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}

void Tema1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
