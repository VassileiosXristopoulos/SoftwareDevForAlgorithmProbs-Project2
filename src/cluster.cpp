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
    srand(time(NULL));

    if(argv != 5) {
        cout << "Invalid command line arguments" << endl;
        exit(0);
    }

    DataSetMap Map;
    string input_file = argc[1];
    string config_file = argc[2];
    string output_file = argc[3];
    string metric = argc[4];

    Map.InsertFile(input_file);
    Config_info info = Util::GetConfiguration(config_file);
    ClusterMaster* Clustermaster = new ClusterMaster(info.k,Map);
    Clustermaster->Clustering();
}