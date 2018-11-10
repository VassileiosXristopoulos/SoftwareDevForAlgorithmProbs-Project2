//
// Created by vassilis on 18/10/2018.
//

#include <cmath>
#include <algorithm>
#include <bitset>
#include <regex>
#include <fstream>
#include "../header/Util.h"

int Util::my_mod(int x, int y) {
    return (x % y + y) % y;
}
double Util::EucledianDistance(vector<int> x, vector<int> y) {
    double distance=0;
    for (unsigned int j = 0; j < x.size(); j++) {
        distance += pow(x[j] - y[j], 2);
    }
    return sqrt(distance);
}

int Util::intVectortoInteger(vector<int> table) { //convert vector<int> to integer
    int retval = 0;
    int i =0;
    std::reverse(table.begin(),table.end());
    for (auto it = table.begin() ; it != table.end(); it++,i++){
        if(*it){
            retval |= 1<<i;
        }
    }
    return retval;
}

vector<int> Util::intToIntVector(int number, int d) { //convert int to vector<int>
    vector<int> ret = vector<int>(d);
    for ( int i = 0; i < d; i++) {
        ret[d - 1 - i] = (1 << i & number) != 0;
    }
    return ret;
}

unsigned long Util::upper_power_of_two(unsigned long v){ //find the closest upper power of 2 to v
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

void Util::getHammingCloseVectors( int changesLeft, vector<int>& str, const int i,
        vector<vector<int>>&res
) {                                 //get all the vectors with hamming distance up to "changesleft"
    if (changesLeft == 0) {
        res.push_back(str);
        return;
    }
    if (i < 0) return;

    str[i] ^= 1;
    getHammingCloseVectors(changesLeft-1,str, i-1,res);

    str[i] ^= 1;
    getHammingCloseVectors(changesLeft,str, i-1,res);
}

double Util::cosineDistance(vector<int> &x, vector<int> &y) {
    double sum=0,partial_sumX=0,partial_sumY=0;

    for(unsigned int i=0;i<x.size();i++){
        sum+=x[i]*y[i];
        partial_sumX+=pow(x[i],2);
        partial_sumY+=pow(y[i],2);
    }
    partial_sumX=sqrt(partial_sumX);
    partial_sumY=sqrt(partial_sumY);
    return 1 - sum/(partial_sumX*partial_sumY);
}

vector<string> Util::Split(string &line) { //split a string by spaces and return a vector<string>
    vector<string> element;
    size_t pos = line.find(' ');
    size_t startPos = 0;
    while (pos != string::npos) {
        element.push_back(line.substr(startPos, pos - startPos));
        startPos = pos + 1;
        pos = line.find(' ', startPos);
    }
    element.push_back(line.substr(startPos, pos - startPos));

    return element;
}

Lsh_arguments Util::getLshArguments(int argv,char*argc[]) { //get arguments for lsh. Depends on command line arguments
    Lsh_arguments args;
    if(argv>=7){
        if( argv == 11 ){
            if(argc[2]==NULL || argc[4]==NULL || argc[6]==NULL || argc[8]==NULL || argc[10]==NULL ){
                cout << "Invalid Arguments" << endl;
                exit(0);
            }

            args.inputFile = "Input/" + string(argc[6]);
            args.queryFile = "Input/" + string(argc[8]);
            //trick - modified order of arguments to implement rerun
            args.k = atoi(argc[2]);
            args.L = atoi(argc[4]);
            if( args.k<=0 || args.L<=0 ){
                cout << "k and/or L arguments not given properly" << endl;
                exit(0);
            }
            args.outputFIle = "Output/" + string(argc[10]);
        }
        else if( argv == 7){
            if(argc[2]==NULL || argc[4]==NULL || argc[6]==NULL){
                cout << "Invalid Arguments" << endl;
                exit(0);
            }
            args.inputFile = "Input/" + string(argc[2]);
            args.queryFile = "Input/" + string(argc[4]);
            args.k = 4;
            args.L = 5;
            args.outputFIle = "Output/" + string(argc[6]);
        }
        else {
            cout << "Wrong arguments!" << endl;
            exit(0);
        }


    }
    else {
        if (argv == 5) {
            args.k = atoi(argc[2]);
            args.L = atoi(argc[4]);

        } else if (argv == 1) {
            args.k = 4;
            args.L = 5;
        } else {
            cout << "Wrong arguments!" << endl;
            exit(0);
        }
        cout << "Please give path of dataset:";
        cin >> args.inputFile;
        cout << endl << "Please give path of query file:";
        cin >> args.queryFile;
        args.outputFIle = "Output/output.txt";
        cout << "Output file generated at: "<< args.outputFIle << endl;
    }
    std::ifstream inputfile(args.inputFile);
    std::ifstream queryfile(args.queryFile);
    if(!(inputfile.good() && queryfile.good())){
        cout << "Not existing Input files!"<<endl;
        exit(0);
    }
    return args;
}

Cube_arguments Util::getCubeArguments(int argv,char*argc[]) { //get arguments for cube. Depends on command line
    // arguments
    Cube_arguments args;
    if(argv>=7){
        if (argv == 13) {
            if (argc[2] == NULL || argc[4] == NULL || argc[6] == NULL || argc[8] == NULL || argc[10] == NULL ||
                argc[12] == NULL) {
                cout << "Invalid Arguments" << endl;
                exit(0);
            }

            args.inputFile = "Input/" + string(argc[8]);
            args.queryFile = "Input/" + string(argc[10]);
            args.k = atoi(argc[2]);
            args.M = atoi(argc[4]);
            if (args.k <= 0 || args.M <= 0) {
                cout << "k and/or M arguments not given properly" << endl;
                exit(0);
            }
            args.probes = atoi(argc[6]);
            args.outputFIle = "Output/" + string(argc[12]);
        } else if (argv == 7 && string(argc[1])=="-d") {
            if (argc[2] == NULL || argc[4] == NULL || argc[6] == NULL) {
                cout << "Invalid Arguments" << endl;
                exit(0);
            }
            args.inputFile = "Input/" + string(argc[2]);
            args.queryFile = "Input/" + string(argc[4]);
            args.k = -1;
            args.M = 10;
            args.probes = 2;
            args.outputFIle = "Output/" + string(argc[6]);
        } else {
            cout << "Wrong arguments!" << endl;
            exit(0);
        }
    }
    else {
        if (argv == 7) {
            args.k = atoi(argc[2]);
            args.M = atoi(argc[4]);
            args.probes = atoi(argc[6]);

        } else if (argv == 1) {
            args.k = -1;
            args.M = 10;
            args.probes = 2;
        } else {
            cout << "Wrong arguments!" << endl;
            exit(0);
        }
        cout << "Please give path of dataset:";
        cin >> args.inputFile;
        cout << endl << "Please give path of query file:";
        cin >> args.queryFile;
        args.outputFIle = "Output/output.txt";
        cout << "Output file generated at: "<< args.outputFIle << endl;
    }
    std::ifstream inputfile(args.inputFile);
    std::ifstream queryfile(args.queryFile);
    if(!(inputfile.good() && queryfile.good())){
        cout << "Not existing Input files!"<<endl;
        exit(0);
    }
    return args;
}
