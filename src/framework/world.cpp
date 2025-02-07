#include "world.h"
#include "framework/camera.h"
#include "game/game.h"
#include "game/scene_parser.h"
#include "framework/input.h"


World* World::instance = nullptr;

World::World() {
	int window_width = Game::instance->window_width;
	int window_height = Game::instance->window_height;

	// set camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 1.f, 1.f), Vector3(0.f), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(70.f, window_width / float(window_height), 0.1f, 1000.f);

	// parent root
	root = new Entity();

	SceneParser parser;
	bool ok = parser.parse("data/myscene.scene", root);
}

void World::render() {
	camera->enable();

	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	/* si hacemos SKYBOX
	glDisable(GL_DEPTH_TEST);
	skybox->render(camera);
	glEnable(GL_DEPTH_TEST);
	*/

	glEnable(GL_DEPTH_TEST);

	// player->render(camera);

	root->render(camera);
}

void World::update(double seconds_elapsed) {
	if (free_camera) {
		float speed = (float)seconds_elapsed * camera_speed;

		if (Input::isMousePressed(SDL_BUTTON_LEFT) || Game::instance->mouse_locked) {
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.f, -1.f, 0.f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.f, 0.f, 0.f)));
		}
	}
}

void World::addEntity(Entity* entity) {
	root->addChild(entity);
}

void World::destroyEntity(Entity* entity) {
	entities_to_destroy.push_back(entity);
}