#pragma once
#include "framework/entities/entity.h"
#include "framework/entities/entity_mesh.h"
#include "framework/player.h"
#include "framework/entities/entity_collider.h"

class World {
	static World* instance;

public:

	static World* get_instance() {
		if (instance != nullptr)
			return instance;
		instance = new World();
		return instance;
	}

	World();

	Entity* root = nullptr;

	EntityMesh* skybox = nullptr;
	
	Camera* camera = nullptr;
	Camera* camera2D = nullptr;
	
	Vector3 looking_color;
	float camera_yaw = 0.f;
	float camera_pitch = 0.f;
	float camera_speed = 2.f;
	float mouse_speed = 0.5f;

	bool free_camera = false;
	bool use_first_person = false;
	bool use_third_person = true;

	void render();
	void update(double seconds_elapsed);

	std::vector<Entity*> entities_to_destroy;

	void addEntity(Entity* entity);
	void destroyEntity(Entity* entity);
	void update_fpcamera(float seconds_elapsed);
	void update_thirdpcamera(float seconds_elapsed);

	sCollisionData raycast(const Vector3& origin, const Vector3& direction, int layer = eCollisionFilter::ALL, bool closest = true, double mad_ray_distance = 3.4);
	void test_scene_collisions(const Vector3& position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions);

};