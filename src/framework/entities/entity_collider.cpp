#include "entity_collider.h"
#include "framework/world.h"

void EntityCollider::getCollisionsWithModel(const Matrix44& m, const Vector3& center, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions) {
	Vector3 collision_point;
	Vector3 collision_normal;

	float sphere_radius = 0.15f;
	float sphere_ground_radius = 0.05f;
	float player_height = 0.2f;

	// Checkear colision
	Vector3 floor_sphere_center = center + Vector3(0.0f, sphere_ground_radius, 0.0f);

}

void EntityCollider::getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions) {

}