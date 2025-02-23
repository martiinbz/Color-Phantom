#include "stage.h"
#include "game.h"
#include "framework/utils.h"
#include <iostream>
#include "framework/input.h"
#include "game/scene_parser.h"
#include "framework/entities/entity.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "framework/player.h"
#include "framework/world.h"
#include "framework/entities/entity_ui.h"

void MenuStage::init() {
    int width = Game::instance->window_width;
    int height = Game::instance->window_height;

    

    Material background_material;

    //background_material.diffuse = Texture::Get("data/textures/novios.png");
    background = new EntityUI(Vector2(width * 0.5, height * 0.5), Vector2(width, height), background_material);
}

void MenuStage::render(Camera* camera) {
    //background->render(World::get_instance()->camera2D);
    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Un quad del tamaño de la ventana
    



    // boton exit
    if (EntityUI::addButton(Vector2(Game::instance->window_width * 0.7, Game::instance->window_height * 0.7), Vector2(108,48), "data/button/flatDark41.png")) {
        exit(0);
    }

    // boton play
    if (EntityUI::addButton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(494,520), "data/button/flatDark41.png")) {
        Game::instance->goToStage(STAGE_PLAY);
    }
}

void MenuStage::update(double dt) {
    background->update(dt);
}

void MenuStage::onEnter(Stage* stage)
{
    Game::instance->setMouseLocked(false); // Por si en otra escena estaba bloqueado
}

void MenuStage::onLeave(Stage* stage)
{
    Game::instance->setMouseLocked(true);
}


// -------------------------------------
// PLAY STAGE
// -------------------------------------
void PlayStage::init()
{
    // ...
}

void PlayStage::render(Camera* camera)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera->enable();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Renderizamos el mundo
    World::get_instance()->render();

    // Dibujar grid/estadísticas, etc.
    drawGrid();
    drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

    // Intercambiar buffers
    SDL_GL_SwapWindow(Game::instance->window);
}

void PlayStage::update(double seconds_elapsed)
{
    World::get_instance()->update(seconds_elapsed);
}

void PlayStage::onEnter(Stage* stage)
{
    Game::instance->setMouseLocked(true);
}

void PlayStage::onLeave(Stage* stage)
{
    // ...
}
