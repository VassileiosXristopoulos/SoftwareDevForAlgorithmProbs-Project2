//
// Created by vassilis on 10/11/2018.
//
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <ctime>
#include <iterator>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "../header/DataSetMap.h"
#include "../header/ClusterMaster.h"
#include "../header/Util.h"

int k=0;
int main(int argv, char*argc[]){


    if(argv != 9 && argv!= 10) {
        cout << "Invalid command line arguments" << endl;
        exit(0);
    }

    DataSetMap* Map = new DataSetMap;
    string input_file = argc[2];
    string config_file = argc[4];
    string output_file = argc[6];
    string metric = argc[8];
    bool complete = false;
    if(argv==10){
        if(*(new string(argc[9])) == "-complete")
            complete=true;
    }
                                                            // TODO: implement printing
    Map->InsertFile(input_file);                            // TODO: implement Shilouette
    Config_info info = Util::GetConfiguration(config_file); // TODO: implement giving all lsh/cube args from config
    int* Choises = Util::GetUserChoise();
    ClusterMaster* Clustermaster = new ClusterMaster(info, Map, Choises,metric,complete);
    Clustermaster->Clustering();
}