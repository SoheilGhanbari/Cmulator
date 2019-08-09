#include <iostream>
#include "actor.h"
#include <vector>
#include <random>
#include <pthread.h>
#include "repository.h"

using namespace std;

float random_float(float a, float b) {
    // Pick a random number :'|
    int max =10000;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1,max);
    float random = ((float) dis(gen)) / (float) max;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
vector<float> random_bbox(vector<float> bbox) {
    return {random_float(bbox[0],bbox[2]),random_float(bbox[1],bbox[3]),random_float(bbox[0],bbox[2]),random_float(bbox[1],bbox[3])};
}
int main () {
    
    vector<float> bbox;
    bbox = {24.716689, 51.652350, 26.917291, 55.240375};
    actor ship(60, bbox);    
    for (int i=0;i<30;i++){
        vector<float>  actor_bbox = random_bbox(bbox);
        actor *ship = new actor(30, actor_bbox);
        ship->move(i);
    }
    pthread_exit(NULL);
  
}