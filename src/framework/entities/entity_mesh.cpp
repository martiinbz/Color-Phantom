#include "entity.h"
#include "entity_mesh.h"
#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "graphics/shader.h"

#include <algorithm>


EntityMesh::EntityMesh(Mesh* new_mesh, const Material& new_material)
{
	mesh = new_mesh;
	material = new Material(new_material);
}

void EntityMesh::render(Camera* camera)
{
	material->shader = Shader::Get(isInstanced ? "data/shaders/instanced.vs": "data/shaders/basic.vs", "data/shaders/flat.fs");
	if (!material || !mesh || material->shader) {
		return;
	}
		
	camera->enable();



	material->shader->enable();
	material->shader->setUniform("u_model", getGlobalMatrix());
	material->shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	material->shader->setUniform("u_color", Vector4(1, 0, 0, 1));

	if (isInstanced) {
		mesh->renderInstanced(GL_TRIANGLES,models.data(), models.size());
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

