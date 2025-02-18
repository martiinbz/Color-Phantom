#include "framework/player.h"
#include "framework/world.h"
#include "framework/input.h"

Player::Player() {
	
}

Player::Player(Mesh* mesh, const Material& material, const std::string& name) 
	: EntityMesh(mesh, material) {
	walk_speed = 1.0f;
}

void Player::render(Camera* camera) {
	EntityMesh::render(camera);
}

void Player::update(float seconds_elapsed) {
	float camera_yaw = World::get_instance()->camera_yaw;

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	Vector3 front = mYaw.frontVector();
	Vector3 right = mYaw.rightVector();
	Vector3 position = model.getTranslation();
	Vector3 move_dir;

	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP))
		move_dir += front;
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))
		move_dir -= front;

	float speed_mult = walk_speed;
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
		speed_mult *= 3.0f;

	move_dir.normalize();
	move_dir *= speed_mult;

	position += velocity * seconds_elapsed;

	velocity.x *= 0.5f;
	velocity.y *= 0.5f;

	model.setTranslation(position);
	model.rotate(camera_yaw, Vector3(0, 1, 0));

	EntityMesh::update(seconds_elapsed);
}