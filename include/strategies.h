#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "publicFunction.h"
#include <unistd.h>
#include <cfloat>
#define DE_strategyMinID    0
#define random          0
#define best            1
#define currentToRand   2
#define currentToBest   3
#define DE_strategyMaxID    3

class strategies
{
    public:
        strategies();
        virtual ~strategies();
        void setDE_Parameter(double,double,unsigned,unsigned,double*,double*);
        void setCR(double);
        void setF(double);
        void setK(double);
        void setNP(unsigned);
        void setDim(unsigned);
        void setBound(double*,double*);
        double* DE_CurrentToBest(double**,unsigned,unsigned,unsigned number_of_vector_differences = 1);
        double* DE_currentToRand(double**,unsigned,unsigned number_of_vector_differences = 1);
        double* DE_Rand(double**,unsigned,unsigned number_of_vector_differences = 1);
        double* DE_best(double**,unsigned,unsigned,unsigned number_of_vector_differences = 1);

    protected:
        bool checkParameter(unsigned);
        double CR, F, K;
        unsigned np,dim;
        double *bound_min,*bound_max;
    private:
};

#endif // STRATEGIES_H
