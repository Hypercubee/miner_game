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
    float baseDrillSpeed;
    int oreDiscoverRadius;
    int seed;
} Miner;

int isMinerMoving = 0.0f;
float minedTime = 0.0f;
Vector2 posBefore;
Vector2 posAfter;
float drillTime = 1.0f;

float lerp(float a, float b, float t){
    return a + (b - a) * t;
}

void minerMove(World w, Miner* m){
    if(isMinerMoving) {
        Vector2 camOffset = {
            m->cam.pos.x - m->pos.x,
            m->cam.pos.y - m->pos.y,
        };
        float dt = GetFrameTime();
        minedTime += dt;
        float t = minedTime >= drillTime ? 1.0f : minedTime / drillTime;
        m->pos.x = lerp(posBefore.x, posAfter.x, t);
        m->pos.y = lerp(posBefore.y, posAfter.y, t);
        m->cam.pos.x = lerp(posBefore.x, posAfter.x, t) + camOffset.x;
        m->cam.pos.y = lerp(posBefore.y, posAfter.y, t) + camOffset.y;
        if(t >= 1.0f) {
            isMinerMoving = 0;
            minedTime = 0.0f;
        }
    } else {
        i8 dirx = 0;
        i8 diry = 0;

        if(IsKeyDown(KEY_W)){
            diry = -1;
        } else if(IsKeyDown(KEY_S)){
            diry = 1;
        } else if(IsKeyDown(KEY_A)){
            dirx = -1;
        } else if(IsKeyDown(KEY_D)){
            dirx = 1;
        }
        if(dirx == 0 && diry == 0) return;
        if(m->pos.x + dirx >= 0 && m->pos.x + dirx < w.width && m->pos.y + diry >= 0 && m->pos.y + diry < w.height){
            u8 ore = worldAt(w, (int)m->pos.x+dirx, (int)m->pos.y+diry);
            if(ore <= m->max_tier){
                isMinerMoving = 1;
                drillTime = m->baseDrillSpeed * (1+ore);
                posBefore = m->pos;
                posAfter.x = m->pos.x + dirx;
                posAfter.y = m->pos.y + diry;
                worldAt(w, (int)m->pos.x + dirx, (int)m->pos.y + diry) = ORE_AIR;
                if(ore > ORE_DIRT){
                    printf("pos x: %d, pos y: %d, mined: %d\n", (int)m->pos.x + dirx, (int)m->pos.y + diry, (int) ore);
                }
            }
        }
        discoverOresInRadius(w, m->pos.x + dirx, m->pos.y + diry, m->oreDiscoverRadius, m->seed);
    }
}

void cameraMove(Miner* m){
    const float speed = 0.2f;
    if(IsKeyDown(KEY_UP)){
        m->cam.pos.y -= speed;
    }
    if(IsKeyDown(KEY_DOWN)){
        m->cam.pos.y += speed;
    }
    if(IsKeyDown(KEY_LEFT)){
        m->cam.pos.x -= speed;
    }
    if(IsKeyDown(KEY_RIGHT)){
        m->cam.pos.x += speed;
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
