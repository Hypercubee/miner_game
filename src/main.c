#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include <raylib.h>
#include "../lib/raylib.h" // for autocomplete to recognise the .h

// define ints
typedef unsigned char u8;
typedef char i8;
typedef unsigned short u16;
typedef short i16;
typedef unsigned int u32;
typedef int i32;
typedef unsigned long u64;
typedef long i64;

#include "ores.h"
#include "world.h"
#include "generation.h"

typedef struct{
    Vector2 pos;
    u8 dir;
} Miner;

#include "drawing.h"



void minerMove(Miner* m){
    if(IsKeyPressed(KEY_W)){
        m->pos.y -= 1;
    }
    if(IsKeyPressed(KEY_S)){
        m->pos.y += 1;
    }
    if(IsKeyPressed(KEY_A)){
        m->pos.x -= 1;
    }
    if(IsKeyPressed(KEY_D)){
        m->pos.x += 1;
    }
}

void cameraMove(Vector2* camera){
    if(IsKeyDown(KEY_UP)){
        camera->y -= 1;
    }
    if(IsKeyDown(KEY_DOWN)){
        camera->y += 1;
    }
    if(IsKeyDown(KEY_LEFT)){
        camera->x -= 1;
    }
    if(IsKeyDown(KEY_RIGHT)){
        camera->x += 1;
    }
}

void updateZoom(double *zoom){
    double zoomSpeed = 0.02;
    if(IsKeyDown(KEY_KP_ADD)){
        *zoom *= 1 + zoomSpeed;
    }
    if(IsKeyDown(KEY_KP_SUBTRACT)){
        *zoom *= 1 - zoomSpeed;
    }
    if(*zoom < 0.5) *zoom = 0.5;
    if(*zoom > 2) *zoom = 2;
}




int main(void){
    World gameWorld = {0};
    gameWorld.width = 500;
    gameWorld.height = 500;
    gameWorld.data = malloc(gameWorld.width * gameWorld.height);
    if(gameWorld.data == NULL){
        fprintf(stderr, "failed to allocate memory for world");
        return 1;
    }

    SetRandomSeed(69);
    genOres(gameWorld);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(800, 600, "miner");
    SetTargetFPS(60);

    Miner miner = {
        .pos = (Vector2) {50, 0},
        .dir = 0,
    };

    Vector2 camera = {0, 0};
    double zoom = 2.5;

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);

        cameraMove(&camera);
        minerMove(&miner);
        updateZoom(&zoom);

        drawWorld(gameWorld, miner, camera, zoom);
        DrawLine(0, 0, 800, 600, WHITE);
        DrawLine(800, 0, 0, 600, WHITE);
        EndDrawing();
    }

    CloseWindow();
    free(gameWorld.data);
    return 0;
}
