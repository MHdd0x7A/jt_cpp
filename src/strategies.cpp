#include "../include/strategies.h"
using namespace std;
strategies::strategies()
{
    //ctor
    strategies::CR = DBL_MAX;
    strategies::F = DBL_MAX;
    strategies::K = DBL_MAX;
    strategies::np = UINT_MAX;
    strategies::dim = UINT_MAX;
    strategies::bound_max = nullptr;
    strategies::bound_min = nullptr;
}

strategies::~strategies()
{
    //dtor
}

void strategies::setDE_Parameter(double CR, double F, unsigned np, unsigned dim, double* bound_max, double* bound_min){
    strategies::setCR(CR);
    strategies::setF(F);
    strategies::setNP(np);
    strategies::setDim(dim);
    strategies::setBound(bound_max, bound_min);
}

void strategies::setCR(double CR){
    strategies::CR = CR;
}

void strategies::setF(double F){
    strategies::F = F;
}

void strategies::setK(double K){
    strategies::K = K;
}

void strategies::setNP(unsigned np){
    strategies::np = np;
}

void strategies::setDim(unsigned dim){
    strategies::dim = dim;
}

void strategies::setBound(double* bound_max, double* bound_min){
    strategies::bound_max = bound_max;
    strategies::bound_min = bound_min;
}

double* strategies::DE_Rand(double** population, unsigned base_individual, unsigned number_of_vector_differences){
    if(!checkParameter(random)){
        cout<<"parameter unset!!"<<endl;
        exit(0);
    }
    if(strategies::np < number_of_vector_differences*2+1){// (number_of_vector_differences*2+1) are included base individual
        cout<<"number of population is not enough to choose differences individuals"<<endl;
        return nullptr;
    }
    unsigned* individuals = publicFunction::population_shuffle(base_individual, number_of_vector_differences*2+1, strategies::np);//only index below number_of_vector_differences*2+1 are shuffled
    double* trail_individual = (double*)malloc(sizeof(double)*strategies::dim);
    unsigned rnbr = publicFunction::random_unsigned(strategies::dim-1,0);
    for(unsigned dim_counter = 0; dim_counter < strategies::dim; dim_counter++)
        if(publicFunction::random_double((double)1.0, (double)0.0) <= strategies::CR || dim_counter == rnbr){
            trail_individual[dim_counter] = population[individuals[0]][dim_counter];
            for(unsigned vector_differences_counter = 0; vector_differences_counter < number_of_vector_differences; vector_differences_counter++){
                //Vi += F*(Xr[2j] - Xr[2j+1])
                trail_individual[dim_counter] += strategies::F * (population[individuals[1+vector_differences_counter*2]][dim_counter] - population[individuals[1+vector_differences_counter*2+1]][dim_counter]);
                //trail_individual[dim_counter] += F * (population[individuals[1]][dim_counter] - population[individuals[2]][dim_counter]);
            }
            if(trail_individual[dim_counter] < strategies::bound_min[0])
                trail_individual[dim_counter] = strategies::bound_min[0];
            else if(trail_individual[dim_counter] > strategies::bound_max[0])
                trail_individual[dim_counter] = strategies::bound_max[0];
        }
        else
            trail_individual[dim_counter] = population[base_individual][dim_counter];
    free(individuals);
    return trail_individual;
}

double* strategies::DE_best(double** population, unsigned base_individual, unsigned best_individual, unsigned number_of_vector_differences){
    if(!checkParameter(best)){
        cout<<"parameter unset!!"<<endl;
        exit(0);
    }
    if(strategies::np < number_of_vector_differences*2+1){// (number_of_vector_differences*2+1) are included base individual
        cout<<"number of population is not enough to choose differences individuals"<<endl;
        return nullptr;
    }
    unsigned* individuals = publicFunction::population_shuffle(base_individual, number_of_vector_differences*2, strategies::np);//only index below number_of_vector_differences*2+1 are shuffled
    double* trail_individual = (double*)malloc(sizeof(double)*strategies::dim);
    unsigned rnbr = publicFunction::random_unsigned(dim-1,0);
    for(unsigned dim_counter = 0; dim_counter < strategies::dim; dim_counter++)
        if(publicFunction::random_double((double)1.0, (double)0.0) <= strategies::CR || dim_counter == rnbr){
            trail_individual[dim_counter] = population[best_individual][dim_counter];
            for(unsigned vector_differences_counter = 0; vector_differences_counter < number_of_vector_differences; vector_differences_counter++){
                //Vi += F*(Xr[2j] - Xr[2j+1])
                trail_individual[dim_counter] += strategies::F * (population[individuals[vector_differences_counter*2]][dim_counter] - population[individuals[vector_differences_counter*2+1]][dim_counter]);
                //trail_individual[dim_counter] += F * (population[individuals[1]][dim_counter] - population[individuals[2]][dim_counter]);
            }
            if(trail_individual[dim_counter] < strategies::bound_min[0])
                trail_individual[dim_counter] = strategies::bound_min[0];
            else if(trail_individual[dim_counter] > strategies::bound_max[0])
                trail_individual[dim_counter] = strategies::bound_max[0];
        }
        else
            trail_individual[dim_counter] = population[base_individual][dim_counter];
    free(individuals);
    return trail_individual;
}

