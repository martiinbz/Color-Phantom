#pragma once
#include "framework/entities/entity_mesh.h"

class Player : public EntityMesh {

	float walk_speed = 0.0f;
	Vector3 velocity = Vector3(0.0f);

public:
	Player() {};
	Player(Mesh* mesh, const Material& material, const std::string& name = "");
	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
};