#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>
#include <signal.h>


typedef struct {
    Vector2 position;
    Vector2 velocity;
    double radius;
    double mass;
} player;


typedef struct {
    Vector2 position;
    double radius;
    double mass;
} planet;


void create_planets(planet* planets, const int number_of_planets){
    double base_radius = 25.0;
    float base_position_x = 100.0;
    float base_position_y = 200.0;
    for (int i=0; i<number_of_planets; ++i) {
        Vector2 position = { base_position_x, base_position_y };
        planets[i].position = position;
        planets[i].radius = base_radius;
        planets[i].mass = 100.0;
        base_radius += 50.0;
        base_position_x += 600.0;
    }
    return;
}


void draw_planets(const planet planets[], const int number_of_planets, const Texture2D texture){
    for (int i=0; i<number_of_planets; ++i) {
        double scale = planets[i].radius / 75.0; // TODO: Not very nice to hardcode 75.0 here
        Vector2 texture_center = {planets[i].position.x-64.0*scale, planets[i].position.y-64.0*scale}; // TODO: Not very nice to hardcode 64.0 here. We do this because the image is 128x128
        DrawTextureEx(texture, texture_center, 0, scale, WHITE);
        DrawCircleLinesV(planets[i].position, planets[i].mass, MAROON);
    }
}


void draw_player(const player* player){
    DrawCircleV(player->position, player->radius, MAROON);
}


void draw_player_velocity(player* player){
    float velocity_vector_x = player->position.x + player->velocity.x * 2000.0;
    float velocity_vector_y = player->position.y + player->velocity.y * 2000.0;
    Vector2 endpoint = { velocity_vector_x, velocity_vector_y };
    DrawLineEx(player->position, endpoint, 4, MAROON);
}


float get_distance_single_vector(Vector2 vector){
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}


double get_distance_between_two_vectors(Vector2 vector, Vector2 vector2){
    double x = vector.x - vector2.x;
    double y = vector.y - vector2.y;

    return sqrt(pow(x,2) + pow(y,2));
}


void draw_stat(char *text, double value, const int stats_position_x, const int stats_position_y, const int position){
    char arr[100];
    sprintf(arr, text, value);
    DrawText(arr, stats_position_x, stats_position_y*position, 20, RAYWHITE);
}


void draw_stats(const player* player, const int screen_width, const int screen_height){
    const int stats_position_x = screen_width - 200;
    const int stats_position_y = 20;

    draw_stat("X: %f", player->position.x, stats_position_x, stats_position_y, 1);
    draw_stat("Y: %f", player->position.y, stats_position_x, stats_position_y, 2);
    draw_stat("Vy: %f", player->velocity.y, stats_position_x, stats_position_y, 3);
    draw_stat("Vx: %f", player->velocity.x, stats_position_x, stats_position_y, 4);

    const float speed = get_distance_single_vector(player->velocity);
    draw_stat("V: %f", speed, stats_position_x, stats_position_y, 5);
}


void update_player_position(player* player, const int screen_width, const int screen_height){
    player->position.x += player->velocity.x;
    player->position.y += player->velocity.y;
}


void update_player_velocity(player* player, const planet planets[], int number_of_planets){
    for (int i=0;i<number_of_planets;++i){
        double z = get_distance_between_two_vectors(player->position, planets[i].position);

        DrawLineV(player->position, planets[i].position, MAROON); // TODO: Move this to a separate function

        if (z < planets[i].mass) {
            double rads = atan2(player->position.y - planets[i].position.y, player->position.x - planets[i].position.x);
            rads += 0.001;

            if (!IsKeyDown(KEY_SPACE))player->velocity.y = (planets[i].position.y + (z * sin(rads))) - player->position.y;
            if (!IsKeyDown(KEY_SPACE))player->velocity.x = (planets[i].position.x + (z * cos(rads))) - player->position.x;
        }
    }
}


int main(void)
{
    const int screen_width = 1480;
    const int screen_height = 850;
    const int number_of_planets = 2;

    Vector2 player_start_position = { (float)screen_width/2, (float)screen_height/2 };
    Vector2 player_start_velocity = { 0.05, 0.005 };

    player player = { player_start_position, player_start_velocity, 5.0, 10.0 };
    planet planets[number_of_planets] = {};
    create_planets(planets, number_of_planets);

    InitWindow(screen_width, screen_height, "SpacerLaser");
    SetWindowMonitor(1);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.position.x + 20.0f, player.position.y + 20.0f };
    camera.offset = (Vector2){ screen_width/2.0f, screen_height/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Texture2D red_planet = LoadTexture("resources/red_planet_large.png");
    //SetTargetFPS(60); //TODO: This should probably be added at some point

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_D)) player.velocity.x += 0.00005f;
        if (IsKeyDown(KEY_A)) player.velocity.x -= 0.00005f;
        if (IsKeyDown(KEY_W)) player.velocity.y -= 0.00005f;
        if (IsKeyDown(KEY_S)) player.velocity.y += 0.00005f;
        
        camera.target = (Vector2){ player.position.x, player.position.y };

        BeginDrawing();
            ClearBackground((Color){ 0, 0, 0, 255 });
            BeginMode2D(camera);

            draw_planets(planets, number_of_planets, red_planet);
            draw_player(&player);
            draw_player_velocity(&player);

            update_player_velocity(&player, planets, number_of_planets);
            update_player_position(&player, screen_width, screen_height);

            draw_stats(&player, screen_width, screen_height);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
