#include "framework/camera.h"
#include "game/game.h"
#include "game/scene_parser.h"
#include "framework/input.h"
#include "framework/player.h"
#include "framework/world.h"
#include "framework/entities/entity_collider.h"
#include "graphics/mesh.h"
#include "framework/entities/entity_mesh.h"

World* World::instance = nullptr;

World::World() {
	int window_width = Game::instance->window_width;
	int window_height = Game::instance->window_height;

	// normal camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 1.f, 1.f), Vector3(0.f), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(70.f, window_width / float(window_height), 0.1f, 1000.f);


	// 2D camera
	camera2D = new Camera();
	camera2D->view_matrix.setIdentity();
	camera2D->setOrthographic(0, window_width, window_height, 0, -1, -1);


	// parent root
	root = new Entity();

	
	if (!Player::instance) {
		Player::instance = new Player();
	}

	SceneParser parser;
	bool ok = parser.parse("data/myscene.scene", root);
}

void World::render() {
	camera->enable();
	camera2D->enable();
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	/* si hacemos SKYBOX
	glDisable(GL_DEPTH_TEST);
	skybox->render(camera);
	glEnable(GL_DEPTH_TEST);
	*/

	glEnable(GL_DEPTH_TEST);

	root->render(camera);
	
	glReadPixels(Game::instance->window_width/2, Game::instance->window_height/2, 1, 1, GL_RGB, GL_FLOAT, &looking_color.x);
	
	drawText(5, 15, "LOOKING_COLOR", looking_color, 4);

	//mira (en negativo al color que apuntas para que siempre se vea)
	drawText(Game::instance->window_width / 2, Game::instance->window_height / 2-5, "o", Vector3(1-looking_color.x, 1 - looking_color.y, 1 - looking_color.z), 2);
}

void World::update(double seconds_elapsed) {
	
	
	//update the scene
	if (root) {
		root->update(seconds_elapsed);
	}

	if (Player::instance) {
			
		Player::instance->update(seconds_elapsed);

		//si se hace click, se cambia la cámara
		if (Input::isMousePressed(SDL_BUTTON_LEFT)) {
			
			update_fpcamera(seconds_elapsed);
		}
		else {
			update_thirdpcamera(seconds_elapsed);
		}
	}
	else {
		std::cout << "ERROR: Player es nullptr. No se puede actualizar ni seguir con la cámara." << std::endl;
	}
	

	
	for (auto e : entities_to_destroy) {
		root->removeChild(e);
		delete e;
	}
	entities_to_destroy.clear();
}

void World::addEntity(Entity* entity) {
	root->addChild(entity);
}

void World::destroyEntity(Entity* entity) {
	entities_to_destroy.push_back(entity);
}

//realmente no es primera persona como tal
void World::update_fpcamera(float seconds_elapsed) {
	
	camera_yaw -= Input::mouse_delta.x * seconds_elapsed * mouse_speed;
	camera_pitch -= Input::mouse_delta.y * seconds_elapsed * mouse_speed;

	camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0)); 

	Matrix44 mPitch;
	mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0)); 

	Vector3 front = (mPitch * mYaw).frontVector().normalize();

	
	Vector3 center = Player::instance->model.getTranslation() + Vector3(0.5f, 1.5f, 0);  

	
	float orbit_distance = 1.3f; 
	Vector3 eye = center - front * orbit_distance; 

	
	camera->lookAt(eye, center + front * 2.0f, Vector3(0, 1, 0));  

	//detectar colisiones
	sCollisionData data = raycast(center, (eye - center).normalize());

	if (data.collided) {
		//suavizar la transición para que la camara no de saltos bruscos
		float smoothing_factor = 0.3f;
		eye = eye * (1 - smoothing_factor) + data.col_point * smoothing_factor;
	}

	
	camera->lookAt(eye, center + front * 2.0f, Vector3(0, 1, 0));  
}


void World::update_thirdpcamera(float seconds_elapsed) {
	
	camera_yaw -= Input::mouse_delta.x * seconds_elapsed * mouse_speed;
	camera_pitch -= Input::mouse_delta.y * seconds_elapsed * mouse_speed;

	camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	Matrix44 mPitch;
	mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0));

	Vector3 front = (mPitch * mYaw).frontVector().normalize();

	
	Vector3 center = Player::instance->model.getTranslation() + Vector3(0,1.5, 0);
	float orbit_distance = 2.0f; 
	Vector3 eye = center - front * orbit_distance;
	
	//colisiones
	sCollisionData data = raycast(center, (eye - center).normalize());
	if (data.collided) {
		
		float smoothing_factor = 0.3f; 
		eye = eye * (1 - smoothing_factor) + data.col_point * smoothing_factor;
		camera->lookAt(eye, center, Vector3(0, 1, 0));
		
	}
	else {
		camera->lookAt(eye, center, Vector3(0, 1, 0));
	}
}

void World::test_scene_collisions(const Vector3& position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions) {
	for (auto e : root->children) {
		EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
		// si no es un collider no hacemos nada
		if (ec == nullptr)
			continue;
		// si es collider las guarda en los vectores de colisiones
		ec->getCollisions(position, collisions, ground_collisions);
	}
}

sCollisionData World::raycast(const Vector3& origin, const Vector3& direction, int layer, bool closest, double mad_ray_distance) {

	sCollisionData data;

	for (auto e : root->children) {
		EntityCollider* ec = dynamic_cast<EntityCollider*>(e);
		if (ec == nullptr || !(ec->layer & layer)){
			continue;
		}

		Vector3 col_point;
		Vector3 col_normal;

		if (!ec->isInstanced) {
			if (!ec->mesh->testRayCollision(ec->model,origin,direction,col_point,col_normal,mad_ray_distance,closest))
				continue;

			float new_distance = (col_point - origin).length();
			if (new_distance < data.distance) {
				data = { col_point, col_normal, new_distance, true, ec };
			}

			if (!closest)
				return data;
		}
		else {
			for (const Matrix44& model : ec->models) {
				if (!ec->mesh->testRayCollision(model, origin, direction, col_point, col_normal, mad_ray_distance, closest))
					continue;

				float new_distance = (col_point - origin).length();
				if (new_distance < data.distance) {
					data = { col_point, col_normal, new_distance, true, ec };
				}

				if (!closest)
					return data;
			}
		}
	}
	return data;
}
