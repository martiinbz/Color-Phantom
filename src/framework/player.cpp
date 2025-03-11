#include "framework/player.h"
#include "framework/world.h"
#include "framework/input.h"
#include "framework/camera.h"
#include "graphics/mesh.h"
#include "graphics/material.h"
#include "framework/entities/entity_collider.h"
#include "game/game.h"
#include "framework/animation.h"
Player* Player::instance = NULL;


Player::Player(Mesh* mesh, const Material& material, const std::string& name)
    : EntityMesh(mesh, material) {
    walk_speed = 2.0f;

    //cargamos el player_texture shader
    player_shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/player_texture.fs");
    instance = this;
    

    isAnimated = true;

    animator.playAnimation("data/animations/idle.skanim");

    if (Game::instance->level == 1) {
        model.setTranslation(Vector3(0, 0.00963535, -17.9041));
        model.scale(0.8, 0.8, 0.8);
    }
    else if (Game::instance->level == 2) {
        model.setTranslation(Vector3(-0.8243, 0.00978039, 9.12383));
        model.scale(0.8, 0.8, 0.8);
    }
    else if (Game::instance->level == 3) {
        model.setTranslation(Vector3(2.94316, 1.00970833, 17.7309));
        model.setScale(0.2, 0.2, 0.2);
    }

}


void Player::render(Camera* camera) {

   

   
    

    // Ajustar la escala según la tecla presionada
     // Tomamos la escala en X como referencia
	//como el jugador usa otro fs diferente, se debe cambiar el shader.
    if (!mesh || !player_shader) return;
    

    player_shader->enable();


    player_shader->setUniform("u_model", getGlobalMatrix());
    player_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
  

    // enviamos el color actual al shader
    player_shader->setUniform("u_playerColor", Vector4(current_color, 1.0f));
    
  

    
    mesh->renderAnimated(GL_TRIANGLES,&animator.getCurrentSkeleton());

    player_shader->disable();
	

    //EntityMesh::render(camera);
}


void Player::update(float seconds_elapsed) {

    
    float camera_yaw = World::get_instance()->camera_yaw;

    Matrix44 mYaw;
    mYaw.setRotation(camera_yaw, Vector3(0, 1, 0));

    Vector3 front = mYaw.frontVector();
    Vector3 right = mYaw.rightVector();
    Vector3 position = model.getTranslation(); 
    Vector3 move_dir;

    //WASD keys
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
        Audio::Play("data/audio/SONIDO_ABSORBER.mp3", 1.0f, BASS_SAMPLE_MONO);
        if (Game::instance->level == 3) {
            target_color = Vector3(1,1,1)-World::get_instance()->looking_color;

        }
        else target_color = World::get_instance()->looking_color;

        //cambiar el color progresivamente 
        if((current_color-target_color).length()> 0.01f){
            current_color = current_color * (1.0f - seconds_elapsed * 0.4) + target_color * seconds_elapsed * 0.4;
        }
        else {
			current_color = target_color;
        }
        
    }

    //REINICIAR COLOR A BLANCO
    if (Input::isKeyPressed(SDL_SCANCODE_C)) {

        current_color = Vector3(1, 1, 1);
    }

    // Definir los límites de escala
    const float max_scale = 2.5f;  // Tamaño máximo del jugador
    const float min_scale = 0.4f;  // Tamaño mínimo del jugador
    const float scale_speed = 0.5f; // Velocidad de escalado

   

    if (Input::isKeyPressed(SDL_SCANCODE_Q)) {
        new_scale += scale_speed * seconds_elapsed;
    }
    if (Input::isKeyPressed(SDL_SCANCODE_E)) {
        new_scale -= scale_speed * seconds_elapsed;
    }

    // Limitar la escala dentro de los valores permitidos
    if (new_scale > max_scale) {
        new_scale = max_scale;
    }
	else if (new_scale < min_scale) {
		new_scale = min_scale;
    }
	
    

    
    //correr con shift
    float speed_mult = walk_speed;
    if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT))
        speed_mult *= 3.0f;  // Correr

  
    move_dir.normalize();
    move_dir *= speed_mult;

    velocity = move_dir;

    //colisiones del jugador
    test_collisions(position, seconds_elapsed);

    // aplicar el movimiento
    position += velocity * seconds_elapsed;

    
    model.setTranslation(position);
    model.scale(new_scale, new_scale, new_scale);
    //rotar al jugador con la cámara
    model.rotate(camera_yaw, Vector3(0, 1, 0));

	
   
	
		

    bool is_moving = move_dir.length() > 0.01f;
    if (is_moving) {
        if (animation_state != eAnimationState::WALK) {
            animation_state = eAnimationState::WALK;
            animator.playAnimation("data/animations/walking.skanim");
        }
		
		
    }
    
    else {
        if (animation_state != eAnimationState::IDLE) {
            animation_state = eAnimationState::IDLE;
            animator.playAnimation("data/animations/idle.skanim");
        }
    }
    


    animator.update(seconds_elapsed);

    
    EntityMesh::update(seconds_elapsed);
    

}

void Player::test_collisions(Vector3& position, float seconds_elapsed) {
    std::vector<sCollisionData> collisions;
    std::vector<sCollisionData> ground_collisions;

    World::get_instance()->test_scene_collisions(position + velocity * seconds_elapsed, collisions, ground_collisions);

    
    for (const sCollisionData& collision : collisions) {

        //mirar si hay colision de suelo
        float up_vector = fabsf(collision.col_normal.dot(Vector3::UP));
        if (up_vector > 0.8)
            continue;
		
        float collisionFactor = velocity.dot(collision.col_normal);
        if (collisionFactor > 0.0f) {
			//deteneos la velocidad en la dirección de la colisión
            velocity -= collisionFactor * collision.col_normal;  
        }
        Vector3 newDir = velocity.dot(collision.col_normal) * collision.col_normal;
        
        velocity.x -= newDir.x;
        velocity.y -= newDir.y;
    }

    // colisiones de suelo
    bool is_grounded = false;
    for (const sCollisionData& collision : ground_collisions) {
        float up_vector = fabsf(collision.col_normal.dot(Vector3::UP));
        if (up_vector > 0.8 && (position.y - collision.col_point.y)< 0.01f)  //miramos tambien que el jugador esté pegado al suelo
			// si está en mitad de un salto, is_grounded = false para que se le aplique la gravedad y caiga
            is_grounded = true;
		
        
    }
	

    /*if (!is_grounded)
        velocity.y -= 9.8f * seconds_elapsed;
    else if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
        velocity.y += 50.0f;
    }*/
      
}

