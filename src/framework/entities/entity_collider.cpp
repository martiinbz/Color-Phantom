#include "entity_collider.h"
#include "framework/world.h"
#include "graphics/mesh.h"

EntityCollider::EntityCollider(Mesh* mesh, const Material& material, const std::string name)
	: EntityMesh(mesh, material) {
};

void EntityCollider::getCollisionsWithModel(const Matrix44& m, const Vector3& center, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions) {
	Vector3 collision_point;
	Vector3 collision_normal;

	float sphere_radius = 0.15f;
	float sphere_ground_radius = 0.05f;
	float player_height = 0.2f;

	// Checkear colision
	Vector3 floor_sphere_center = center + Vector3(0.0f, sphere_ground_radius, 0.0f);
	if (mesh->testSphereCollision(m, floor_sphere_center, sphere_radius, collision_point, collision_normal)) {
		collisions.push_back(sCollisionData(collision_point, collision_normal.normalize(), (collision_point - floor_sphere_center).length(), true));
	}

	// Colisiones de muros
	Vector3 character_center = center + Vector3(0.f, player_height, 0.f);
	if (mesh->testSphereCollision(m, character_center, sphere_radius, collision_point, collision_normal)) {
		collisions.push_back(sCollisionData(collision_point, collision_normal.normalize(), (collision_point - character_center).length(), true));
	}

	// Colisiones de suelo
	if (mesh->testRayCollision(m, character_center, Vector3(0, -1, 0), collision_point, collision_normal)) {
		ground_collisions.push_back(sCollisionData(collision_point, collision_normal.normalize(), (collision_point - character_center).length(), true));
	}
}

void EntityCollider::getCollisions(const Vector3& target_position, std::vector<sCollisionData>& collisions, std::vector<sCollisionData>& ground_collisions) {
	// if (!(layer & filter)) no hay filter
	if (!(layer))
		return;

	if (!isInstanced)
		getCollisionsWithModel(model, target_position, collisions, ground_collisions);
	else {
		for (int i = 0; i < models.size(); i++) {
			getCollisionsWithModel(models[i], target_position, collisions, ground_collisions);
		}
	}
}