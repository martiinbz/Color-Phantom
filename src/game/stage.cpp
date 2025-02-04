#include "stage.h"
#include "game.h"


#include "stage.h"
#include "framework/utils.h"
#include <iostream>
#include "framework/input.h"




//PLAY STAGE///////////////////////////////////////////



PlayStage::PlayStage(): Stage() {
	root = new Entity();

	SceneParser parser;
	parser.parse("data/scene.txt", root);

	
}



void PlayStage::render(Camera* camera) {
    
	

	
}

void PlayStage::update(double elapsed_time) {
	
	
		
}