#ifndef READCONFIG_H
#define READCONFIG_H

#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;
class ReadConfig
{
    public:
        ReadConfig();
        ReadConfig(string);
        virtual ~ReadConfig();
        unsigned getDE_NP(int);
        unsigned getDim(int);
        unsigned getDE_Generation(int);
        double* getMinX(int);
        double* getMaxX(int);
        double getDE_F(int);
        double getDE_CR(int);
    protected:
        int count_space();
        int readBound(int,bool);
        void bound_checker(int,int,int);
        void parseConfig();
        void parseInfo();
        void parseParameter();
        void parseDE_Parameter(int);
        unsigned* DE_NP;
        unsigned* Dim;
        unsigned* DE_Gen;
        double** minX;
        double** maxX;
        double* DE_F;
        double* DE_CR;
    private:
        ifstream config;
        string oneLine;
};

#endif // READCONFIG_H
