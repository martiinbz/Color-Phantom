#include "entity.h"
#include "entity_mesh.h"
#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "framework/world.h"
#include "graphics/shader.h"
#include "framework/player.h"
#include "game/game.h"

#include <algorithm>


EntityMesh::EntityMesh(Mesh* new_mesh, const Material& new_material)
{
	mesh = new_mesh;
	material = new Material(new_material);
}

void EntityMesh::render(Camera* camera)
{	
	camera->enable();

	if (!material || !mesh ) {
		return;
	}

	std::vector<Matrix44> must_render_models;
	//// CULLINGS
	bool must_render = true;
	if (!isInstanced) {

		Matrix44 global_matrix = getGlobalMatrix();
		float distance = camera->eye.distance(global_matrix.getTranslation());
		must_render &= (distance < 100);

		Vector3 bb_center = global_matrix * mesh->box.center;
		Vector3 bb_halfsize = mesh->box.halfsize;
		must_render &= (camera->testBoxInFrustum(bb_center, bb_halfsize) != CLIP_OUTSIDE);
	}
	else {
		for (const Matrix44& model : models) {
			Matrix44 global_matrix = model * parent->getGlobalMatrix();
			float distance = camera->eye.distance(global_matrix.getTranslation());

			Vector3 bb_center = global_matrix * mesh->box.center;
			Vector3 bb_halfsize = mesh->box.halfsize;
			if (distance < 100 && camera->testBoxInFrustum(bb_center, bb_halfsize) != CLIP_OUTSIDE) {
				must_render_models.push_back(model);
			}
		}
	}

	if (!must_render) {
		return;
	}

	Shader* shader = Shader::Get(isInstanced ? "data/shaders/instanced.vs" : "data/shaders/basic.vs", "data/shaders/texture.fs");;

	glEnable(GL_DEPTH_TEST);

	shader->enable();

	shader->setUniform("u_model", getGlobalMatrix());
	shader->setUniform("u_color", material->color);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_background_color", Vector4(0.1f, 0.1f, 0.1f, 1.0f));
	shader->setUniform("u_camera_position", camera->eye);

	shader->setUniform("u_Ka", Vector3(1.0f));
	shader->setUniform("u_Kd", Vector3(1.0f));
	shader->setUniform("u_Ks", Vector3(1.0f));
	shader->setUniform("u_light_color", Vector3(0.9f, 0.9f, 1.0f));
	shader->setUniform("u_light_position", Vector3(0.0f, 30.0f, 20.0f));
	shader->setUniform("u_fog_factor", 1.0f);
	shader->setUniform("u_time", Game::instance->time);

	Vector2 maps = { 0.0f, 0.0f };

	if (material->diffuse) {
		maps.x = 1.0f;
		shader->setUniform("u_texture", material->diffuse, 1);
	}
	if (material->normal) {
		maps.y = 1.0f;
		shader->setUniform("u_normal_texture", material->normal, 1);
	}
	shader->setUniform("u_maps", maps);

	if (isAnimated) {
		mesh->renderAnimated(GL_TRIANGLES, &animator.getCurrentSkeleton());
	}

	if (isInstanced && !isAnimated) {
		mesh->renderInstanced(GL_TRIANGLES, must_render_models.data(), must_render_models.size());
	}
	else {
		mesh->render(GL_TRIANGLES);
	}

	shader->disable();

	//propagate render call to children
	Entity::render(camera);
}

void EntityMesh::update(float delta_time)
{
	Entity::update(delta_time);
}

// SI IMPLEMENTAMOS LODS
//void EntityMesh::addMeshLOD(Mesh* mesh, float distance)
//{
//	mesh_lods.push_back({ mesh, distance });
//
//	std::sort(mesh_lods.begin(), mesh_lods.end(),
//		[](const sMeshLOD& l1, const sMeshLOD& l2) {
//			return l1.distance > l2.distance;
//		});
//}	