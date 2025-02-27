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

   
}

void MenuStage::render(Camera* camera) {
    // fondo de pantalla
    UI::addbackground(Vector2(Game::instance->window_width*0.5, Game::instance->window_height*0.5), Vector2(1000,1000), "data/button/blue.png");
       
    // boton exit
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(112,112), "data/button/Play@2X.png")) {
        Game::instance->goToStage(STAGE_PLAY);
    }

    // boton play
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.8), Vector2(112,112), "data/button/Exit@2X.png")) {
        exit(0);
    }
}

void MenuStage::update(double dt) {
   
}

void MenuStage::onEnter(Stage* stage)
{
    Game::instance->setMouseLocked(false); // por si en otra escena estaba bloqueado
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
 
    camera->enable();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

   
    World::get_instance()->render();

    drawGrid();
    drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
    Vector3 color = Player::instance->current_color;
	std::cout << "color: " << color.x << " " << color.y << " " << color.z << std::endl; 
    if (color.x==0 && color.y==0 && color.z==0) {
		std::cout << "has ganado!" << std::endl;

    }
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
    
}
