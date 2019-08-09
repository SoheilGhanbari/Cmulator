#include <iostream>
#include <vector>
#include "actor.h"
#include <pthread.h>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>
#include "repository.h"

using namespace std;
struct movement
{
    int interval;
    int threadid;
    vector<float> bbox;        
};

vector <string> county_name = {"Iran", "Iraq", "kuwait", "UAE"};
vector <string> ship_type = {"DD", "CL", "CA", "CB", "BB", "BC"};

string get_current_time()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    char output[30];
    strftime(output, 30, "%Y-%m-%dT%H:%M:%S", timeinfo);

    return string(output);
}

actor::actor(int seconds, vector<float> actor_area)
{
    interval = seconds;
    bbox = actor_area;
} 

actor::~actor()
{
}
mutex mu;

void actor::move(int id)
{
    struct movement *data = new movement;
    data->interval = interval;
    data->bbox = bbox;
    data->threadid = id;
    int rc;
    pthread_t thread;
    rc = pthread_create(&thread, NULL, moveRoutine, (void *)data);
    if (rc) {
        cout << "Error: unable to create thread," << rc << endl;
        exit(-1);
    }
    cout << "Theared ID: " << thread << endl;
}
void *actor::moveRoutine (void *data)
{
// Pick a random number :'|
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1,100);


// make thread sleep random milisecond  for acync run 
    this_thread::sleep_for(chrono::milliseconds(dis(gen)*100));


    struct movement *routineData;
    routineData = (struct movement *)data;
    mu.lock();
    cout << "New Start!" << routineData->threadid << endl;
    mu.unlock();

// Pick random cornel of BBOX
    vector<float> starting_point;
    vector<float> ending_point;

    switch (dis(gen)%4)
    {
    case 0:
        starting_point = {routineData->bbox[0], routineData->bbox[1]};
        ending_point = {routineData->bbox[2], routineData->bbox[3]};
        break;
    case 1:
        starting_point = {routineData->bbox[2], routineData->bbox[3]};
        ending_point = {routineData->bbox[0], routineData->bbox[1]};
        break;
    case 2:
        starting_point = {routineData->bbox[0], routineData->bbox[3]};
        ending_point = {routineData->bbox[2], routineData->bbox[1]};
        break;
    case 3:
        starting_point = {routineData->bbox[2], routineData->bbox[1]};
        ending_point = {routineData->bbox[0], routineData->bbox[1]};
        break;
    default:
        starting_point = {routineData->bbox[2], routineData->bbox[1]};
        ending_point = {routineData->bbox[0], routineData->bbox[1]};
        break;
    }
    
    float lat_dif = ending_point[0] - starting_point[0];
    float lon_dif = ending_point[1] - starting_point[1];
    int tune;
    if (lon_dif >= lat_dif)
        tune = (int) 10000*lon_dif;
    else
    {
        tune = (int ) 10000*lat_dif;
    }
    

    int count = 1000 + tune;

    float lat_part = lat_dif/count;
    float lon_part = lon_part/count;
    //create a record 
    for (int i = 1; i < count; i++)
    {
        vector <float> current_point = {starting_point[0]+(i*lat_part), starting_point[1]+(i*lon_part)};
        string current_time = get_current_time();
        string record = "{"
        "\"id\":" + to_string(i+1) + ","
        "\"userId\":" + to_string(routineData->threadid+1) +","
        "\"countryId\":1,"
        "\"shipCode\":2,"
        "\"shipType\":\""+ship_type[dis(gen)%6]+"\","
        "\"countryName\":\""+county_name[dis(gen)%4]+"\","
        "\"messageType\":3,"
        "\"loc\":{"
                    "\"lat\":"+to_string(current_point[0])+","
                    "\"lon\":"+to_string(current_point[1])+""
                "},"
        "\"time\":\""+current_time+"\","
        "\"spetial\":12"
        "}";
        cout << current_time << endl;
        
        // insert record in elastic
        repository *elas = new repository();
        elas->elastics(record); 
        
        //wait for for interval
        this_thread::sleep_for(chrono::milliseconds(routineData->interval*1000));

    }
    

    
    pthread_exit(NULL);
}

// TODO: to make actor move cureved 
vector<float> actor::curveMove (vector<float>, vector<float>)
{
    vector<float> test = {1.2};
    return test;
}


