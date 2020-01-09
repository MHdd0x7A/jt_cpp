#include "../include/publicFunction.h"
using namespace std;
publicFunction::publicFunction()
{
    //ctor
}

publicFunction::~publicFunction()
{
    //dtor
}

double publicFunction::random_double(double _max, double _min){
    return (_max - _min) * rand() / (RAND_MAX + 1.0) + _min;
}

unsigned publicFunction::random_unsigned(unsigned _max, unsigned _min){
    return rand() % (_max - _min  + 1) + _min;
}

unsigned* publicFunction::population_shuffle(unsigned base_individual, unsigned how_much, unsigned np){
    if(how_much > np-1){
        cout<<"can't shuffle more than np-1"<<endl;
        exit(0);
    }
    if(base_individual >= np || base_individual < 0){
        cout<<"base_individual out of population range."<<endl;
        exit(0);
    }
    unsigned* pool = (unsigned*)malloc(sizeof(unsigned)*np-1);
    bool skiped = false;
    for(unsigned ini = 0; ini < np-1; ini++){
        if(ini == base_individual) skiped = true;
        if(skiped)
            pool[ini] = ini+1;
        else
            pool[ini] = ini;
    }
    for(unsigned index = 0; index < how_much; index++){
        swap(pool[index], pool[index + publicFunction::random_unsigned(np - index - 2, 0)]);
    }
    return pool;
}

double normal_distribution_random(double mean, double stddev){
    default_random_engine generator;
    normal_distribution<double> distribution(mean,stddev);
    return distribution(generator);
}
