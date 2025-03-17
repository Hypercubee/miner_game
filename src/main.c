#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
//#include <raylib.h>
#include "../include/raylib.h" // for autocomplete to recognise the .h

#include "int_types.h"

#include "ores.h"
#include "world.h"
#include "generation.h"
#include "miner.h"

#include "drawing.h"
#include "saves.h"

typedef enum{
    MENU_START,
    MENU_GAME
} Menu;

Menu menu = MENU_START;

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
    DrawText(TextFormat("DEPTH: %dm", (int) (5 * m->pos.y)), 20, 20, 20, WHITE);
    DrawText(TextFormat("%dFPS", GetFPS()), 20, 50, 20, WHITE);

}

void update(World w, Miner *m){
    minerMove(w, m);
    cameraMove(m);
    updateZoom(m);
    drawWorld(w, *m);
    drawUI(m);
}

void runGame(World *gameWorld, Miner *miner){
    update(*gameWorld, miner);

    if(IsKeyPressed(KEY_K)){
        printf("world saved\n");
        saveWorld("saves/first.save", *gameWorld, *miner);
    }

    if(IsKeyPressed(KEY_L)){
        printf("world loaded\n");
        uninitWorld(*gameWorld);
        loadWorld("saves/first.save", gameWorld, miner);
    }
}

void runStartMenu(){
    const int WINDOW_WIDTH = GetRenderWidth();
    const int WINDOW_HEIGHT = GetRenderHeight();
    DrawText(TextFormat("%dFPS", GetFPS()), 20, 20, 20, WHITE);
    const int size = WINDOW_HEIGHT / 5;
    const char* name = "Miner";
    const char* play = "Press Enter to play";

    int nameWidth = MeasureText(name, size);
    int playWidth = MeasureText(play, size / 2);

    DrawText(name, WINDOW_WIDTH / 2 - nameWidth / 2, size, size, BROWN);

    DrawText(play, WINDOW_WIDTH / 2 - playWidth / 2, size * 4, size / 2, GRAY);
    if(IsKeyPressed(KEY_ENTER)){
        menu = MENU_GAME;
    }

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
        .max_tier = ORE_GOLD,
        .baseDrillSpeed = 1.0,
        .oreDiscoverRadius = 4,
        .seed = 69,
    };
    discoverOresInRadius(gameWorld, miner.pos.x, miner.pos.y, miner.oreDiscoverRadius, miner.seed);
    SetTraceLogLevel(LOG_WARNING);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "miner");
    loadTextures();

    SetTargetFPS(60);
    SetExitKey(0);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        switch(menu){
        case MENU_START:
            runStartMenu();
            break;
        case MENU_GAME:
            runGame(&gameWorld, &miner);
            break;
        default:
            assert(0 && "menu not implemented");
        }
        EndDrawing();
    }

    unloadTextures();
    CloseWindow();
    uninitWorld(gameWorld);
    return 0;
}
