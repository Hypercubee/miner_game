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
    double zoom;
} MinerCamera;

typedef struct{
    Vector2 pos;
    MinerCamera cam;
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

void cameraMove(Miner* m){
    if(IsKeyDown(KEY_UP)){
        m->cam.pos.y -= 1;
    }
    if(IsKeyDown(KEY_DOWN)){
        m->cam.pos.y += 1;
    }
    if(IsKeyDown(KEY_LEFT)){
        m->cam.pos.x -= 1;
    }
    if(IsKeyDown(KEY_RIGHT)){
        m->cam.pos.x += 1;
    }
}

void updateZoom(Miner* m){
    double zoomSpeed = 0.02;
    if(IsKeyDown(KEY_KP_ADD)){
        m->cam.zoom *= 1 + zoomSpeed;
    }
    if(IsKeyDown(KEY_KP_SUBTRACT)){
        m->cam.zoom *= 1 - zoomSpeed;
    }
    if(m->cam.zoom < 0.5) m->cam.zoom = 0.5;
    if(m->cam.zoom > 2) m->cam.zoom = 2;
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
        .cam = (MinerCamera) {
            .pos = (Vector2) {50, 0},
            .zoom = 1
        },
        .dir = 0,
    };

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);

        minerMove(&miner);
        cameraMove(&miner);
        updateZoom(&miner);

        drawWorld(gameWorld, miner);
        DrawLine(0, 0, 800, 600, WHITE);
        DrawLine(800, 0, 0, 600, WHITE);
        EndDrawing();
    }

    CloseWindow();
    free(gameWorld.data);
    return 0;
}
