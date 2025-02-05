#pragma once

#include "framework/includes.h"
#include "framework/framework.h"
#include "framework/entities/entity.h"

class Camera;
class Mesh;
class Material;
class Shader;
class Texture;



class EntityMesh: public Entity {

public:
	bool isInstanced = false;
	std::vector<Matrix44> models;
	EntityMesh() {}; 			// Constructor
	EntityMesh(Mesh* new_mesh, const Material& new_material);
	virtual ~EntityMesh() {}; 	// Destructor	

	
	Mesh* mesh = nullptr;
	Material* material = nullptr;
	Texture* texture = nullptr;
	Vector4 color;
	Shader* shader = nullptr;



	// Methods that should be overwritten
	// by derived classes 
	void render(Camera* camera) override;
	void update(float delta_time) override;

	
};
