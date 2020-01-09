#include "../include/DE.h"

DE::DE(unsigned np, unsigned dim,double CR, double F, double* bound_min, double* bound_max)
{
    //ctor
    DE::np = np;
    DE::nextGeneration = (double**)malloc(sizeof(double*)*DE::np);
    DE::dim = dim;
    DE::bound_max = bound_max;
    DE::bound_min = bound_min;
    DE::CR = CR;
    DE::F = F;
    DE::ss = new strategies();
    DE::ss->setDE_Parameter(CR,F,np,dim,bound_max,bound_min);
}

DE::~DE()
{
    //dtor
    free(population);
    free(fitness);
    free(nextGeneration);
    delete ss;
}

void DE::setBenchmark(Benchmarks* fp){
    DE::fp = fp;
}

void DE::ini_population(){
    DE::nowGen = 0;
    free(population);
    DE::population = (double**)malloc(sizeof(double*) * DE::np);
    //double* population[np] = {new double[dim]};//create np population and each population has dim data
    for(unsigned np_counter = 0; np_counter < DE::np; np_counter++){//ini
        DE::population[np_counter] = (double*)malloc(sizeof(double) * DE::dim);
        for(unsigned dim_counter = 0; dim_counter < DE::dim; dim_counter ++){
            DE::population[np_counter][dim_counter] = publicFunction::random_double(bound_max[0],bound_min[0]);
        }
    }
}

void DE::ini_fitness(){
    delete []DE::fitness;
    DE::fitness = new double[DE::np];
    DE::nowbest = DBL_MAX;
    for(unsigned np_counter = 0; np_counter < DE::np; np_counter++){
        DE::fitness[np_counter] = DE::fp->compute(DE::population[np_counter]);
        if(DE::fitness[np_counter] < DE::nowbest){
            DE::nowbest = DE::fitness[np_counter];
            DE::nowBestIndividual = np_counter;
        }
    }
}

void DE::setK(double K){
    DE::K = K;
    DE::ss->setK(K);
}

void DE::setStrategyID(unsigned strategyID){
    DE::strategyID = strategyID;
}

void DE::produceNextGeneration(){
    for(unsigned np_counter = 0; np_counter < np; np_counter++){
        DE::mutantAndCrossover(DE::strategyID,np_counter,1);
    }
}

void DE::mutantAndCrossover(unsigned strategyID, unsigned base_individual,unsigned number_of_vector_differences){
    if(strategyID == random){
        DE::nextGeneration[base_individual] = ss->DE_Rand(DE::population, base_individual, number_of_vector_differences);
    }else if(strategyID == best){
        DE::nextGeneration[base_individual] = ss->DE_best(DE::population, base_individual, DE::nowBestIndividual, number_of_vector_differences);
    }else if(strategyID == currentToRand){
        DE::nextGeneration[base_individual] = ss->DE_currentToRand(DE::population, base_individual, number_of_vector_differences);
    }else if(strategyID == currentToBest){
        DE::nextGeneration[base_individual] = ss->DE_CurrentToBest(DE::population, base_individual, DE::nowBestIndividual, number_of_vector_differences);
    }
}

void DE::selection(){
    double temp_fitness;
    for(unsigned np_counter = 0; np_counter < DE::np; np_counter++){
        temp_fitness = DE::fp->compute(DE::nextGeneration[np_counter]);
        if(temp_fitness < DE::fitness[np_counter]){
            DE::fitness[np_counter] = temp_fitness;
            free(DE::population[np_counter]);
            DE::population[np_counter] = DE::nextGeneration[np_counter];
            if(temp_fitness < DE::nowbest){
                DE::nowbest = temp_fitness;
                DE::nowBestIndividual = np_counter;
            }
        }
        else
            free(DE::nextGeneration[np_counter]);//!!important!!
    }
    DE::nowGen++;
}

double DE::getNowBest(){
    return nowbest;
}

void DE::nextRun(){
    DE::ini_population();
    DE::ini_fitness();
}
