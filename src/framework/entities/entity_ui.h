#pragma once
#include "framework/entities/entity_mesh.h"
#include "framework/camera.h"

class EntityUI : public EntityMesh {

public:
	EntityUI(Vector2 size, const Material& material);
	EntityUI(Vector2 pos, Vector2 size, const Material& material, const std::string& name = "");

	Vector2 position;
	Vector2 size;

	float mask = 1.0f;

	// 3D HUD
	
	bool is3D = false;
	bool visible = true;
	Vector3 pos3D;

	void render(Camera* camera) override;
	//void update(float seconds_elapsed) override;
	//void update3D(const Vector3& position3D);
};

namespace UI {
	bool addbutton(Vector2 pos, Vector2 size, const char* text);
	void addbackground(Vector2 pos, Vector2 size, const char* text);
};