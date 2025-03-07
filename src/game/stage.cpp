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

bool showMenuLevel = false;

int level = 1;


// INTRO STAGE

void IntroStage::init() {
    Audio::Init();
}

void IntroStage::render(Camera* camera) {
    // fondo de pantalla
    UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(800, 600), "data/button/PORTADA.png");

    // boton play
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.725), Vector2(192, 64), "data/button/PLAY.png")) {
        opened_menu = true;
        level = 1;
        if (soundsOn) Audio::Play("data/audio/CLICK_PLAY_INTRO.ogg", 1.0f, BASS_SAMPLE_MONO);
        Game::instance->goToStage(STAGE_MENU);
    }

    // boton exit
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.85), Vector2(192, 64), "data/button/EXIT.png")) {
        if (soundsOn) Audio::Play("data/audio/SALIR.ogg", 1.0f, BASS_SAMPLE_MONO);
        exit(0);
    }
}

void IntroStage::update(double dt) {

}

void IntroStage::onEnter(Stage* stage) {
    std::cout << "ENTERING INTRO..." << std::endl;
    Game::instance->setMouseLocked(false);
    if (musicOn) musica_intro = Audio::Play("data/audio/MUSICA_INTRO.mp3", 1.0f, BASS_SAMPLE_LOOP);
}

void IntroStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);
    Audio::Stop(musica_intro);
}


// MENU STAGE

void MenuStage::init() {
    Audio::Init();
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
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.75), Vector2(64, 64), "data/button/X.png")) {
                if (soundsOn) Audio::Play("data/audio/CERRAR_X.ogg", 0.7f, BASS_SAMPLE_MONO);
                showHowToPlay = false;
            }
        }
    }

    if (level == 2 && !L1_completed)
        if (!settingsOpen && !showHowToPlay)
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(100, 100), "data/button/LOCKED.png");
    if (level == 3 && !L2_completed)
        if (!settingsOpen && !showHowToPlay)
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

    // flechas izquierda y derecha
    if (level != 0) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.05, Game::instance->window_height * 0.5), Vector2(32, 32), "data/button/FLECHAIZQ.png")) {
            if (!showHowToPlay && !settingsOpen) {
                if (soundsOn) Audio::Play("data/audio/CLICK_PLAY_INTRO.ogg", 0.7f, BASS_SAMPLE_MONO);
                level--;
				std::cout << "level: " << level << std::endl;
            }
        }
    }
    if (level != 4) {
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.95, Game::instance->window_height * 0.5), Vector2(32, 32), "data/button/FLECHADER.png")) {
            if (!showHowToPlay && !settingsOpen) {
                if (soundsOn) Audio::Play("data/audio/CLICK_PLAY_INTRO.ogg", 0.7f, BASS_SAMPLE_MONO);
                level++;
				std::cout << "level: " << level << std::endl;
            }
        }
    }

    // settings button
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.955, Game::instance->window_height * 0.05), Vector2(50, 50), "data/button/LLAVESETTINGS.png")) {
        if (!showHowToPlay)
            if (soundsOn) Audio::Play("data/audio/OPEN_MENU.ogg", 0.7f, BASS_SAMPLE_MONO);
            settingsOpen = true;
    }
    if (settingsOpen) {
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(250, 70), "data/button/FONDONEGRO.png");
        if (musicOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICON.png")) {
                Audio::Stop(musica_menu);
                musicOn = false;
            }
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICOFF.png")) {
                if (soundsOn) Audio::Play("data/audio/SWITCH.ogg", 1.0f, BASS_SAMPLE_MONO);
                musica_menu = Audio::Play("data/audio/MUSICA_MENU.mp3", 0.15f, BASS_SAMPLE_LOOP);
                musicOn = true;
            }
        }
        if (soundsOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOON.png"))
                soundsOn = false;
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOOFF.png")) {
                Audio::Play("data/audio/SWITCH.ogg", 1.0f, BASS_SAMPLE_MONO);
                soundsOn = true;
            }
        }
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.5), Vector2(64, 64), "data/button/X.png")) {
            if (soundsOn) Audio::Play("data/audio/CERRAR_X.ogg", 0.7f, BASS_SAMPLE_MONO);
            settingsOpen = false;
        }
    }

    // how to play button
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.955, Game::instance->window_height * 0.94), Vector2(50, 50), "data/button/HOWTOPLAY.png")) {
        if (soundsOn) Audio::Play("data/audio/OPEN_MENU.ogg", 0.7f, BASS_SAMPLE_MONO);
        showHowToPlay = true;
    }

    // back button
    if (UI::addbutton(Vector2(Game::instance->window_width * 0.04, Game::instance->window_height * 0.05), Vector2(50, 50), "data/button/EXITBACK.png")) {
        if (!showHowToPlay && !settingsOpen) {
            if (soundsOn) Audio::Play("data/audio/SALIR.ogg", 0.7f, BASS_SAMPLE_MONO);
            Game::instance->goToStage(STAGE_INTRO);
        }
    }
}

