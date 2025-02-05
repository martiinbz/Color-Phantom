#include "stage.h"
#include "game.h"


#include "stage.h"
#include "framework/utils.h"
#include <iostream>
#include "framework/input.h"
#include "game/scene_parser.h"
#include "framework/entities/entity.h"




//PLAY STAGE///////////////////////////////////////////



PlayStage::PlayStage(): Stage() {
	Entity*root = new Entity();

	SceneParser parser;
	parser.parse("data/myscene.scene", root);
}



void PlayStage::render(Camera* camera) {
    
	

	
}

void PlayStage::update(double elapsed_time) {
	
	
		
}