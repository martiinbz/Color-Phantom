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




//PLAY STAGE///////////////////////////////////////////



//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;
float mouse_speed = 10.0f;
Entity* root = new Entity();


EntityMesh* entity_mesh = nullptr;


PlayStage::PlayStage(): Stage() {


	SceneParser parser;
	parser.parse("data/myscene.scene", root);


	// Load one texture using the Texture Manager
	texture = Texture::Get("data/textures/texture.tga");

	// Example of loading Mesh from Mesh Manager
	mesh = Mesh::Get("data/meshes/box.ASE");

	// Example of shader loading using the shaders manager
	shader= Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	Material material;
	entity_mesh = new EntityMesh(mesh, material);
}



void PlayStage::render(Camera* camera) {
    
	// Set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera as default
	camera->enable();

	// Set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Create model matrix for cube
	Matrix44 m;
	m.rotate(angle * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));

	//if (shader)
	//{
	//	// Enable shader
	//	shader->enable();
	//	// Upload uniforms
	//	/*shader->setUniform("u_color", Vector4(0, 1, 1, 1));
	//	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//	shader->setUniform("u_texture", texture, 0);
	//	shader->setUniform("u_model", m);
	//	shader->setUniform("u_time", time);*/

	//	// Do the draw call
	//	mesh->render(GL_TRIANGLES);
	//	
	//	// Disable shader
	//	shader->disable();
	//}
	root->render(camera);
	// Draw the floor grid
	drawGrid();

		
	// Render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	// Swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);

	
}

void PlayStage::update(double seconds_elapsed) {
	
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	// Example
	angle += (float)seconds_elapsed * 10.0f;

	// Mouse input to rotate the cam
	if (Input::isMousePressed(SDL_BUTTON_LEFT) || Game::instance->mouse_locked) //is left button pressed?
	{
		Game::instance->camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		Game::instance->camera->rotate(Input::mouse_delta.y * 0.005f, Game::instance->camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	// Async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) Game::instance->camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) Game::instance->camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) Game::instance->camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) Game::instance->camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
}