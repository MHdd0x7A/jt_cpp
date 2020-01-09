#include "../include/SaDE.h"

using namespace std;
SaDE::SaDE(unsigned np, unsigned dim, unsigned LP, unsigned numberOfStrategies, double CR, double F, double epsilion, double* bound_min, double* bound_max):DE::DE(np, dim, CR, F, bound_min, bound_max)
{
    //ctor
    SaDE::LP = LP;
    SaDE::epsilion = epsilion;
    SaDE::numberOfStrategies = numberOfStrategies;
}

SaDE::~SaDE()
{
    //dtor
    free(SaDE::successMemory);
}

/*void SaDE::setBenchmark(Benchmarks* fp){
    SaDE::fp = fp;
}*/

void SaDE::produceNextGeneration(){
    SaDE::selectStrategies();
    for(unsigned np_counter = 0; np_counter < DE::np; np_counter++){
        DE::F = publicFunction::normal_distribution_random(0.5,0.3);
        SaDE::mutantAndCrossover(SaDE::selectedStrategies[np_counter],np_counter,1);
    }
}

void SaDE::selection(){
    double temp_fitness;
    unsigned nowLearn = DE::nowGen%SaDE::LP;
    for(unsigned strategiesCounter = 0; strategiesCounter < SaDE::numberOfStrategies; strategiesCounter++){
        if(DE::nowGen >= SaDE::LP){
            SaDE::calculateStrategiesProbility();
            SaDE::successMemorySUM[strategiesCounter] -= successMemory[strategiesCounter][nowLearn];
            SaDE::failMemorySUM[strategiesCounter] -= failMemory[strategiesCounter][nowLearn];
        }
        SaDE::successMemory[strategiesCounter][nowLearn] = 0;//initial memory record
        SaDE::failMemory[strategiesCounter][nowLearn] = 0;
    }
    for(unsigned np_counter = 0; np_counter < DE::np; np_counter++){
        temp_fitness = DE::fp->compute(DE::nextGeneration[np_counter]);
        if(temp_fitness < DE::fitness[np_counter]){
            SaDE::successMemory[SaDE::selectedStrategies[np_counter]][nowLearn]++;//record how much children are selected
            DE::fitness[np_counter] = temp_fitness;
            free(DE::population[np_counter]);
            DE::population[np_counter] = DE::nextGeneration[np_counter];
            if(temp_fitness < DE::nowbest){
                DE::nowbest = temp_fitness;
                DE::nowBestIndividual = np_counter;
            }
        }
        else{
            SaDE::failMemory[SaDE::selectedStrategies[np_counter]][nowLearn]++;//record how much children are not selected
            free(DE::nextGeneration[np_counter]);//!!important!!
        }
    }
    for(unsigned strategiesCounter = 0; strategiesCounter < SaDE::numberOfStrategies; strategiesCounter++){
        SaDE::successMemorySUM[strategiesCounter] += SaDE::successMemory[strategiesCounter][nowLearn];
        SaDE::failMemorySUM[strategiesCounter] += SaDE::failMemory[strategiesCounter][nowLearn];
    }
    DE::nowGen++;
}

void SaDE::setStrategyID(unsigned strategyID){
    cout<<"SaDE no need to assign 'strategyID'"<<endl;
}

/*void SaDE::setK(double K){
    SaDE::K = K;
}*/

/*double SaDE::getNowBest(){
    return SaDE::nowBest;
}*/

void SaDE::nextRun(){
    DE::ini_population();
    DE::ini_fitness();
    SaDE::ini_Meomory();
}

/*void SaDE::ini_population(){
    SaDE::population = (double**)malloc(sizeof(double*)*SaDE::np);

}for(unsigned np_counter = 0; np_counter < np; np_counter++){
        DE::mutantAndCrossover(DE::strategyID,np_counter,1);
    }

void SaDE::ini_fitness(){
    SaDE::fitness = (double*)malloc(sizeof(double)*SaDE::np);

}*/

void SaDE::ini_Meomory(){
    free(SaDE::successMemory);
    free(SaDE::failMemory);
    free(SaDE::strategiesProbility);
    free(SaDE::selectedStrategies);
    SaDE::successMemory = (unsigned**)malloc(sizeof(unsigned*)*SaDE::numberOfStrategies);
    SaDE::successMemorySUM = (unsigned*)malloc(sizeof(unsigned)*SaDE::numberOfStrategies);
    SaDE::failMemory = (unsigned**)malloc(sizeof(unsigned*)*SaDE::numberOfStrategies);
    SaDE::failMemorySUM = (unsigned*)malloc(sizeof(unsigned)*SaDE::numberOfStrategies);
    SaDE::strategiesProbility = (double*)malloc(sizeof(double)*SaDE::numberOfStrategies);
    SaDE::selectedStrategies = (unsigned*)malloc(sizeof(unsigned)*SaDE::np);
    for(unsigned strategiesCounter = 0; strategiesCounter < SaDE::numberOfStrategies; strategiesCounter++){
        SaDE::successMemory[strategiesCounter] = (unsigned*)malloc(sizeof(unsigned)*SaDE::LP);
        SaDE::successMemorySUM[strategiesCounter] = 0;
        SaDE::failMemory[strategiesCounter] = (unsigned*)malloc(sizeof(unsigned)*SaDE::LP);
        SaDE::failMemorySUM[strategiesCounter] = 0;
        SaDE::strategiesProbility[strategiesCounter] = 1.0 / (double)numberOfStrategies;
    };
}

void SaDE::calculateStrategiesProbility(){
    double allSum = 0;
    for(unsigned strategiesCounter = 0; strategiesCounter < SaDE::numberOfStrategies; strategiesCounter++){
        SaDE::strategiesProbility[strategiesCounter] = SaDE::successMemorySUM[strategiesCounter] / (SaDE::successMemorySUM[strategiesCounter] + SaDE::failMemorySUM[strategiesCounter]) + SaDE::epsilion;
        allSum +=  SaDE::strategiesProbility[strategiesCounter];
    }
    for(unsigned strategiesCounter = 0; strategiesCounter < SaDE::numberOfStrategies; strategiesCounter++){
        SaDE::strategiesProbility[strategiesCounter] /= allSum;
    }
}

void SaDE::selectStrategies(){
    double bias = 1.0/SaDE::np;
    double randomNumber = publicFunction::random_double(bias, 0);
    double probilityUpperBound = SaDE::strategiesProbility[0];
    unsigned nowStrategies = 0;
    for(unsigned npCounter = 0; npCounter < SaDE::np; npCounter++){
        if(randomNumber <= probilityUpperBound){
            SaDE::selectedStrategies[npCounter] = nowStrategies;
            randomNumber += bias;
        }
        else if(randomNumber > probilityUpperBound){
            probilityUpperBound += SaDE::strategiesProbility[++nowStrategies];
        }
    }
}

/*void SaDE::mutantAndCrossover(){

}*/
