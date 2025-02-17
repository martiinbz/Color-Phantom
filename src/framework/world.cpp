#include "framework/camera.h"
#include "game/game.h"
#include "game/scene_parser.h"
#include "framework/input.h"
#include "framework/player.h"
#include "../../build/world.h"


World* World::instance = nullptr;

World::World() {
	int window_width = Game::instance->window_width;
	int window_height = Game::instance->window_height;

	// normal camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 1.f, 1.f), Vector3(0.f), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(70.f, window_width / float(window_height), 0.1f, 1000.f);

	// camera 2D
	camera2D = new Camera();
	// ...

	// parent root
	root = new Entity();

	// player init 
	player = new Player();
	// ...

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

	player->render(camera);

	root->render(camera);
}

void World::update(double seconds_elapsed) {
	if (free_camera) {
		float speed = (float)seconds_elapsed * camera_speed;

		if (Input::isMousePressed(SDL_BUTTON_LEFT) || Game::instance->mouse_locked) {
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.f, -1.f, 0.f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.f, 0.f, 0.f)));
		}

		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) Game::instance->camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) Game::instance->camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) Game::instance->camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) Game::instance->camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else {
		root->update(seconds_elapsed);
		player->update(seconds_elapsed);

		camera_yaw -= Input::mouse_delta.x * seconds_elapsed * mouse_speed;
		camera_pitch -= Input::mouse_delta.y * seconds_elapsed * mouse_speed;

		camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f); // restrict angle

		Matrix44 mYaw;
		mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

		Matrix44 mPitch;
		mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0));

		Vector3 front = (mPitch * mYaw).frontVector().normalize();
		Vector3 eye;
		Vector3 center;

		if (use_first_person) {
			eye = player->model.getTranslation() + Vector3(0.f, 0.5f, 0.f) + front * 0.1f;
			center = eye + front;
		}
		else {
			float orbit_dist = 1.5f;
			eye = player->model.getTranslation() - front * orbit_dist;
			center = player->model.getTranslation() + Vector3(0.f, 0.5f, 0.f);
		}
		camera->lookAt(eye, center, Vector3(0, 1, 0));
	}

	for (auto e : entities_to_destroy) {
		root->removeChild(e);
		delete e;
	}
}

void World::addEntity(Entity* entity) {
	root->addChild(entity);
}

void World::destroyEntity(Entity* entity) {
	entities_to_destroy.push_back(entity);
}