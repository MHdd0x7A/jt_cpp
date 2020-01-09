#ifndef DE_1997_FUNCTIONS_H
#define DE_1997_FUNCTIONS_H

#include "..\Benchmarks.h"

class DE_1997_functions:public Benchmarks
{
    public:
        DE_1997_functions();
        double compute(double* x);
        virtual ~DE_1997_functions();

    protected:

    private:
};

#endif // DE_1997_FUNCTIONS_H
