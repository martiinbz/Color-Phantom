#include "framework/camera.h"
#include "game/game.h"
#include "game/scene_parser.h"
#include "framework/input.h"
#include "framework/player.h"
#include "framework/world.h"

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
	GLfloat color[3];
	glReadPixels(camera->eye.x, camera->eye.y, 1, 1, GL_RGB, GL_FLOAT, color);
	std::cout << "Color: " << color[0] << " " << color[1] << " " << color[2] << std::endl;


	glEnable(GL_DEPTH_TEST);

	player->render(camera);

	root->render(camera);

	
}

void World::update(double seconds_elapsed) {

	
	if (free_camera) {
		// Movimiento de cámara libre
		float speed = seconds_elapsed * camera_speed;

		if (Input::isMousePressed(SDL_BUTTON_LEFT) || Game::instance->mouse_locked) {
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.f, -1.f, 0.f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.f, 0.f, 0.f)));
		}

		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10;
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else {
		// Actualizar el mundo y el player
		if (root) root->update(seconds_elapsed);
		if (player) player->update(seconds_elapsed);

		// Actualizar la cámara en tercera persona o primera persona
		if (use_first_person) {
			update_fpcamera(seconds_elapsed);
		}
		else {
			update_thirdpcamera(seconds_elapsed);
		}
	}
}

void World::addEntity(Entity* entity) {
	root->addChild(entity);
}

void World::destroyEntity(Entity* entity) {
	entities_to_destroy.push_back(entity);
}

void World::update_fpcamera(float seconds_elapsed) {

	camera_yaw -= Input::mouse_delta.x * seconds_elapsed * mouse_speed;
	camera_pitch -= Input::mouse_delta.y * seconds_elapsed * mouse_speed;

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	Matrix44 mPitch;
	mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0));

	Vector3 front = (mPitch * mYaw).frontVector().normalize();

	//put the camera in front of the player
	Vector3 eye = player->model.getTranslation() + Vector3(0, 0.5, 0) - front * 0.5;
	Vector3 center = eye + front;

	camera->lookAt(eye, center, Vector3(0, 1, 0));
}

void World::update_thirdpcamera(float seconds_elapsed) {

	camera_yaw -= Input::mouse_delta.x * seconds_elapsed * mouse_speed;
	camera_pitch -= Input::mouse_delta.y * seconds_elapsed * mouse_speed;
	camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f); // restrict angle

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	Matrix44 mPitch;
	mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0));

	Vector3 front = (mPitch * mYaw).frontVector().normalize();

	//put the camera in behind of the player and keep the orbit always looking at the player
	Vector3 center = player->model.getTranslation() + Vector3(0, 0.5, 0);
	float orbit_distance = 1.5;
	Vector3 eye = player->model.getTranslation() - front * orbit_distance;

	//esto es para que la camara de tercera persona no atraviese las paredes
	/*
	{
		sCollisionData data = raycast(center, (camera->eye - center).normalize());
		if (data.collided)
			eye = data.col_point;

	}*/


	
	camera->lookAt(eye, center, Vector3(0, 1, 0));
}

/*sCollisionData World::raycast(const Vector3& origin, const Vector3& direction, int layer) {

	sCollisionData data;

	for (auto e : root->children) {
		EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
		if (ec == nullptr || !(ec->layer & layer)){
			continue;
		}

		Vector3 col_point;
		Vector3 col_normal;



	}


}*/
