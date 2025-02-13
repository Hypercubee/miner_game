#ifndef MINER_MINER_H
#define MINER_MINER_H

typedef struct{
    Vector2 pos;
    double zoom;
} MinerCamera;

typedef struct{
    Vector2 pos;
    MinerCamera cam;
    u8 dir;
    Ores max_tier;
    int oreDiscoverRadius;
    int seed;
} Miner;


void minerMove(World w, Miner* m){
    i8 dirx = 0;
    i8 diry = 0;

    if(IsKeyPressed(KEY_W)){
        diry = -1;
    } else if(IsKeyPressed(KEY_S)){
        diry = 1;
    } else if(IsKeyPressed(KEY_A)){
        dirx = -1;
    } else if(IsKeyPressed(KEY_D)){
        dirx = 1;
    }
    if(dirx == 0 && diry == 0) return;
    if(m->pos.x + dirx >= 0 && m->pos.x + dirx < w.width && m->pos.y + diry >= 0 && m->pos.y + diry < w.height){
        u8 ore = worldAt(w, (int)m->pos.x+dirx, (int)m->pos.y+diry);
        if(ore <= m->max_tier){
            m->pos.x += dirx;
            m->pos.y += diry;
            m->cam.pos.x += dirx;
            m->cam.pos.y += diry;
            worldAt(w, (int)m->pos.x, (int)m->pos.y) = ORE_AIR;
            if(ore > ORE_DIRT){
                printf("pos x: %d, pos y: %d, mined: %d\n", (int)m->pos.x, (int)m->pos.y, (int) ore);
            }
        }
    }
    discoverOresInRadius(w, m->pos.x, m->pos.y, m->oreDiscoverRadius, m->seed);
}

void cameraMove(Miner* m){
    if(IsKeyDown(KEY_UP)){
        m->cam.pos.y -= 1;
    }
    if(IsKeyDown(KEY_DOWN)){
        m->cam.pos.y += 1;
    }
    if(IsKeyDown(KEY_LEFT)){
        m->cam.pos.x -= 1;
    }
    if(IsKeyDown(KEY_RIGHT)){
        m->cam.pos.x += 1;
    }
}

void updateZoom(Miner* m){
    double zoomSpeed = 0.02;
    if(IsKeyDown(KEY_KP_ADD)){
        m->cam.zoom *= 1 + zoomSpeed;
    }
    if(IsKeyDown(KEY_KP_SUBTRACT)){
        m->cam.zoom *= 1 - zoomSpeed;
    }
    if(m->cam.zoom < 0.4) m->cam.zoom = 0.4;
    if(m->cam.zoom > 2) m->cam.zoom = 2;
}

#endif // MINER_MINER_H
