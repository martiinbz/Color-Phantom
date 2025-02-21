#include "entity.h"
#include "entity_mesh.h"
#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "framework/world.h"
#include "graphics/shader.h"
#include "framework/player.h"

#include <algorithm>


EntityMesh::EntityMesh(Mesh* new_mesh, const Material& new_material)
{
	mesh = new_mesh;
	material = new Material(new_material);
}

void EntityMesh::render(Camera* camera)
{
	if (!material || !mesh ) {
		return;
	}
	if (!material->shader) {
		material->shader = Shader::Get(isInstanced ? "data/shaders/instanced.vs" : "data/shaders/basic.vs", "data/shaders/texture.fs");
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
	camera->enable();

	material->shader->enable();
	material->shader->setUniform("u_model", getGlobalMatrix());
	material->shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	material->shader->setUniform("u_color", material->color);

	if (isInstanced) {
		mesh->renderInstanced(GL_TRIANGLES, must_render_models.data(), must_render_models.size());
	}
	else {
		mesh->render(GL_TRIANGLES);
	}

	material->shader->disable();

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