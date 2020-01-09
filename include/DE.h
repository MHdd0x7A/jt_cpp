#ifndef DE_H
#define DE_H

#include "../Benchmarks.h"
#include "strategies.h"
#include "publicFunction.h"

class DE
{
    public:
        DE(unsigned,unsigned,double,double,double*,double*);
        virtual ~DE();
        void setBenchmark(Benchmarks*);
        virtual void produceNextGeneration();
        virtual void selection();
        void setK(double);
        virtual void setStrategyID(unsigned);
        double getNowBest();
        virtual void nextRun();
    protected:
        void ini_population();
        void ini_fitness();
        void mutantAndCrossover(unsigned,unsigned,unsigned);
        double **population = nullptr, *fitness, **nextGeneration;
        double F, CR, K;
        double nowbest = DBL_MAX;
        unsigned np, dim, strategyID;
        unsigned nowBestIndividual, nowGen;
        double *bound_min, *bound_max;
        Benchmarks* fp;
        strategies* ss;
    private:
};

#endif // DE_H
