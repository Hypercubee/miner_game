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
    int orePercentages[ORE_UNDISCOVERED + 1] = {0};
#define ADD_ORE(oreType, percent, minY, maxY) if(y >= (minY) && y <= (maxY)) {orePercentages[(oreType)] += (percent);}
    ADD_ORE(ORE_COAL, 5, 1, 20);
    ADD_ORE(ORE_IRON, 5, 20, 40);
    ADD_ORE(ORE_COPPER, 5, 40, 60);
    ADD_ORE(ORE_SILVER, 5, 60, 80);
    ADD_ORE(ORE_GOLD, 5, 80, 100);
#undef ADD_ORE
    int totPercent = 0;
    for(int i = 0; i < ORE_UNDISCOVERED + 1; i++){
        totPercent += orePercentages[i];
    }
    assert(totPercent <= 100);
    int maxPercent = totPercent;
    for(int i = 0; i < ORE_UNDISCOVERED + 1; i++){
        if(maxPercent > current) ore = i;
        maxPercent -= orePercentages[i];
    }
    return ore;
}

void discoverOresInRadius(World world, int x, int y, float radius, int seed){
    float radius_squared = (radius + 0.5) * (radius + 0.5);
    for(int dy = y-radius; dy <= y + radius; dy++) {
        if(dy < 0 || dy > world.height) continue;
        for(int dx = x-radius; dx <= x + radius; dx++) {
            if(dx < 0 || dx > world.width) continue;
            if(worldAt(world, dx, dy) == ORE_UNDISCOVERED){
                if((dx - x) * (dx - x) + (dy - y) * (dy - y) > radius_squared) continue;
                worldAt(world, dx, dy) = oreAt(dx, dy, seed);
            }
        }
    }
}

void genBaseWorld(World world){
    for(int y = 0; y < world.height; y++){
        for(int x = 0; x < world.width; x++){
            if (y == world.height-1) worldAt(world, x, y) = ORE_BEDROCK;
            else if (x == world.width-1) worldAt(world, x, y) = ORE_BEDROCK;
            else if (x == 0) worldAt(world, x, y) = ORE_BEDROCK;
            else if(y == 0) worldAt(world, x, y) = ORE_AIR;
            else worldAt(world, x, y) = ORE_UNDISCOVERED;
        }
    }
}

#endif // MINER_GENERATION_H
