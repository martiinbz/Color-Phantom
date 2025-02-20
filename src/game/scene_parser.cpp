#include "scene_parser.h"

#include "graphics/material.h"
#include "graphics/mesh.h"
#include "framework/player.h"

#include "framework/utils.h"
#include "game/stage.h"
#include "game/game.h"
#include "framework/world.h"
#include <fstream>

bool SceneParser::parse(const char* filename, Entity* root)
{
	std::cout << " + Scene loading: " << filename << "..." << std::endl;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}

	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		if (mesh_name[0] == '#')
			continue;

		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {

		mesh_name = "data/" + data.first;
		sRenderData& render_data = data.second;

		// No transforms, nothing to do here
		if (render_data.models.empty())
			continue;

		Material mat = render_data.material;
		EntityMesh* new_entity = nullptr;

		std::cout << "Procesando entidad: " << data.first << std::endl;

		size_t tag = data.first.find("@player");

		if (tag != std::string::npos) {
			
			std::cout << "Encontrado @player en escena!" << std::endl;

			// Cargar la malla desde el archivo de Blender
			Mesh* mesh = Mesh::Get(mesh_name.c_str());

			// Crear la entidad como un Player en vez de EntityMesh
			new_entity = new Player(mesh, mat, "player");

			// Referenciar globalmente el player
			root->addChild(new_entity); // Agregarlo a la jerarquía de la escena

			// Guardamos el jugador en World
			//World::get_instance()->player = new_entity;
			
		
		}
		else {
			Mesh* mesh = Mesh::Get(mesh_name.c_str());
			new_entity = new EntityMesh(mesh, mat);
		}

		if (!new_entity) {
			continue;
		}

		new_entity->name = data.first;

		// Create instanced entity
		if (render_data.models.size() > 1) {
			new_entity->isInstanced = true;
			new_entity->models = render_data.models; // Add all instances
		}
		// Create normal entity
		else {
			new_entity->model = render_data.models[0];
		}

		// Add entity to scene root
		root->addChild(new_entity);
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;
}