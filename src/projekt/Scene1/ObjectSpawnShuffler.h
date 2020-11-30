#pragma once
#include "src/projekt/Object.h"
#include "src/projekt/Scene.h"

class ObjectSpawnShuffler {
public:
    std::list< std::unique_ptr<Object> > objects;
    void shuffle(Scene& scene);
};
