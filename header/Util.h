//
// Created by vassilis on 18/10/2018.
//

#ifndef UTIL_H
#define UTIL_H


#include "Item.h"
struct Lsh_arguments{ //TODO:  remove structs for project 1
    string inputFile;
    string queryFile;
    int k;
    int L;
    string outputFIle;
};

struct Cube_arguments{
    string inputFile;
    string queryFile;
    int probes;
    int k;
    int M;
    string outputFIle;

};

struct Config_info{
    int k;
    int numof_hashFunctions;
    int numOf_hashTables;
};

class Util {
public:
    static int my_mod(int x, int y);

    static double EucledianDistance(vector<int> x, vector<int> y);

    static int intVectortoInteger(vector<int> table);

    static vector<int> intToIntVector(int number, int d);

    static unsigned long upper_power_of_two(unsigned long v);

    static void getHammingCloseVectors( int changesLeft, std::vector<int>& str, const int i,
            vector<vector<int>>&res );
    static double cosineDistance(vector<int>&x,vector<int>&y);
    static vector<string> Split(string &line);

    static Lsh_arguments getLshArguments(int argv,char *argc[]);
    static Cube_arguments getCubeArguments(int argv,char *argc[]);
    static Config_info GetConfiguration(string);
};

#endif //UTIL_H
