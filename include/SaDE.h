#ifndef SADE_H
#define SADE_H

#include "DE.h"
#include "../Benchmarks.h"
#include "strategies.h"
#include <random>

class SaDE : public DE
{
    public:
        SaDE(unsigned,unsigned,unsigned,unsigned,double,double,double,double*,double*);
        virtual ~SaDE();
        //void setBenchmark(Benchmarks*);
        void produceNextGeneration() override;
        void selection() override;
        void setStrategyID(unsigned) override;
        //void setK(double);
        //double getNowBest();
        void nextRun() override;
    protected:
        //void ini_population();
        //void ini_fitness();
        void ini_Meomory();
        void selectStrategies();
        void calculateStrategiesProbility();
        //void mutantAndCrossover();
        unsigned** successMemory, **failMemory, *successMemorySUM, *failMemorySUM, *selectedStrategies, numberOfStrategies;
        double *strategiesProbility, *CRm, *fitness, *bound_min, *bound_max;
        double **CRMomery,**population, **nextGeneration;
        double CR, F, K, epsilion, nowBest = DBL_MAX;
        unsigned dim, LP, nowBestIndividual;
        //Benchmarks* fp;
        //strategies* ss;
    private:
};

#endif // SADE_H