void MenuStage::update(double dt) {
    if (L1_completed && L2_completed && L3_completed) {
        // todo el juego completado, podemos implementar un titulo de victoria del juegador
    }
}

void MenuStage::onEnter(Stage* stage) {
    std::cout << "ENTERING MENU..." << std::endl;
    Game::instance->setMouseLocked(false);
    if (musicOn) musica_menu = Audio::Play("data/audio/MUSICA_MENU.mp3", 0.15f, BASS_SAMPLE_LOOP);
}

void MenuStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(true);
    Audio::Stop(musica_menu);
}


// TUTORIAL STAGE

void TutorialStage::init() {

}

void TutorialStage::render(Camera* camera) {
    camera->enable();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    World::get_instance()->render();

    if (showMenuLevel) {
        Game::instance->setMouseLocked(false);
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(270, 70), "data/button/FONDONEGRO.png");
        if (musicOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.45, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICON.png")) {
                Audio::Stop(musica_L1);
                musicOn = false;
            }
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.45, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICOFF.png")) {
                if (soundsOn) Audio::Play("data/audio/SWITCH.ogg", 1.0f, BASS_SAMPLE_MONO);
                musica_L1 = Audio::Play("data/audio/MUSICA_L1_SUPERMERCADO.mp3", 0.15f, BASS_SAMPLE_LOOP);
                musicOn = true;
            }
        }
        if (soundsOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.35, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOON.png"))
                soundsOn = false;
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.35, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOOFF.png")) {
                Audio::Play("data/audio/SWITCH.ogg", 1.0f, BASS_SAMPLE_MONO);
                soundsOn = true;
            }
        }
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.65, Game::instance->window_height * 0.5), Vector2(64, 64), "data/button/X.png")) {
            if (soundsOn) Audio::Play("data/audio/CERRAR_X.ogg", 0.7f, BASS_SAMPLE_MONO);
            Game::instance->setMouseLocked(true);
            showMenuLevel = false;
        }
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.55, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/EXITBACK.png")) {
            if (soundsOn) Audio::Play("data/audio/SALIR.ogg", 0.7f, BASS_SAMPLE_MONO);
            Game::instance->goToStage(STAGE_MENU);
        }
    }

    drawGrid();
    drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
    color = Player::instance->current_color;
    std::cout << "color: " << color.x << " " << color.y << " " << color.z << std::endl;
    if (color.x < 0.01 && color.y < 0.01 && color.z < 0.01) {
        std::cout << "has ganado!" << std::endl;
        L1_completed = true;
    }
}

void TutorialStage::update(double seconds_elapsed) {
    if (!showMenuLevel) World::get_instance()->update(seconds_elapsed);

    if (Input::wasKeyPressed(SDL_SCANCODE_T)) showMenuLevel = true;

    if (L1_completed)
        Game::instance->goToStage(STAGE_MENU);
}

void TutorialStage::onEnter(Stage* stage) {
    std::cout << "ENTERING TUTORIAL..." << std::endl;
    Game::instance->setMouseLocked(true);
    if (musicOn) musica_L1 = Audio::Play("data/audio/MUSICA_L1_SUPERMERCADO.mp3", 0.15f, BASS_SAMPLE_LOOP);
    color = Vector3(0.f);
}

