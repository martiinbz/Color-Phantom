#include "stage.h"
#include "game.h"
#include "framework/utils.h"
#include <iostream>
#include "framework/input.h"
#include "game/scene_parser.h"
#include "framework/entities/entity.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "framework/player.h"
#include "framework/world.h"
#include "framework/entities/entity_ui.h"
#include "framework/audio.h"

// VARIABLES PARA GESTIONAR LOS NIVELES

bool opened_menu = false;
bool L1_completed = false;
bool L2_completed = false;
bool L3_completed = false;

bool showHowToPlay = false;
bool settingsOpen = false;
bool musicOn = true;
bool soundsOn = true;

int level = 1;


// INTRO STAGE

void IntroStage::init() {
}

void IntroStage::render(Camera* camera) {
    // fondo de pantalla
    UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/PORTADA.png");

    // boton play
    if (!settingsOpen) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.625), Vector2(192, 64), "data/button/PLAY.png")) {
            opened_menu = true;
            level = 1;
            Audio::Play("data/audio/1_PLAY_INTRO.ogg", 1.0f, BASS_SAMPLE_MONO);
            Game::instance->goToStage(STAGE_MENU);
        }
    }

    // settings play
    if (!settingsOpen) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.75), Vector2(192, 64), "data/button/SETTINGS.png")) {
            settingsOpen = true;
        }
    }

    if (settingsOpen) {
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(250, 70), "data/button/FONDONEGRO.png");
        if (musicOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICON.png"))
                musicOn = false;
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICOFF.png"))
                musicOn = true;
        }

        if (soundsOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOON.png"))
                soundsOn = false;
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOOFF.png"))
                soundsOn = true;
        }

        if (UI::addbutton(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.5), Vector2(64, 64), "data/button/X.png"))
            settingsOpen = false;
    }

    // boton exit
    if (!settingsOpen) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.875), Vector2(192, 64), "data/button/EXIT.png")) {
            exit(0);
        }
    }
}

void IntroStage::update(double dt) {

}

void IntroStage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(false); // por si en otra escena estaba bloqueado

    intro_audio = Audio::Play("data/audio/1_PLAY_INTRO.ogg", 1.0f, BASS_SAMPLE_MONO);
}

void IntroStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);

    Audio::Stop(intro_audio);
}


// MENU STAGE

void MenuStage::init() {
}

void MenuStage::render(Camera* camera) {
    if (level == 0) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/TUTORIAL.png");
    if (level == 1) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/LEVEL1.png");
    if (level == 2) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/LEVEL2.png");
    if (level == 3) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/LEVEL3.png");
    if (level == 4) UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/COMINGSOON.png");
    
    if (showHowToPlay) {
        if (!settingsOpen) {
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(600, 400), "data/button/MENUHTP.png");
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.75), Vector2(64, 64), "data/button/X.png"))
                showHowToPlay = false;
        }
    }

    if (level == 2 && !L1_completed)
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(100, 100), "data/button/LOCKED.png");
    if (level == 3 && !L2_completed)
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(100, 100), "data/button/LOCKED.png");

    if (level != 4) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.93), Vector2(192, 64), "data/button/PLAY.png")) {
            if (!showHowToPlay && !settingsOpen) {
                if (level == 1) Game::instance->goToStage(STAGE_L1);
                if (level == 2 && L1_completed) Game::instance->goToStage(STAGE_L2);
                if (level == 3 && L2_completed) Game::instance->goToStage(STAGE_L3);
                if (level == 0) Game::instance->goToStage(STAGE_TUTORIAL);
            }
        }
    }

    if (level != 0) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.05, Game::instance->window_height * 0.5), Vector2(32, 32), "data/button/FLECHAIZQ.png")) {
            if (!showHowToPlay && !settingsOpen) level--;
        }
    }

    if (level != 4) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.95, Game::instance->window_height * 0.5), Vector2(32, 32), "data/button/FLECHADER.png")) {
            if (!showHowToPlay && !settingsOpen) level++;
        }
    }

    // settings button
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.955, Game::instance->window_height * 0.05), Vector2(50, 50), "data/button/LLAVESETTINGS.png")) {
        if (!showHowToPlay)
            settingsOpen = true;
    }

    if (settingsOpen) {
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(250, 70), "data/button/FONDONEGRO.png");
        if (musicOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICON.png"))
                musicOn = false;
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICOFF.png"))
                musicOn = true;
        }

        if (soundsOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOON.png"))
                soundsOn = false;
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOOFF.png"))
                soundsOn = true;
        }

        if (UI::addbutton(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.5), Vector2(64, 64), "data/button/X.png"))
            settingsOpen = false;
    }

    // how to play button
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.955, Game::instance->window_height * 0.94), Vector2(50, 50), "data/button/HOWTOPLAY.png")) {
        showHowToPlay = true;
    }

    // back button
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.04, Game::instance->window_height * 0.05), Vector2(50, 50), "data/button/EXITBACK.png")) {
        if (!showHowToPlay && !settingsOpen) Game::instance->goToStage(STAGE_INTRO);
    }
}

void MenuStage::update(double dt) {
    if (L1_completed && L2_completed && L3_completed) {
        // todo el juego completado, podemos implementar un titulo de victoria del juegador
    }
}

void MenuStage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(false);
}

void MenuStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);
}


// TUTORIAL STAGE

void TutorialStage::init() {

}

void TutorialStage::render(Camera* camera) {

}

void TutorialStage::update(double dt) {
    
}

void TutorialStage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(false);
}

void TutorialStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);
}


// LEVEL 1 STAGE

void L1Stage::init() {

}

void L1Stage::render(Camera* camera) {
 
    camera->enable();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

   
    World::get_instance()->render();

    drawGrid();
    drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
    Vector3 color = Player::instance->current_color;
	std::cout << "color: " << color.x << " " << color.y << " " << color.z << std::endl; 
    if (color.x==0 && color.y==0 && color.z==0) {
		std::cout << "has ganado!" << std::endl;

    }
}

void L1Stage::update(double seconds_elapsed) {
    World::get_instance()->update(seconds_elapsed);

    if (L1_completed)
        Game::instance->goToStage(STAGE_MENU);
}

void L1Stage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(true);
}

void L1Stage::onLeave(Stage* stage) {
    
}


// LEVEL 2 STAGE

void L2Stage::init() {
}

void L2Stage::render(Camera* camera) {

}

void L2Stage::update(double seconds_elapsed) {
    World::get_instance()->update(seconds_elapsed);

    if (L2_completed)
        Game::instance->goToStage(STAGE_MENU);
}

void L2Stage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(true);
}

void L2Stage::onLeave(Stage* stage) {

}


// LEVEL 3 STAGE

void L3Stage::init() {
}

void L3Stage::render(Camera* camera) {

}

void L3Stage::update(double seconds_elapsed) {
    World::get_instance()->update(seconds_elapsed);

    if (L3_completed)
        Game::instance->goToStage(STAGE_MENU);
}

void L3Stage::onEnter(Stage* stage) {
    Game::instance->setMouseLocked(true);
}

void L3Stage::onLeave(Stage* stage) {

}