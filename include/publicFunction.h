#ifndef PUBLICFUNCTION_H
#define PUBLICFUNCTION_H

#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <time.h>

class publicFunction
{
    public:
        publicFunction();
        virtual ~publicFunction();
        static time_t t;
        static double random_double(double,double);
        static unsigned random_unsigned(unsigned,unsigned);
        static unsigned* population_shuffle(unsigned,unsigned,unsigned);
        static double normal_distribution_random(double,double);
    protected:

    private:
};

#endif // PUBLICFUNCTION_H