void TutorialStage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(false);
    showMenuLevel = false;
    Audio::Stop(musica_L1);
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

    if (showMenuLevel) {
        Game::instance->setMouseLocked(false);
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(305, 70), "data/button/FONDONEGRO.png");
        if (musicOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.45, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICON.png")) {
                Audio::Stop(musica_L1);
                musicOn = false;
            }
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.45, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICOFF.png")) {
                if (soundsOn) Audio::Play("data/audio/SWITCH.ogg", 1.0f, BASS_SAMPLE_MONO);
                musica_L1 = Audio::Play("data/audio/MUSICA_L1_SUPERMERCADO.mp3", 0.15f, BASS_SAMPLE_LOOP);
                musicOn = true;
            }
        }
        if (soundsOn) {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.35, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOON.png"))
                soundsOn = false;
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.35, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/AUDIOOFF.png")) {
                Audio::Play("data/audio/SWITCH.ogg", 1.0f, BASS_SAMPLE_MONO);
                soundsOn = true;
            }
        }
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.65, Game::instance->window_height * 0.5), Vector2(64, 64), "data/button/X.png")) {
            if (soundsOn) Audio::Play("data/audio/CERRAR_X.ogg", 0.7f, BASS_SAMPLE_MONO);
            Game::instance->setMouseLocked(true);
            showMenuLevel = false;
        }
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.55, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/EXITBACK.png")) {
            if (soundsOn) Audio::Play("data/audio/SALIR.ogg", 0.7f, BASS_SAMPLE_MONO);
            Game::instance->goToStage(STAGE_MENU);
        }
    }

    color = Player::instance->current_color;
    

    if (!showMenuLevel) {
        UI::addbackground(Vector2(Game::instance->window_width * 0.1875, 32), Vector2(800, 64), "data/button/FONDONEGRO.png");
        UI::addbackground(Vector2(Game::instance->window_width * 0.96, 32), Vector2(64, 64), "data/button/T.png");
        drawText(5, 5, "TARGET COLOR", target_color, 3);
    }

    //WIN CONDITION//
   
    if (Input::wasKeyPressed(SDL_SCANCODE_G)) {
        
        float accuracy = 1 - ((color - target_color).length()/sqrt(3));
        accuracy *= 100;
		if (accuracy < 0) accuracy = 0;
		if (accuracy > 100) accuracy = 100;

		drawText(20, 20, "ACCURACY: " + std::to_string(accuracy) + "%", Vector3(1, 1, 1), 5);

        //if acc>80--> go to win stage
		// if acc<80--> go to lose stage
    }
    if (color.x < 0.01 && color.y < 0.01 && color.z < 0.01) {
		std::cout << "has ganado!" << std::endl;
        L1_completed = true;
    }
}

void L1Stage::update(double seconds_elapsed) {
    if (!showMenuLevel) World::get_instance()->update(seconds_elapsed);

    if (Input::wasKeyPressed(SDL_SCANCODE_T)) showMenuLevel = true;

    if (L1_completed) Game::instance->goToStage(STAGE_MENU);
		// drawText(5, 5, "LEVEL COMPLETED", Vector3(0, 1, 0), 3);
}

void L1Stage::onEnter(Stage* stage) {
    std::cout << "ENTERING LEVEL 1..." << std::endl;
    Game::instance->setMouseLocked(true);
    if (musicOn) musica_L1 = Audio::Play("data/audio/MUSICA_L1_SUPERMERCADO.mp3", 0.15f, BASS_SAMPLE_LOOP);
    color = Vector3(0.f);
}

void L1Stage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(false);
    showMenuLevel = false;
    Audio::Stop(musica_L1);
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
    std::cout << "ENTERING LEVEL 2..." << std::endl;
    Game::instance->setMouseLocked(true);
    musica_L2 = Audio::Play("data/audio/MUSICA_L2_FABRICA.mp3", 0.15f, BASS_SAMPLE_LOOP);
}

void L2Stage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(false);
    Audio::Stop(musica_L2);
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
    std::cout << "ENTERING LEVEL 3..." << std::endl;
    Game::instance->setMouseLocked(true);
    musica_L3 = Audio::Play("data/audio/MUSICA_L3_CASA.mp3", 0.15f, BASS_SAMPLE_LOOP);
}

void L3Stage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(false);
    Audio::Stop(musica_L3);
}

