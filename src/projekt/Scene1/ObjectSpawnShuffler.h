#pragma once
#include "src/projekt/Object.h"
#include "src/projekt/Scene.h"

//This object will randomize the spawn positions of the furniture
class ObjectSpawnShuffler {
public:
    //list of objects to be shuffled
    std::list< std::unique_ptr<Object> > objects;
    void shuffle(Scene& scene);
};
