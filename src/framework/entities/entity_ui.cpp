// entity_ui.cpp

#include "entity_ui.h"
#include "game/game.h"
#include "framework/world.h"
#include "framework/entities/entity_mesh.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/material.h"

EntityUI::EntityUI(Vector2 new_size, const Material& material) {
    size = new_size;
    this->material = new Material(material);

    if (!this->material->shader)
        this->material->shader = Shader::Get("data/shaders/basic.vs", material.diffuse ? "data/shaders/texture.fs" : "data/shaders/flat.fs");
}

EntityUI::EntityUI(Vector2 new_pos, Vector2 new_size, const Material& material, const std::string& name) {
    position = new_pos;
    size = new_size;
	this->material = new Material(material);
    mesh = new Mesh();
    mesh->createQuad(position.x, position.y, size.x, size.y, true);

    if (!this->material->shader)
        this->material->shader = Shader::Get("data/shaders/basic.vs", material.diffuse ? "data/shaders/texture.fs" : "data/shaders/flat.fs");
}

bool EntityUI::addButton(Vector2 pos, Vector2 siz, const char* texture_path)
{
    // 1. Cargar la textura del botón
    Texture* btn_texture = Texture::Get(texture_path);

    // 2. Crear un material con esa textura
    Material btn_material;
    btn_material.diffuse = btn_texture;

    // 3. Crear un EntityUI que represente el botón (quad 2D)
    //    Reutilizamos el mismo constructor de arriba
    EntityUI* button = new EntityUI(pos, siz, btn_material, "temp_button");

    // 4. Renderizarlo de inmediato (Immediate Mode)
    button->render(World::get_instance()->camera2D);

    // 5. Manejar el evento de clic capturando todos los eventos de esta frame
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // Verificamos clic izquierdo
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            // Comprobamos si el clic está dentro del área del botón
            int mx = event.button.x;
            int my = event.button.y;
            if (mx >= pos.x && mx <= (pos.x + siz.x) &&
                my >= pos.y && my <= (pos.y + siz.y))
            {
                std::cout << "Button clicked!" << std::endl;
                return true; // botón presionado
            }
        }
    }

    return false; // no hubo clic dentro del botón
}

void EntityUI::render(Camera* camera2D) {
    if (!visible)
        return;
    if (!is3D)
        glDisable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    material->shader->enable();

    if (is3D) {
        Mesh quad;
        quad.createQuad(position.x, position.y, size.x, size.y, true);
        quad.render(GL_TRIANGLES);
    }
    else
        mesh->render(GL_TRIANGLES);

    material->shader->disable();

    glDisable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);

    Entity::render(camera2D);
}

