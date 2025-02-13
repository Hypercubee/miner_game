#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include <raylib.h>
#include "../lib/raylib.h" // for autocomplete to recognise the .h

#include "int_types.h"

#include "ores.h"
#include "world.h"
#include "generation.h"
#include "miner.h"

#include "drawing.h"

World initWorld(){
    World gameWorld = {0};
    gameWorld.width = 512;
    gameWorld.height = 512;
    gameWorld.data = malloc(gameWorld.width * gameWorld.height);
    if(gameWorld.data == NULL){
        fprintf(stderr, "failed to allocate memory for world");
        return (World){0};
    }
    genBaseWorld(gameWorld);
    return gameWorld;
}

void uninitWorld(World world){
    free(world.data);
}

void update(World w, Miner *m){
    minerMove(w, m);
    cameraMove(m);
    updateZoom(m);

    drawWorld(w, *m);
}


int main(void){
    World gameWorld = initWorld();
    Miner miner = {
        .pos = (Vector2) {50, 0},
        .cam = (MinerCamera) {
            .pos = (Vector2) {50, 0},
            .zoom = 1
        },
        .dir = 0,
        .max_tier = ORE_COAL,
        .oreDiscoverRadius = 4,
        .seed = 69,
    };
    discoverOresInRadius(gameWorld, miner.pos.x, miner.pos.y, miner.oreDiscoverRadius, miner.seed);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(800, 600, "miner");
    SetTargetFPS(60);
    SetExitKey(0);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);

        update(gameWorld, &miner);

        DrawLine(0, 0, 800, 600, WHITE);
        DrawLine(800, 0, 0, 600, WHITE);
        EndDrawing();
    }

    CloseWindow();

    uninitWorld(gameWorld);
    return 0;
}
