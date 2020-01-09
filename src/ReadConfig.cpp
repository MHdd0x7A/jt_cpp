#include "../include/ReadConfig.h"
using namespace std;

ReadConfig::ReadConfig()
{
    //ctor
    config.open("config",ios::in);
    if(!config){
        cout<<"loss config"<<endl;
        exit(0);
    }
    while(!config.eof() && config.is_open()){
        getline(config,oneLine);
        cout<<oneLine<<endl;
    }
}

ReadConfig::ReadConfig(string filename){
    config.open(filename,ios::in);
    if(!config){
        cout<<"loss "<<filename<<endl;
        while(true) cin;
    }
    while(!config.eof() && config.is_open()){
        getline(config,oneLine);
        cout<<oneLine<<endl;
        if(oneLine[0] == '#') parseConfig();
        else if(oneLine[0] == '@') continue;
    }
}

ReadConfig::~ReadConfig()
{
    //dtor
}

void ReadConfig::parseConfig(){
    bool config_ini = false;
    if(oneLine.compare("#info") == 0){
        config_ini = true;
    }
    while(oneLine[0] != '@'){
        if(config_ini){
            parseInfo();
        }
        else{
            parseParameter();
        }
    }
}

void ReadConfig::parseInfo(){
    string var;
    int pos;
    while(oneLine[0] != '@'){
        getline(config,oneLine);
        pos = oneLine.find('=');
        if(pos!=string::npos){
            var = oneLine.substr(0,pos);
            oneLine.erase(0,pos+1);
            if(var.compare("totalFunction")==0){
                int totalFunction = stoi(oneLine);
                DE_NP = (unsigned*)malloc(sizeof(unsigned)*totalFunction);
                Dim = (unsigned*)malloc(sizeof(unsigned)*totalFunction);
                DE_Gen = (unsigned*)malloc(sizeof(unsigned)*totalFunction);
                minX = (double**)malloc(sizeof(double*)*totalFunction);
                maxX = (double**)malloc(sizeof(double*)*totalFunction);
                DE_F = (double*)malloc(sizeof(double)*totalFunction);
                DE_CR = (double*)malloc(sizeof(double)*totalFunction);
            }
        }
    }
}

void ReadConfig::parseParameter(){
    string var;
    int pos,funcID,space_counter;
    if(oneLine[0] == '#' && oneLine[1] == 'F'){
        oneLine.erase(0,2);
        funcID = stoi(oneLine)-1;
    }
    else{
        cout<<"wrong function name"<<endl;
        exit(0);
    }
    space_counter = 0;
    while(true){
        if(oneLine[0] == '@')break;
        getline(config,oneLine);
        cout<<oneLine<<endl;
        pos = oneLine.find('=');
        if(pos != string::npos){
            var = oneLine.substr(0,pos);
            oneLine.erase(0,pos+1);
            if(var.compare("dimension") == 0){
                Dim[funcID] = stoi(oneLine);
            }else if(var.compare("minX") == 0){
                space_counter = space_counter==0?count_space():space_counter;
                minX[funcID] = (double*)malloc(sizeof(double)*space_counter);
                int counter = readBound(funcID,false);
                bound_checker(counter, space_counter, funcID);
            }else if(var.compare("maxX") == 0){
                space_counter = space_counter==0?count_space():space_counter;
                maxX[funcID] = (double*)malloc(sizeof(double)*space_counter);
                int counter = readBound(funcID,true);;
                bound_checker(counter, space_counter, funcID);
            }
        }else if(oneLine.compare("##DE")==0){
            parseDE_Parameter(funcID);
        }else{
            cout<<"undefined command: "<<oneLine<<endl;
            exit(0);
        }
    }
}

int ReadConfig::count_space(){
    int space_counter = 0;
    for(int counter = 0; counter < oneLine.size(); counter++){
        if(oneLine[counter] == ',') space_counter++;
    }
    return ++space_counter;
}

int ReadConfig::readBound(int funcID, bool isMax){
    int counter = 0, pos;
    while(true){
        if((pos = oneLine.find(','))!=string::npos){
            if(isMax){
                maxX[funcID][counter]=stod(oneLine.substr(0, pos));
                cout<<"maxX["<<funcID<<"]["<<counter<<"] =  "<<maxX[funcID][counter]<<endl;
            }
            else{
                minX[funcID][counter] = stod(oneLine.substr(0,pos));
                cout<<"minX["<<funcID<<"]["<<counter<<"] =  "<<minX[funcID][counter]<<endl;
            }
            oneLine.erase(0,pos+1);
            counter++;
        }
        else{
            if(isMax){
                maxX[funcID][counter]=stod(oneLine);
                cout<<"maxX["<<funcID<<"]["<<counter<<"] =  "<<maxX[funcID][counter]<<endl;
            }
            else{
                minX[funcID][counter] = stod(oneLine);
                cout<<"minX["<<funcID<<"]["<<counter<<"] =  "<<minX[funcID][counter]<<endl;
            }
            break;
        }
    }
    return counter;
}

void ReadConfig::bound_checker(int counter, int space_counter, int funcID){
    if(counter >= space_counter){
        cout<<"wrong space array in function "<<funcID<<endl;
        exit(0);
    }
    if(counter < space_counter-1){
        cout<<"wrong space array in function "<<funcID<<endl;
        exit(0);
    }
}

void ReadConfig::parseDE_Parameter(int funcID){
    string var;
    int pos;
    while(true){
        getline(config,oneLine);
        if(oneLine[0] == '@') break;
        pos = oneLine.find('=');
        if(pos != string::npos){
            var = oneLine.substr(0,pos);
            oneLine.erase(0,pos+1);
            if(var.compare("np")==0){
                DE_NP[funcID] = stoi(oneLine);
            }else if(var.compare("generation")==0){
                DE_Gen[funcID] = stoi(oneLine);
            }else if(var.compare("F")==0){
                DE_F[funcID] = stod(oneLine);
            }else if(var.compare("CR")==0){
                DE_CR[funcID] = stod(oneLine);
            }else{
                cout<<"undefined command: "<<var<<"="<<oneLine<<endl;
                exit(0);
            }
        }
    }
}

unsigned ReadConfig::getDE_NP(int funcID) {return DE_NP[funcID];}
unsigned ReadConfig::getDim(int funcID) {return Dim[funcID];}
unsigned ReadConfig::getDE_Generation(int funcID) {return DE_Gen[funcID];}
double* ReadConfig::getMinX(int funcID) {return minX[funcID];}
double* ReadConfig::getMaxX(int funcID) {return maxX[funcID];}
double ReadConfig::getDE_F(int funcID) {return DE_F[funcID];}
double ReadConfig::getDE_CR(int funcID) {return DE_CR[funcID];}
