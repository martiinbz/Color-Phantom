#include "game.h"
#include "framework/utils.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/fbo.h"
#include "graphics/shader.h"
#include "graphics/material.h"
#include "framework/input.h"
#include "framework/entities/entity_mesh.h"
#include "game/stage.h"
#include <cmath>
#include "framework/world.h"

//some globals

Stage* current_stage = NULL;
Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	setMouseLocked(true);

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;
	
	stages[STAGE_INTRO] = new IntroStage();
	stages[STAGE_MENU] = new MenuStage();
	stages[STAGE_TUTORIAL] = new TutorialStage();
	stages[STAGE_L1] = new L1Stage();
	stages[STAGE_L2] = new L2Stage();
	stages[STAGE_L3] = new L3Stage();

	for (auto entry : stages) {
		int id = entry.first;
		Stage* stage = entry.second;
		stage->init();
	}

	goToStage(STAGE_INTRO);

	// OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	SDL_ShowCursor(!mouse_locked);
}

//what to do when the image has to be draw
void Game::render(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (current_stage)
		current_stage->render(World::get_instance()->camera);

	// drawText(2, 2, getGPUStats(), Vector3(1,1,1), 2);
	SDL_GL_SwapWindow(window);
}

void Game::update(double seconds_elapsed)
{
	if (current_stage)
		current_stage->update(seconds_elapsed);
}

void Game::goToStage(uint8_t stage_id) {
	Stage* new_stage = stages[stage_id];
	assert(new_stage);
	switch (stage_id) {
		case STAGE_L1:
			level = 1;
			break;
		case STAGE_L2:
			level = 2;
			break;
		case STAGE_L3:
			level = 3;
			break;
		default:
			break; // No cambiar level para otras escenas
		}
	if (current_stage)
		current_stage->onLeave(new_stage);
	new_stage->onEnter(current_stage);
	current_stage = new_stage;
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
		SDL_SetRelativeMouseMode((SDL_bool)(mouse_locked));
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{

}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Game::setMouseLocked(bool must_lock)
{
	SDL_ShowCursor(!must_lock);

	SDL_SetRelativeMouseMode((SDL_bool)must_lock);

	mouse_locked = must_lock;
}

