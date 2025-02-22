#include "entity_ui.h"
#include "game/game.h"
#include "framework/world.h"
#include "framework/entities/entity_mesh.h"
#include "graphics/mesh.h"

EntityUI::EntityUI(Vector2 new_size, const Material& material) {
	size = new_size;
	this->material = material;

	if (!this->material->shader)
		this->material->shader = Shader::Get("data/shaders/basic.vs", material.diffuse ? "data/shaders/texture.fs" : "data/shaders/flat.fs");
}

EntityUI::EntityUI(Vector2 new_pos, Vector2 new_size, const Material& material, eUIButtonID new_button_id, const std::string& name) {
	position = new_pos;
	size = new_size;
	button_id = new_button_id;

	mesh = new Mesh();
	mesh->createQuad(position.x, position.y, size.x, size.y, true);

	if (!this->material->shader)
		this->material->shader = Shader::Get("data/shaders/basic.vs", material.diffuse ? "data/shaders/texture.fs" : "data/shaders/flat.fs");
}

void EntityUI::render(Camera* camera2D) {
	if (!visible)
		return;
	if (!is3D)
		glDisable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	material->shader->enable();

	if (is3D) {
		Mesh quad;
		quad.createQuad(position.x, position.y, size.x, size.y, true);
		quad.render(GL_TRIANGLES);
	}
	else
		mesh->render(GL_TRIANGLES);

	material->shader->disable();

	glDisable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);

	Entity::render(camera2D);
}

void EntityUI::update3D(const Vector3& position3D) {
	pos3D = position3D;

	// actualizar el HUD 3D
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
	World* world = World::get_instance();

	visible = true;

	// fuera del frustum no pintamos 
	Vector3 pos = world->camera->project(position3D, width, height);
	if (pos.z < -1.0f || pos.z > 1.0f)
		visible = false;

	// si pintamos lo hacemos en esta posicion
	else {
		pos.z = height - pos.y;
		position = Vector2(pos.x, pos.y);
	}
}