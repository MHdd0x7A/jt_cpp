#include "Header.h"//2013 CEC functinos
#include "include/DE.h"
#include <sys/time.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <float.h>

using namespace std;
double F = 0.5, CR = 0.9, K;
unsigned dim = 30;
unsigned np = 100;
unsigned gen_max = 5000;//halt after gen_max generations
double* bound_min,*bound_max;
DE* de = NULL;

int main(){
    /*  Test the basic benchmark function */

    srand(0);
    ReadConfig* rc = new ReadConfig("config");
    Benchmarks* fp=NULL;
    unsigned funToRun[] = {16,17,18,19,20,21,22,23,24};
    unsigned funNum = 9;
    unsigned run = 50;//to get average
    vector<double> runTimeVec;
    struct timeval start, end;
    long seconds, useconds;
    double mtime;
    unsigned totalEval;
    double allFitness;//each run add best fitness value
    for(unsigned i=0; i<funNum; i++){
        totalEval = 0;
        allFitness = 0.0;
        //ini function
        fp = generateFuncObj(funToRun[i]);
        if(funToRun[i] < 16){
            //bound_min = fp->getMinX();// get solution space minimum bound
            //bound_max = fp->getMaxX();// get solution space maximum bound
        }
        else{
            gen_max = rc->getDE_Generation(funToRun[i]-1);
            dim = rc->getDim(funToRun[i]-1);
            bound_min = rc->getMinX(funToRun[i]-1);
            bound_max = rc->getMaxX(funToRun[i]-1);
        }
        de = new DE(np,dim,CR,F,bound_min,bound_max);
        de->setStrategyID(random);
        cout<<"min:"<<bound_min[0]<<" max:"<<bound_max[0]<<endl;
        gettimeofday(&start, NULL);
        for (unsigned run_counter=0; run_counter < run; run_counter++){
            fp->nextRun();
            de->setBenchmark(fp);
            de->nextRun();
            /****ini****/
            totalEval += np;
            for(unsigned gen_counter = 1; gen_counter < gen_max; gen_counter++){
                de->produceNextGeneration();
                de->selection();
            }
            totalEval += np;
            cout<<"run"<<run_counter<<": "<<de->getNowBest()<<endl;
            allFitness += de->getNowBest();
            /*stop do something*/
        }
        cout<<endl << "total evaluation:" << totalEval << " AVG evaluation:" << totalEval/run <<endl;
        cout<<"Mean best:"<<allFitness/run<<endl;
        gettimeofday(&end, NULL);
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = (((seconds) * 1000 + useconds/1000.0) + 0.5)/1000;
        runTimeVec.push_back(mtime);
        printf ( "F %d, Running Time = %f s\n\n", fp->getID(), mtime);
        delete fp;
    }
    delete de;
    return 0;
}
// create new object of class with default setting
Benchmarks* generateFuncObj(int funcID){
    Benchmarks *fp;
    // run each of specified function in "configure.ini"
    if (funcID==1){
        fp = new F1();
    }else if (funcID==2){
        fp = new F2();
    }else if (funcID==3){
        fp = new F3();
    }else if (funcID==4){
        fp = new F4();
    }else if (funcID==5){
        fp = new F5();
    }else if (funcID==6){
        fp = new F6();
    }else if (funcID==7){
        fp = new F7();
    }else if (funcID==8){
        fp = new F8();
    }else if (funcID==9){
        fp = new F9();
    }else if (funcID==10){
        fp = new F10();
    }else if (funcID==11){
        fp = new F11();
    }else if (funcID==12){
        fp = new F12();
    }else if (funcID==13){
        fp = new F13();
    }else if (funcID==14){
        fp = new F14();
    }else if (funcID==15){
        fp = new F15();
    }else if (funcID==16){
        fp = new benchmark_1999_F1();
    }else if (funcID==17){
        fp = new benchmark_1999_F2();
    }else if (funcID==18){
        fp = new benchmark_1999_F3();
    }else if (funcID==19){
        fp = new benchmark_1999_F4();
    }else if (funcID==20){
        fp = new benchmark_1999_F5();
    }else if (funcID==21){
        fp = new benchmark_1999_F6();
    }else if (funcID==22){
        fp = new benchmark_1999_F7();
    }else if (funcID==23){
        fp = new benchmark_1999_F8();
    }else if (funcID==24){
        fp = new benchmark_1999_F9();
    }else{
        cerr<<"Fail to locate Specified Function Index"<<endl;
        exit(-1);
    }
    return fp;
}

