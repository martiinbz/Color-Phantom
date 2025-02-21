#include "framework/player.h"
#include "framework/world.h"
#include "framework/input.h"
#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "framework/entities/entity_collider.h"

Player* Player::instance = NULL;


Player::Player(Mesh* mesh, const Material& material, const std::string& name)
    : EntityMesh(mesh, material) {
    walk_speed = 2.0f;
    //cargamos el player_texture shader
    player_shader = Shader::Get(isInstanced ? "data/shaders/instanced.vs" : "data/shaders/basic.vs" , "data/shaders/player_texture.fs");
	instance = this;
    
}


void Player::render(Camera* camera) {

	//como el jugador usa otro fs diferente, se debe cambiar el shader.
    if (!mesh || !player_shader) return;
    

    player_shader->enable();


    player_shader->setUniform("u_model", getGlobalMatrix());
    player_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
  

    // enviamos el color actual al shader
    player_shader->setUniform("u_playerColor", Vector4(current_color, 1.0f));

    
    mesh->render(GL_TRIANGLES);

    player_shader->disable();
	
    EntityMesh::render(camera);
}


void Player::update(float seconds_elapsed) {

    float camera_yaw = World::get_instance()->camera_yaw;

    Matrix44 mYaw;
    mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

    Vector3 front = mYaw.frontVector();
    Vector3 right = mYaw.rightVector();
    Vector3 position = model.getTranslation();
	
    Vector3 move_dir;

    if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
        move_dir += front;
    }
    if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
        move_dir -= front;
    }
    if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
        move_dir += right;
        
    }
    if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
        move_dir -= right;
    }

    //mecanica de cambio de color 
    if (Input::isKeyPressed(SDL_SCANCODE_X)) {
        target_color = World::get_instance()->looking_color;

        //cambiar el color progresivamente
		current_color = current_color * (1.0f - seconds_elapsed) + target_color * seconds_elapsed;
    }

    //REINICIAR COLOR A BLANCO
    if (Input::isKeyPressed(SDL_SCANCODE_C)) {
        
        current_color = Vector3(1,1,1);
    }

    // Ajustar velocidad
    float speed_mult = walk_speed;
    if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
        speed_mult *= 3.0f;

    move_dir.normalize();
    move_dir *= speed_mult;

    // Miramos que no haya colisiones
    test_collisions(position, seconds_elapsed);

    // Aplicar el movimiento
    position += move_dir * seconds_elapsed;
    
    model.setTranslation(position);
    model.rotate(camera_yaw, Vector3(0, 1, 0));
	
    // Reducir velocidad para fricción
    velocity.x *= 0.5f;
    velocity.y *= 0.5;
   

    model.scale(0.6f);
    EntityMesh::update(seconds_elapsed);
}

void Player::test_collisions(Vector3& position, float seconds_elapsed) {
    std::vector<sCollisionData> collisions;
    std::vector<sCollisionData> ground_collisions;

    World::get_instance()->test_scene_collisions(position + velocity * seconds_elapsed, collisions, ground_collisions);

    // Checkeo de colisiones
    for (const sCollisionData& collision : collisions) {

        // Si el vector normal apunta hacia arriba tenemos una colision de suelo
        float up_vector = fabsf(collision.col_normal.dot(Vector3::UP));
        if (up_vector > 0.8)
            continue;
		
        // Movernos arrastrandonos por la pared cuando chocamos
		position += collision.col_normal * collision.distance;
        Vector3 newDir = velocity.dot(collision.col_normal) * collision.col_normal;
        
        velocity.x -= newDir.x;
        velocity.y -= newDir.y;
    }

    // Colisiones de suelo
    bool is_grounded = false;
    for (const sCollisionData& collision : ground_collisions) {
        float up_vector = fabsf(collision.col_normal.dot(Vector3::UP));
        if (up_vector > 0.8)
            is_grounded = true;
    }
	

    if (!is_grounded)
        velocity.y -= 9.8f * seconds_elapsed;
    else if (Input::wasKeyPressed(SDL_SCANCODE_SPACE))
        velocity.y = 3.0f;
    else
        velocity.y = 0.0f;
}