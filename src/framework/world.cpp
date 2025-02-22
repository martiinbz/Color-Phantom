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

	

	// parent root
	root = new Entity();

	// Si la instancia de Player aún no está creada, crearla
	if (!Player::instance) {
		Player::instance = new Player();
	}

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

	root->render(camera);
	
	glReadPixels(Game::instance->window_width/2, Game::instance->window_height/2, 1, 1, GL_RGB, GL_FLOAT, &looking_color.x);
	
	drawText(5, 15, "COLOR", looking_color, 4);
}

void World::update(double seconds_elapsed) {
	// Verificamos si la cámara está en modo libre
	free_camera = false;
	
	
	
	
	// Si no estamos en free_camera, actualizamos la escena y el jugador
	if (root) {
		root->update(seconds_elapsed);
	}

	if (Player::instance) {
			
		Player::instance->update(seconds_elapsed);

		// Asegurar que la cámara sigue al Player en tercera o primera persona
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
	

	// Destruir entidades marcadas para eliminación
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


void World::update_fpcamera(float seconds_elapsed) {
	// Ajustar la orientación del jugador (y no de la cámara) con el movimiento del ratón
	camera_yaw -= Input::mouse_delta.x * seconds_elapsed * mouse_speed;
	camera_pitch -= Input::mouse_delta.y * seconds_elapsed * mouse_speed;

	// Limitar la inclinación de la cámara
	camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);

	// Crear la matriz de rotación para el jugador (pero no para la cámara)
	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));  // Rotación en el eje Y (horizontal)

	Matrix44 mPitch;
	mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0));  // Rotación en el eje X (vertical)

	// Dirección que la cámara sigue (del jugador)
	Vector3 front = (mPitch * mYaw).frontVector().normalize();

	// La cámara siempre debe estar frente al jugador, no se ve afectada por la tercera persona
	Vector3 center = Player::instance->model.getTranslation() + Vector3(0, 1.5f, 0);  // Centro del jugador

	// Colocamos la cámara a una distancia fija frente al jugador
	float orbit_distance = 1.0f;  // Distancia de la cámara respecto al jugador
	Vector3 eye = center - front * orbit_distance; // La cámara está siempre frente al jugador

	// La cámara debe mirar hacia donde el jugador está mirando
	camera->lookAt(eye, center + front * 2.0f, Vector3(0, 1, 0));  // La cámara sigue al jugador

	// Si se detecta una colisión (por ejemplo, una pared entre el jugador y la cámara), ajustamos la posición de la cámara
	sCollisionData data = raycast(center, (eye - center).normalize());

	if (data.collided) {
		// Suavizamos el ajuste de la cámara para evitar que se mueva bruscamente
		float smoothing_factor = 0.3f;  // Ajusta este factor según lo suave que desees que sea el movimiento
		eye = eye * (1 - smoothing_factor) + data.col_point * smoothing_factor;
	}

	// Finalmente, actualizamos la cámara para que mire al jugador desde la posición calculada
	camera->lookAt(eye, center + front * 2.0f, Vector3(0, 1, 0));  // La cámara sigue al jugador
}







void World::update_thirdpcamera(float seconds_elapsed) {
	// Ajustar sensibilidad del ratón
	camera_yaw -= Input::mouse_delta.x * seconds_elapsed * mouse_speed;
	camera_pitch -= Input::mouse_delta.y * seconds_elapsed * mouse_speed;

	// Limitar la inclinación de la cámara
	camera_pitch = clamp(camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);

	Matrix44 mYaw;
	mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

	Matrix44 mPitch;
	mPitch.setRotation(camera_pitch, Vector3(-1, 0, 0));

	Vector3 front = (mPitch * mYaw).frontVector().normalize();

	// Ajustar la posición de la cámara
	Vector3 center = Player::instance->model.getTranslation() + Vector3(0,1.5, 0);
	float orbit_distance = 2.0f;  // Ajusta la distancia de la cámara
	Vector3 eye = center - front * orbit_distance;
	
	// Para que la camara no atraviese las paredes
	sCollisionData data = raycast(center, (eye - center).normalize());
	if (data.collided) {
		
		float smoothing_factor = 0.3f;  // Puedes ajustar esto para hacerlo más suave
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
