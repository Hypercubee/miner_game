#ifndef MINER_DRAWING_H
#define MINER_DRAWING_H

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

void drawMiner(int windowWidth, int windowHeight, Miner m, int cellsize){
    int posx; int posy;
    getPosOnScreen(windowWidth, windowHeight, cellsize, m.cam.pos, m.pos.x, m.pos.y, &posx, &posy);
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
