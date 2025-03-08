#ifndef MINER_DRAWING_H
#define MINER_DRAWING_H

void getPosOnScreen(int windowWidth, int windowHeight, int cellsize, Vector2 cam, int worldx, int worldy, int* outx, int* outy){
    *outx = (cellsize*(2*worldx - 1 - 2*cam.x) + windowWidth)/2;
    *outy = (cellsize*(2*worldy - 1 - 2*cam.y) + windowHeight)/2;
}

void getPosOnScreenf(int windowWidth, int windowHeight, int cellsize, Vector2 cam, float worldx, float worldy, int* outx, int* outy){
    *outx = ((float) cellsize*(2.0*worldx - 1 - 2*cam.x) + windowWidth)/2;
    *outy = ((float) cellsize*(2.0*worldy - 1 - 2*cam.y) + windowHeight)/2;
}

Texture2D textures[32];
int loadedTextureCount = 0;
const int IMAGE_COUNT = 7;
const char *imageFileNames[] = {
    "assets/miner_dirt.png",
    "assets/miner_coal.png",
    "assets/miner_iron.png",
    "assets/miner_copper.png",
    "assets/miner_silver.png",
    "assets/miner_gold.png",
    "assets/miner_bedrock.png",
};

void loadTextures(){
    for(int i = 0; i < IMAGE_COUNT; i++){
        textures[i] = LoadTexture(imageFileNames[i]);
        loadedTextureCount++;
    }
};

void unloadTextures(){
    for(int i = 0; i < loadedTextureCount; i++){
        UnloadTexture(textures[i]);
    }
}

int getOreTextureIndex(Ores ore){
    switch(ore){
    case ORE_AIR:
        return -1;
    case ORE_DIRT:
        return 0;
    case ORE_COAL:
        return 1;
    case ORE_IRON:
        return 2;
    case ORE_COPPER:
        return 3;
    case ORE_SILVER:
        return 4;
    case ORE_GOLD:
        return 5;
    case ORE_BEDROCK:
        return 6;
    case ORE_UNDISCOVERED:
        return -2;
    default:
        assert(0 && "ores texture not implemented");
    }
}


void drawTerrain(World world, int windowWidth, int windowHeight, Vector2 cam, int cellsize){
    int drawcountw = windowWidth / cellsize + 2;
    int drawcounth = windowHeight / cellsize + 2;
    for(int y = cam.y - drawcounth/2; y < drawcounth + cam.y; y++){
        for(int x = cam.x - drawcountw/2; x < drawcountw + cam.x; x++){
            if(x < 0 || x >= world.width || y < 0 || y >= world.height) continue;
            int posx = (cellsize*(2*x - 1 - 2*cam.x) + windowWidth)/2;
            int posy = (cellsize*(2*y - 1 - 2*cam.y) + windowHeight)/2;

            int index = getOreTextureIndex(worldAt(world, x, y));
            if(index < 0){
                Color c = getOreColor(worldAt(world, x, y));
                DrawRectangle(posx, posy, cellsize, cellsize, c);
            } else {
                Texture2D tex = textures[index];
                DrawTexturePro(tex, (Rectangle) {0, 0, 32, 32}, (Rectangle) {posx, posy, cellsize, cellsize}, (Vector2) {0, 0}, 0, WHITE);
            }
        }
    }
}

void drawMiner(int windowWidth, int windowHeight, Miner m, int cellsize){
    int posx; int posy;
    getPosOnScreenf(windowWidth, windowHeight, cellsize, m.cam.pos, m.pos.x, m.pos.y, &posx, &posy);
    DrawRectangle(posx+cellsize*0.2, posy+cellsize*0.2, cellsize*0.6, cellsize*0.6, WHITE);
}

void drawWorld(World w, Miner m){
    const int WINDOW_WIDTH = GetRenderWidth();
    const int WINDOW_HEIGHT = GetRenderHeight();
    const int cellsize = 50 * m.cam.zoom;
    drawTerrain(w, WINDOW_WIDTH, WINDOW_HEIGHT, m.cam.pos, cellsize);
    drawMiner(WINDOW_WIDTH, WINDOW_HEIGHT, m, cellsize);
}
#endif // MINER_DRAWING_H
