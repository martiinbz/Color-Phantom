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

extern int level;


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
	camera2D->setOrthographic(0, window_width, window_height, 0, -1, 1);


	// parent root
	root = new Entity();

	
	if (!Player::instance) {
		Player::instance = new Player();
	}
	
	SceneParser parser;
	int current_level = Game::instance->level; // Obtener el nivel desde `Game`
	const char* scene_path = nullptr;
	std::cout << current_level << std::endl;
	switch (current_level) {

	case 0:
		std::cout << "Tutorial" << std::endl;
		scene_path = "data/tutorial.scene";
		break;
	case 1:
		std::cout << "Supermarket" << std::endl;
		scene_path = "data/supermarket.scene";
		break;
	case 2:
		scene_path = "data/factory.scene";
		break;
	case 3:
		scene_path = "data/graveyard.scene";
		break;
	}
	bool ok = parser.parse(scene_path, root);

	starting_position = Player::instance->model.getTranslation(); //para resetear al jugador si se bugea
	
	
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
	
	
	
	//crosshair, solo en primera persona y del color negativo al que se  está mirando
	if (use_first_person) {
		drawText(Game::instance->window_width / 2, Game::instance->window_height / 2 - 5, "o", Vector3(1 - looking_color.x, 1 - looking_color.y, 1 - looking_color.z), 2);
	}
	
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
			use_first_person = true;
			update_fpcamera(seconds_elapsed);
			//mira (en negativo al color que apuntas para que siempre se vea)

		}
		else {
			use_first_person = false;
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


	if (Input::isKeyPressed(SDL_SCANCODE_O) ){
		Player::instance->model.setTranslation(starting_position);

	}
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
	Vector3 right = (mYaw.rightVector()).normalize(); // Para desplazar la cámara al lado

	// Obtener la escala del jugador
	float player_scale = Player::instance->model.m[5]; // Escala en Y

	// Ajustar la altura de la cámara en función de la escala del jugador
	float base_height = 1.0f;
	float adjusted_height = base_height * player_scale;

	// Posición del jugador
	Vector3 center = Player::instance->model.getTranslation() + Vector3(0, adjusted_height, 0);

	
	Vector3 eye = center - front * player_scale + right * player_scale;

	//// Detectar colisiones con el entorno para que la cámara no atraviese objetos
	//sCollisionData data = raycast(center, (eye - center).normalize());
	//if (data.collided) {
	//	float smoothing_factor = 0.3f;
	//	eye = eye * (1 - smoothing_factor) + data.col_point * smoothing_factor;
	//}

	// Aplicar la posición de la cámara
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

	// Obtener el tamaño del jugador
	float player_scale = Player::instance->model.m[5]; // La escala en Y

	// Ajustar la altura de la cámara en función de la escala del jugador
	float base_height = 1.5f; // Altura base de la cámara respecto al jugador
	float adjusted_height = base_height * player_scale; // Ajuste proporcional

	// Calcular la nueva posición de la cámara
	Vector3 center = Player::instance->model.getTranslation() + Vector3(0, adjusted_height, 0);
	float orbit_distance = 3.0f * player_scale; // Ajustar la distancia en función del tamaño

	Vector3 eye = center - front * orbit_distance;

	// Detectar colisiones con el entorno
	sCollisionData data = raycast(center, (eye - center).normalize());
	if (data.collided) {
		float smoothing_factor = 0.3f;
		eye = eye * (1 - smoothing_factor) + data.col_point * smoothing_factor;
	}

	// Aplicar la posición de la cámara
	camera->lookAt(eye, center, Vector3(0, 1, 0));
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
