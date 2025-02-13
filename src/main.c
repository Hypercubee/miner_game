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


void saveWorld(const char* savename, World world, Miner miner){
    FILE *fp = fopen(savename, "w");
    if(fp == NULL) {
        printf("failed to open save file: %s", strerror(errno));
        return;
    }
    // save world into file
    fwrite(&world.width,  sizeof(int), 1, fp);
    fwrite(&world.height, sizeof(int), 1, fp);
    fwrite(world.data, sizeof(unsigned char), world.width * world.height, fp);
    // save miner
    fwrite(&miner,  sizeof(Miner), 1, fp);
    fclose(fp);
}

void loadWorld(const char *savename, World *world_ptr, Miner *miner_ptr){
    FILE *fp = fopen(savename, "r");
    if(fp == NULL) {
        printf("failed to open save file: %s", strerror(errno));
        return;
    }
    // load world from file
    fread(&world_ptr->width, sizeof(int), 1, fp);
    fread(&world_ptr->height, sizeof(int), 1, fp);
    world_ptr->data = malloc(world_ptr->width * world_ptr->height);
    if(world_ptr->data == NULL){
        fprintf(stderr, "failed to allocate memory for world");
        return;
    }
    fread(world_ptr->data, sizeof(unsigned char), world_ptr->width * world_ptr->height, fp);
    // load miner
    fread(miner_ptr, sizeof(Miner), 1, fp);
    fclose(fp);
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

    CloseWindow();

    uninitWorld(gameWorld);
    return 0;
}