double* strategies::DE_currentToRand(double** population, unsigned base_individual, unsigned number_of_vector_differences){
    if(!checkParameter(currentToRand)){
        cout<<"parameter unset!!"<<endl;
        exit(0);
    }
    if(strategies::np < number_of_vector_differences*2+1+1){// (number_of_vector_differences*2+1+1) are included base individual and Xr[2n+1]
        cout<<"number of population is not enough to choose differences individuals"<<endl;
        return nullptr;
    }
    unsigned* individuals = publicFunction::population_shuffle(base_individual, number_of_vector_differences*2+1, strategies::np);//only index below number_of_vector_differences*2+1 are shuffled
    double* trail_individual = (double*)malloc(sizeof(double)*strategies::dim);
    for(unsigned dim_counter = 0; dim_counter < strategies::dim; dim_counter++){
        //Vi = Xi + F*(Xr[2n+1]-Xi)
        trail_individual[dim_counter] = population[base_individual][dim_counter] + strategies::K * (population[individuals[number_of_vector_differences*2]][dim_counter] - population[base_individual][dim_counter]);
        for(unsigned vector_differences_counter = 0; vector_differences_counter < number_of_vector_differences; vector_differences_counter++){
            //Vi += F*(Xr[2j] - Xr[2j+1])
            trail_individual[dim_counter] += strategies::F * (population[individuals[vector_differences_counter*2]][dim_counter] - population[individuals[vector_differences_counter*2+1]][dim_counter]);
        }
        if(trail_individual[dim_counter] < strategies::bound_min[0])
            trail_individual[dim_counter] = strategies::bound_min[0];
        else if(trail_individual[dim_counter] > strategies::bound_max[0])
            trail_individual[dim_counter] = strategies::bound_max[0];
    }
    free(individuals);
    return trail_individual;
}

double* strategies::DE_CurrentToBest(double** population, unsigned base_individual, unsigned best_individual, unsigned number_of_vector_differences){
    if(!checkParameter(currentToBest)){
        cout<<"parameter unset!!"<<endl;
        exit(0);
    }
    if(strategies::np < number_of_vector_differences*2+1){// (number_of_vector_differences*2+1) are included base individual
        cout<<"number of population is not enough to choose differences individuals"<<endl;
        return nullptr;
    }
    unsigned* individuals = publicFunction::population_shuffle(base_individual, number_of_vector_differences*2, strategies::np);//only index below number_of_vector_differences*2+1 are shuffled
    double* trail_individual = (double*)malloc(sizeof(double)*strategies::dim);
    unsigned rnbr = publicFunction::random_unsigned(strategies::dim-1,0);
    for(unsigned dim_counter = 0; dim_counter < strategies::dim; dim_counter++)
        if(publicFunction::random_double(1.0, 0.0) <= strategies::CR || dim_counter == rnbr){
            //Vi = Xi + F*(Xbest-Xi)
            trail_individual[dim_counter] = population[base_individual][dim_counter] + strategies::F * (population[best_individual][dim_counter] - population[base_individual][dim_counter]);
            for(unsigned vector_differences_counter = 0; vector_differences_counter < number_of_vector_differences; vector_differences_counter++){
                //Vi += F*(Xr[2j] - Xr[2j+1])
                trail_individual[dim_counter] += strategies::F * (population[individuals[vector_differences_counter*2]][dim_counter] - population[individuals[vector_differences_counter*2+1]][dim_counter]);
            }
            if(trail_individual[dim_counter] < strategies::bound_min[0])
                trail_individual[dim_counter] = strategies::bound_min[0];
            else if(trail_individual[dim_counter] > strategies::bound_max[0])
                trail_individual[dim_counter] = strategies::bound_max[0];
        }
        else
            trail_individual[dim_counter] = population[base_individual][dim_counter];
    free(individuals);
    return trail_individual;
}

bool strategies::checkParameter(unsigned strategyID){
    if(strategyID <= DE_strategyMaxID && strategyID >= DE_strategyMinID){
        if(strategyID == currentToRand)
            if(strategies::K == DBL_MAX)
                return false;
        if(strategies::CR == DBL_MAX)
            return false;
        if(strategies::F == DBL_MAX)
            return false;
        if(strategies::np == UINT_MAX)
            return false;
        if(strategies::dim == UINT_MAX)
            return false;
        if(strategies::bound_max == nullptr)
            return false;
        if(strategies::bound_min == nullptr)
            return false;
        return true;
    }else
        return false;
}
