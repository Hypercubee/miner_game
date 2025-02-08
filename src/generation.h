#ifndef MINER_GENERATION_H
#define MINER_GENERATION_H
#include "ores.h"

#define worldAt(w, x, y) (w).data[(y) * (w).width + (x)]


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
#endif // MINER_GENERATION_H
