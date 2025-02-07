#pragma once
#include "framework/entities/entity.h"
#include "framework/entities/entity_mesh.h"


class World {
	static World* instance;

public:

	static World* get_instance() {
		if (instance != nullptr);
			return instance;
		instance = new World();
		return instance;
	}

	World();

	Entity* root = nullptr;

	EntityMesh* skybox = nullptr;
	// Player* player = nullptr;

	Camera* camera = nullptr;

	float camera_yaw = 0.f;
	float camera_pitch = 0.f;
	float camera_speed = 2.f;
	float mouse_speed = 0.f;

	bool free_camera = true;
	bool use_first_person = false;

	void render();
	void update(double seconds_elapsed);

	std::vector<Entity*> entities_to_destroy;

	void addEntity(Entity* entity);
	void destroyEntity(Entity* entity);
};