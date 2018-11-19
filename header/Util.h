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
    int lsh_k;
    int lsh_L;
    int w;
    int cube_k;
    int cube_M;
    int cube_probes;
};



class Util {
public:
    static int my_mod(int x, int y);

    static double EucledianDistance(vector<double> x, vector<double> y);

    static int intVectortoInteger(vector<int> table);

    static vector<int> intToIntVector(int number, int d);

    static unsigned long upper_power_of_two(unsigned long v);

    static void getHammingCloseVectors( int changesLeft, std::vector<int>& str, const int i,
            vector<vector<int>>&res );
    static double cosineDistance(vector<double>&x,vector<double>&y);
    static vector<string> SplitBlanks(string &line);
    static vector<string> SplitCommas(string &line);
    static Lsh_arguments getLshArguments(int argv,char *argc[]);
    static Cube_arguments getCubeArguments(int argv,char *argc[]);
    static Config_info GetConfiguration(string);
    static int safe_atoi(string);
    static string safe_getline(std::ifstream&);
    static int* GetUserChoise();
};

#endif //UTIL_H
