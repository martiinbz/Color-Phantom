#include "stage.h"
#include "game.h"
#include "framework/utils.h"
#include <iostream>
#include <chrono>
#include <iomanip>
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

bool animation_L1 = false;
bool animation_L2 = false;
bool animation_L3 = false;

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

std::chrono::steady_clock::time_point start_time;

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
    Vector3 l_color = World::get_instance()->looking_color;

    if (!showMenuLevel) {
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, 34), Vector2(600, 70), "data/button/FONDONEGRO.png");
        UI::addbackground(Vector2(400, 34), Vector2(64, 64), "data/button/T.png");
        drawText(120, 12, "TARGET", target_color, 6);
        drawText(450, 12, "LOOKING", l_color, 6);
        // drawText(20, 50, "TIME: " + std::to_string(elapsed_time) + "s", Vector3(1, 1, 1), 5);
    }

    //WIN CONDITION//
    accuracy = 1 - ((color - target_color).length() / sqrt(3));
    accuracy *= 100;
    if (accuracy < 0) accuracy = 0;
    if (accuracy > 100) accuracy = 100;

    if (Input::wasKeyPressed(SDL_SCANCODE_G)) {
		drawText(20, 20, "ACCURACY: " + std::to_string(accuracy) + "%", Vector3(1, 1, 1), 5);
    }
    if (accuracy > 70.0f && !L1_completed) {
        L1_completed = true;
        final_time = elapsed_time;
    }

    if (L1_completed) {
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(600, 400), "data/button/ANIMATIONL1.png");
        std::stringstream timeText;
        timeText << std::fixed << std::setprecision(2) << final_time;
        drawText(480, 283, timeText.str(), Vector3(1, 1, 1), 4);
        if (final_time <= 10.0) {
            UI::addbackground(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
        }
        else if (final_time > 10 and final_time <= 15) {
            UI::addbackground(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
        }
        else if (final_time > 15 and final_time <= 20) {
            UI::addbackground(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
        }
        else {
            UI::addbackground(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
        }
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.75), Vector2(192, 64), "data/button/CONTINUE.png")) {
            animation_L1 = true;
        }
    }
}

void L1Stage::update(double seconds_elapsed) {
    if (!showMenuLevel || L1_completed || (!showMenuLevel && L1_completed)) World::get_instance()->update(seconds_elapsed);

    elapsed_time = std::chrono::duration<float>(std::chrono::steady_clock::now() - start_time).count();

    if (Input::wasKeyPressed(SDL_SCANCODE_T)) showMenuLevel = true;

    if (L1_completed) {
        Game::instance->setMouseLocked(false);
        timer_running = false;
        if (animation_L1) Game::instance->goToStage(STAGE_MENU);
    }
}

void L1Stage::onEnter(Stage* stage) {
    std::cout << "ENTERING LEVEL 1..." << std::endl;
    Game::instance->setMouseLocked(true);
    if (musicOn) musica_L1 = Audio::Play("data/audio/MUSICA_L1_SUPERMERCADO.mp3", 0.15f, BASS_SAMPLE_LOOP);
    color = Vector3(0.f);
    start_time = std::chrono::steady_clock::now();
    timer_running = true;
}

void L1Stage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(false);
    showMenuLevel = false;
    Audio::Stop(musica_L1);
    timer_running = false;
}


// LEVEL 2 STAGE

void L2Stage::init() {
}

void L2Stage::render(Camera* camera) {
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
                Audio::Stop(musica_L2);
                musicOn = false;
            }
        }
        else {
            if (UI::addbutton(Vector2(Game::instance->window_width * 0.45, Game::instance->window_height * 0.5), Vector2(50, 50), "data/button/MUSICOFF.png")) {
                if (soundsOn) Audio::Play("data/audio/SWITCH.ogg", 1.0f, BASS_SAMPLE_MONO);
                musica_L2 = Audio::Play("data/audio/MUSICA_L2_FABRICA.mp3", 0.15f, BASS_SAMPLE_LOOP);
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
    Vector3 l_color = World::get_instance()->looking_color;

    if (!showMenuLevel) {
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, 34), Vector2(600, 70), "data/button/FONDONEGRO.png");
        UI::addbackground(Vector2(400, 34), Vector2(64, 64), "data/button/T.png");
        drawText(120, 12, "TARGET", target_color, 6);
        drawText(450, 12, "LOOKING", l_color, 6);
        // drawText(20, 50, "TIME: " + std::to_string(elapsed_time) + "s", Vector3(1, 1, 1), 5);
    }

    //WIN CONDITION//
    accuracy = 1 - ((color - target_color).length() / sqrt(3));
    accuracy *= 100;
    if (accuracy < 0) accuracy = 0;
    if (accuracy > 100) accuracy = 100;

    if (Input::wasKeyPressed(SDL_SCANCODE_G)) {
        drawText(20, 20, "ACCURACY: " + std::to_string(accuracy) + "%", Vector3(1, 1, 1), 5);
    }
    if (accuracy > 70.0f && !L1_completed) {
        L1_completed = true;
        final_time = elapsed_time;
    }

    if (L1_completed) {
        UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5), Vector2(600, 400), "data/button/ANIMATIONL2.png");
        std::stringstream timeText;
        timeText << std::fixed << std::setprecision(2) << final_time;
        drawText(480, 283, timeText.str(), Vector3(1, 1, 1), 4);
        if (final_time <= 10.0) {
            UI::addbackground(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
        }
        else if (final_time > 10 and final_time <= 15) {
            UI::addbackground(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
        }
        else if (final_time > 15 and final_time <= 20) {
            UI::addbackground(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAR.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
        }
        else {
            UI::addbackground(Vector2(Game::instance->window_width * 0.4, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
            UI::addbackground(Vector2(Game::instance->window_width * 0.6, Game::instance->window_height * 0.6), Vector2(64, 60), "data/button/STAREMPTY.png");
        }
        if (UI::addbutton(Vector2(Game::instance->window_width * 0.5, Game::instance->window_height * 0.75), Vector2(192, 64), "data/button/CONTINUE.png")) {
            animation_L1 = true;
        }
    }
}

void L2Stage::update(double seconds_elapsed) {
    if (!showMenuLevel || L2_completed || (!showMenuLevel && L2_completed)) World::get_instance()->update(seconds_elapsed);

    elapsed_time = std::chrono::duration<float>(std::chrono::steady_clock::now() - start_time).count();

    if (Input::wasKeyPressed(SDL_SCANCODE_T)) showMenuLevel = true;

    if (L2_completed) {
        Game::instance->setMouseLocked(false);
        timer_running = false;
        if (animation_L2) Game::instance->goToStage(STAGE_MENU);
    }
}

void L2Stage::onEnter(Stage* stage) {
    std::cout << "ENTERING LEVEL 2..." << std::endl;
    Game::instance->setMouseLocked(true);
    musica_L2 = Audio::Play("data/audio/MUSICA_L2_FABRICA.mp3", 0.15f, BASS_SAMPLE_LOOP);
    color = Vector3(0.f);
    start_time = std::chrono::steady_clock::now();
    timer_running = true;
}

void L2Stage::onLeave(Stage* stage) {
    Game::instance->setMouseLocked(false);
    showMenuLevel = false;
    Audio::Stop(musica_L2);
    timer_running = false;
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