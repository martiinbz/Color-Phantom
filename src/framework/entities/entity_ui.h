#pragma once
#include "framework/entities/entity_mesh.h"

class EntityUI : public EntityMesh {

public:
	EntityUI(Vector2 size, const Material& material);
	EntityUI(Vector2 pos, Vector2 size, const Material& material, eUIButtonID button_id = UI_BUTTON_UNDEFINED, const std::string& name = "");
	
	Vector2 position;
	Vector2 size;

	float mask = 1.0f;

	// 3D HUD
	bool is3D = false;
	bool visible = true;
	Vector3 pos3D;

	eUIButtonID button_id = UI_BUTTON_UNDEFINED;

	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
	void update3D(const Vector3& position3D);
};