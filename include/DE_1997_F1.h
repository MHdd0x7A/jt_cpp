#ifndef DE_1997_F1_H
#define DE_1997_F1_H

#include "..\Benchmarks.h"

class DE_1997_F1:public Benchmarks
{
    public:
        DE_1997_F1();
        double compute(double* x);
        virtual ~DE_1997_F1();

    protected:

    private:
};

#endif // DE_1997_F1_H
