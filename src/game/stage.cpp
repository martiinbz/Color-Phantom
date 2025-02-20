#include "stage.h"
#include "game.h"


#include "stage.h"
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


MenuStage::MenuStage() : Stage() {

}

void MenuStage::render(Camera* camera) {

}

void MenuStage::update(double dt) {

}

void MenuStage::onEnter(Stage* stage) {

}

void MenuStage::onLeave(Stage* stage) {

}
//PLAY STAGE///////////////////////////////////////////

void PlayStage::onEnter(Stage* stage) {
	
}
void PlayStage::onLeave(Stage* stage) {

}




//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;
float mouse_speed = 10.0f;





PlayStage::PlayStage(): Stage() {

}



void PlayStage::render(Camera* camera) {
    
	


	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->enable();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Llamar al render de World en lugar de root->render()
	World::get_instance()->render();

	// Dibujar el grid y estadísticas de FPS
	drawGrid();
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	// Intercambiar buffers
	SDL_GL_SwapWindow(Game::instance->window);
	

	
	

	
}

void PlayStage::update(double seconds_elapsed) {

	World::get_instance()->update(seconds_elapsed);
}