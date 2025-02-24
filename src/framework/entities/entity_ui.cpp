

#include "entity_ui.h"
#include "game/game.h"
#include "framework/world.h"
#include "framework/entities/entity_mesh.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/material.h"
#include "framework/input.h"

EntityUI::EntityUI(Vector2 new_size, const Material& material) {
    size = new_size;
    this->material = new Material(material);

    if (!this->material->shader)
        this->material->shader = Shader::Get("data/shaders/basic.vs",  "data/shaders/texture.fs" );
}

EntityUI::EntityUI(Vector2 new_pos, Vector2 new_size, const Material& material, const std::string& name) {
    position = new_pos;
    size = new_size;
	this->material = new Material(material);
    mesh = new Mesh();
    mesh->createQuad(position.x, position.y, size.x, size.y, true);

    if (!this->material->shader)
        this->material->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

 bool UI::addbutton(Vector2 pos, Vector2 size, const char* texture_path) {

	 World::get_instance()->camera2D->enable();    
     Vector2 mouse_pos = Input::mouse_position;
     bool is_hovered = false;
     bool was_presed = false;
    
     if (mouse_pos.x > (pos.x - size.x * 0.5f) && mouse_pos.x < (pos.x + size.x * 0.5f) &&
         mouse_pos.y >(pos.y - size.y * 0.5f) && mouse_pos.y < (pos.y + size.y * 0.5f)) {
         is_hovered = true;
		 was_presed = Input::wasMousePressed(SDL_BUTTON_LEFT);

     }
     glDisable(GL_DEPTH_TEST);
	 glDisable(GL_CULL_FACE);
	 glDisable(GL_BLEND);
	
	 Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	 Texture* texture = Texture::Get(texture_path);
    
  
     shader->enable();

     if (is_hovered) {
		 std::cout << "hovered" << std::endl;
		 shader->setUniform("u_color", Vector4(0.5, 0.5, 0.5, 1));
	 }
     else {
		 shader->setUniform("u_color", Vector4(1, 0, 0, 1));
     }
     shader->setUniform("u_model", Matrix44());
	 shader->setUniform("u_viewprojection", World::get_instance()->camera2D->viewprojection_matrix);
	 shader->setUniform("u_texture", texture, 0);
    
     Mesh quad;
     quad.createQuad(pos.x, pos.y, size.x, size.y, true);
     quad.render(GL_TRIANGLES);
	 
     shader->disable();
    
	 //drawText(5, 15, "LOS BOTONES NO SE PINTAN,PERO ESTAN CREADOS. HAZ CLICK EN EL CENTRO", Vector3(1, 1, 1), 2);
    
 
	 return was_presed;
}

 void UI::addbackground(Vector2 pos, Vector2 size, const char* texture_path) {
     glDisable(GL_DEPTH_TEST);
     glDisable(GL_CULL_FACE);
     glDisable(GL_BLEND);

     Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
     Texture* texture = Texture::Get(texture_path);

     shader->enable();

     shader->setUniform("u_color", Vector4(1, 0, 0, 1));
     shader->setUniform("u_model", Matrix44());
     shader->setUniform("u_viewprojection", World::get_instance()->camera2D->viewprojection_matrix);
     shader->setUniform("u_texture", texture, 0);

     Mesh quad;
     quad.createQuad(pos.x, pos.y, size.x, size.y, true);
     quad.render(GL_TRIANGLES);

     shader->disable();
     glEnable(GL_DEPTH_TEST);

 
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

