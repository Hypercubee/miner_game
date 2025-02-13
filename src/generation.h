#ifndef MINER_GENERATION_H
#define MINER_GENERATION_H
#include "world.h"
#include "ores.h"

#define worldAt(w, x, y) (w).data[(y) * (w).width + (x)]

int proceduralRandom(int x, int y, int seed) {
    int n = ((unsigned int) x * 374761393) ^ ((unsigned int) y * 668265263) ^ ((unsigned int) seed * 982451653);
    n = (n ^ (n >> 16)) * 0x85ebca6b;
    n = (n ^ (n >> 13)) * 0xc2b2ae35;
    n = n ^ (n >> 16);
    return n;
}

Ores oreAt(int x, int y, int seed){
    int current = proceduralRandom(x, y, seed) % 100;
    Ores ore = ORE_DIRT;
    if(y > 1 && y < 25 && current > 97) ore = ORE_COAL;
    if(y > 25 && y < 40 && current > 97) ore = ORE_IRON;
    if(y > 40 && y < 60 && current > 97) ore = ORE_COPPER;
    if(y > 60 && y < 80 && current > 97) ore = ORE_SILVER;
    if(y > 80 && y < 100 && current > 97) ore = ORE_GOLD;
    return ore;
}

void discoverOresInRadius(World world, int x, int y, int radius, int seed){
    for(int dy = y-radius; dy <= y + radius; dy++) {
        if(dy < 0 || dy > world.height) continue;
        for(int dx = x-radius; dx <= x + radius; dx++) {
            if(dx < 0 || dx > world.width) continue;
            if(worldAt(world, dx, dy) == ORE_UNDISCOVERED){
                worldAt(world, dx, dy) = oreAt(dx, dy, seed);
            }
        }
    }
}

void genBaseWorld(World world){
    for(int y = 0; y < world.height; y++){
        for(int x = 0; x < world.width; x++){
            if(y == 0) worldAt(world, x, y) = ORE_AIR;
            else if (y == world.height-1) worldAt(world, x, y) = ORE_BEDROCK;
            else worldAt(world, x, y) = ORE_UNDISCOVERED;
        }
    }
}

/*
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
            if(y == 0) worldAt(w, x, y) = ORE_AIR;
            else if (y == w.height-1) worldAt(w, x, y) = ORE_BEDROCK;
            else worldAt(w, x, y) = ORE_DIRT;
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
*/
#endif // MINER_GENERATION_H
