#ifndef MINER_ORES_H
#define MINER_ORES_H

typedef enum{
    ORE_AIR,
    ORE_DIRT,
    ORE_COAL,
    ORE_IRON,
    ORE_COPPER,
    ORE_SILVER,
    ORE_GOLD,
    ORE_BEDROCK
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
    case ORE_BEDROCK:
        return (Color){5, 5, 5, 255};
        break;
    default:
        assert(0 && "ores color not implemented");
    }
}

#endif // MINER_ORES_H
