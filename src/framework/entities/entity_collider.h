#pragma once
#include "framework/entities/entity_mesh.h"


enum eCollisionFilter {
	NONE = 0, FLOR = 1 << 0, WALL = 1 << 1, PLAYER = 1 << 2,
	ENEMY = 1 << 3, BULLET = 1 << 4, PICKUP = 1 << 5,
	ALL = 0xFFFFFFFF
};

struct sCollisionData {
	Vector3 col_point;
	Vector3 col_normal;
	float distance = 1e10f;
	bool collided = false;
	EntityCollider* collider = nullptr;
};

class EntityCollider : public EntityMesh {
	void getCollisionsWithModel(const Matrix44& m, const Vector3& center, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions);

public:

	EntityCollider() {};
	EntityCollider(Mesh* mesh, const Material& material, const std::string name) :
		EntityMesh(mesh, material) {};

	bool is_dynamic = false;
	void getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions);

};