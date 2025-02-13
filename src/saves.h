#ifndef MINER_SAVES_H
#define MINER_SAVES_H

#include "world.h"
#include "miner.h"

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

#endif // MINER_SAVES_H
