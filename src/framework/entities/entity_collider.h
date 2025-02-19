#pragma once
#include "framework/entities/entity_mesh.h"


enum {
	NONE = 0, FLOR = 1 << 0, WALL = 1 << 1, PLAYER = 1 << 2,
	ENEMY = 1 << 3, BULLET = 1 << 4, PICKUP = 1 << 5, 
	ALL = 0xFFFFFFFF

class EntityCollider : public EntityMesh {
public:

	bool is_dynamic = false;
	void get