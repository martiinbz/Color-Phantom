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

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;
	
	stages[STAGE_MENU] = new MenuStage();
	stages[STAGE_PLAY] = new PlayStage();

	for (auto entry : stages) {
		int id = entry.first;
		Stage* stage = entry.second;
		stage->init();
	}

	goToStage(STAGE_MENU);

	// OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	// Create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,1.f, 1.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	SDL_ShowCursor(!mouse_locked);
}

//what to do when the image has to be draw
void Game::render(void)
{
	if (current_stage)
		current_stage->render(camera);

	drawText(2, 2, getGPUStats(), Vector3(1,1,1), 2);
}

void Game::update(double seconds_elapsed)
{
	if (current_stage)
		current_stage->update(seconds_elapsed);
}

void Game::goToStage(uint8_t stage_id) {
	Stage* new_stage = stages[stage_id];
	assert(new_stage);

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
		case SDLK_z: Game::instance->goToStage(STAGE_PLAY); break;
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
	//mouse_speed *= event.y > 0 ? 1.1f : 0.9f;
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

