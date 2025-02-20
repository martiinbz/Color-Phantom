#pragma once
#include "framework/entities/entity.h"
#include "framework/entities/entity_mesh.h"

class World {
	static World* instance;

public:

	struct sCollisionData {
		Vector3 col_point;
		Vector3 col_normal;
		float distance = 1e10f;
		bool collided = false;
		//Entitycollider* collider = nullptr;
	};

	static World* get_instance() {
		if (instance != nullptr)
			return instance;
		instance = new World();
		return instance;
	}

	World();

	Entity* root = nullptr;

	EntityMesh* skybox = nullptr;
	Player* player = nullptr;

	Camera* camera = nullptr;
	Camera* camera2D = nullptr;

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
	//sCollisionData raycast(const Vector3& origin, const Vector3& direction, int layer =0 /*eCollisionFilter::ALL*/, bool closest = true, float mad_ray_distance = 3.4);

};