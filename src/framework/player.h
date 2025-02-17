#pragma once
#include "framework/entities/entity_mesh.h"

class Player : public EntityMesh {


public:
	Player();
	void render(Camera* camera);
	void update(double seconds_elapsed);

};