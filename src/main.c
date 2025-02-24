#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
//#include <raylib.h>
#include "../lib/raylib.h" // for autocomplete to recognise the .h

#include "int_types.h"

#include "ores.h"
#include "world.h"
#include "generation.h"
#include "miner.h"

#include "drawing.h"
#include "saves.h"

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

void drawUI(Miner *m){
    char buff[32] = {0};
    sprintf(buff, "DEPTH: %dm", (int) (5 * m->pos.y));
    DrawText(buff, 20, 20, 20, WHITE);
    sprintf(buff, "%dFPS", GetFPS());
    DrawText(buff, 20, 50, 20, WHITE);
}

void update(World w, Miner *m){
    minerMove(w, m);
    cameraMove(m);
    updateZoom(m);
    drawWorld(w, *m);
    drawUI(m);
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
    loadTextures();

    SetTargetFPS(60);
    SetExitKey(0);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);

        update(gameWorld, &miner);
        EndDrawing();

        if(IsKeyPressed(KEY_K)){
            printf("world saved\n");
            saveWorld("saves/first.save", gameWorld, miner);
        }

        if(IsKeyPressed(KEY_L)){
            printf("world loaded\n");
            uninitWorld(gameWorld);
            loadWorld("saves/first.save", &gameWorld, &miner);
        }
    }

    unloadTextures();
    CloseWindow();
    uninitWorld(gameWorld);
    return 0;
}
