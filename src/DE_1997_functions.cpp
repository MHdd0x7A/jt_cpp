#include "..\include\DE_1997_functions.h"

DE_1997_functions::DE_1997_functions():Benchmarks()
{
    dimension = 3;
    minX = -5.12;
    maxX = 5.12;
    //ctor
}

DE_1997_functions::~DE_1997_functions()
{
    //dtor
}

double DE_1997_functions::compute(double* x){
    double result = 0.0;
    for(int dim_counter = 0; dim_counter < dimension; dim_counter++){
        result += pow(x[dim_counter],2);
    }
    update(result);
    return result;
}


