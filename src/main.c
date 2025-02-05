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

typedef struct {
    int width;
    int height;
    unsigned char* data;
} World;

#define worldAt(w, x, y) (w).data[(y) * (w).width + (x)]

typedef enum{
    ORE_AIR,
    ORE_DIRT,
    ORE_COAL,
    ORE_IRON,
    ORE_COPPER,
    ORE_SILVER,
    ORE_GOLD
} Ores;


Color getOreColor(Ores ore){
    switch(ore){
    case ORE_AIR:
        return (Color){50, 160, 200, 255};
        break;
    case ORE_DIRT:
        return (Color){40, 25, 10, 255};
        break;
    case ORE_COAL:
        return (Color){12, 12, 12, 255};
        break;
    case ORE_IRON:
        return (Color){50, 50, 50, 255};
        break;
    case ORE_COPPER:
        return (Color){160, 50, 10, 255};
        break;
    case ORE_SILVER:
        return (Color){140, 140, 140, 255};
        break;
    case ORE_GOLD:
        return (Color){180, 170, 15, 255};
        break;
    default:
        assert(0 && "ores color not implemented");
    }
}

void genOre(World w, int ymin, int ymax, int percent, u8 material){
    for(int y = ymin; y < ymax || y < w.height; y++){
        for(int x = 0; x < w.width; x++){
            int rand = GetRandomValue(0, 100);
            if(y > ymin && y < ymax && rand > 100-percent) worldAt(w, x, y) = material;
        }
    }
}

void genOres(World w){
    for(int y = 0; y < w.height; y++){
        for(int x = 0; x < w.width; x++){
            worldAt(w, x, y) = ORE_DIRT;
        }
    }
    genOre(w, 0, 20, 5, ORE_COAL);
    genOre(w, 20, 30, 3, ORE_COAL);
    genOre(w, 20, 30, 3, ORE_IRON);
    genOre(w, 30, 50, 5, ORE_IRON);
    genOre(w, 50, 60, 3, ORE_IRON);
    genOre(w, 50, 60, 3, ORE_COPPER);
    genOre(w, 60, 80, 5, ORE_COPPER);
    genOre(w, 80, 100, 5, ORE_SILVER);
    genOre(w, 100, 120, 5, ORE_GOLD);
}

typedef struct{
    Vector2 pos;
    u8 dir;
} Miner;

void minerMove(Miner* m){
    if(IsKeyDown(KEY_W)){
        m->pos.y -= 1;
    }
    if(IsKeyDown(KEY_S)){
        m->pos.y += 1;
    }
    if(IsKeyDown(KEY_A)){
        m->pos.x -= 1;
    }
    if(IsKeyDown(KEY_D)){
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

void getPosOnScreen(int windowWidth, int windowHeight, int cellsize, Vector2 cam, int worldx, int worldy, int* outx, int* outy){
    *outx = (cellsize*(2*worldx - 1 - 2*cam.x) + windowWidth)/2;
    *outy = (cellsize*(2*worldy - 1 - 2*cam.y) + windowHeight)/2;
}

void drawTerrain(World world, int windowWidth, int windowHeight, Vector2 cam, int cellsize){
    int drawcountw = windowWidth / cellsize + 2;
    int drawcounth = windowHeight / cellsize + 2;
    for(int y = cam.y - drawcounth/2; y < drawcounth + cam.y; y++){
        for(int x = cam.x - drawcountw/2; x < drawcountw + cam.x; x++){
            if(x < 0 || x >= world.width || y < 0 || y >= world.height) continue;
            int posx = (cellsize*(2*x - 1 - 2*cam.x) + windowWidth)/2;
            int posy = (cellsize*(2*y - 1 - 2*cam.y) + windowHeight)/2;
            Color c = getOreColor(worldAt(world, x, y));
            DrawRectangle(posx, posy, cellsize, cellsize, c);
        }
    }
}

void drawMiner(int windowWidth, int windowHeight, Miner m, Vector2 cam, int cellsize){
    int posx;
    int posy;
    getPosOnScreen(windowWidth, windowHeight, cellsize, cam, m.pos.x, m.pos.y, &posx, &posy);
    DrawRectangle(posx, posy, cellsize, cellsize, WHITE);
}

void drawWorld(World w, Miner m, Vector2 cam, double zoom){
    const int WINDOW_WIDTH = GetRenderWidth();
    const int WINDOW_HEIGHT = GetRenderHeight();
    const int cellsize = 50 * zoom;
    drawTerrain(w, WINDOW_WIDTH, WINDOW_HEIGHT, cam, cellsize);
    drawMiner(WINDOW_WIDTH, WINDOW_HEIGHT, m, cam, cellsize);
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
        .pos = (Vector2) {3, 3},
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
