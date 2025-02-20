#pragma once
#include "framework/entities/entity_mesh.h"
#include "graphics/material.h"
#include "graphics/shader.h"

class Player : public EntityMesh {

	
	float walk_speed = 0.0f;
	Vector3 velocity = Vector3(0,0,0);
	Vector3 current_color = Vector3(1, 1, 1); //el color del jugador es blanco por defecto
	Vector3 target_color; //color al que el jugador va a cambiar

	Shader* player_shader;

public:
	static Player* instance;
	Player() {};
	Player(Mesh* mesh, const Material& material, const std::string& name = "");
	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
};