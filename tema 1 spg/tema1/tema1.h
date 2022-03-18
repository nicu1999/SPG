#pragma once
#include <string>
#include "components/simple_scene.h"
#include "components/transform.h"
#include "Table.h"
#include "core/gpu/particle_effect.h"
#define PI 3.14159265

namespace m2
{

    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        unsigned int UploadCubeMapTexture(const std::string &pos_x, const std::string &pos_y, const std::string &pos_z, const std::string &neg_x, const std::string &neg_y, const std::string &neg_z);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void particleExplode();
        void particleInit(float x, float y, float center);
        void RenderMeshInstanced(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int instances, const glm::vec3 &color);

    private:
        int cubeMapTextureID;
        int **debug_table;
        Table *t;
        int debug = 0;
        int **possible_moves_table;
        int selected_piece;
        bool animation = false;
        float animating = 0; //
        bool animation_done = true;

        float animation_start_x;
        float animation_start_y;
        float animation_stop_x;
        float animation_stop_y;

        int animation_type; // 1 == move, 2 == attack

        float particleAnimation = 0;
        bool showParticle = false;

        int no_of_generated_points = 10;
        int no_of_instances = 50;
        float max_rotate = glm::radians(360.0f);
    };
} // namespace m2
