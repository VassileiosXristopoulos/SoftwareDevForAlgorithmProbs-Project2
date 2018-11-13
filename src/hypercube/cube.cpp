//
// Created by vassilis on 20/10/2018.
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
#include "../../header/DataSetMap.h"
#include "../../header/Item.h"
#include "../../header/hypercube/CosineHypercube.h"
#include "../../header/hypercube/EucledianHypercube.h"
#include "../../header/Util.h"

using namespace std;
int r,k,w=400;
default_random_engine generator;
normal_distribution<float> distribution(0,1);
extern int d,n;

int main(int argv,char **argc) {
    srand(time(NULL));
    int probes, M;
    string FileLine;
    string inputFile, queryFile, outputFile;
    while(1) {
        Cube_arguments arguments = Util::getCubeArguments(argv, argc);
        k = arguments.k;
        M = arguments.M;
        probes = arguments.probes;
        inputFile = arguments.inputFile;
        queryFile = arguments.queryFile;
        outputFile = arguments.outputFIle;

        DataSetMap Map;
        //read dataset file and save it to a structure with all the dataset items.
        // every entry in the HashTables will be a pointer to this Map
        string mode = Map.InsertFile(inputFile);

        AHypercube *hypercube;
        if (mode.compare("eucledian") == 0) {
            hypercube = new EucledianHypercube(k);
        } else {
            hypercube = new CosineHypercube(k);
        }

        for (int i = 0; i < Map.size(); i++) {
            hypercube->add(Map.at(i));
        }
        ifstream input_q(queryFile);
        string radius;
        getline(input_q, radius); // get radius (i.e. first line)

        r = stoi(radius.substr(radius.find(":") + 1));
        cout << radius << endl;
        double max_div = 0, avg_tHypercube=0;
        ofstream output;
        output.open(outputFile);
        while (getline(input_q, FileLine)) { //for each line of query set
            double tHupercube, tTrue;
            istringstream iss(FileLine);

            string line = FileLine.substr(0, FileLine.size() - 1);//trim the last character of line -- get pure the id's

            vector<string> element = Util::Split(line);// get string vector which has at index 0 the id and then the
            // coordinates

            Item *item = new Item(element);  //construct an object with information about the query item
            pair<string, double> closer_item;
            clock_t true_start = clock();
            double trueDist = Map.TrueDistance(item, mode);
            clock_t true_end = clock();
            tTrue = (true_end - true_start) / (double) CLOCKS_PER_SEC;  //count time to find the true closest distance


            clock_t nearest_start = clock();
            probes = 1;
            closer_item = hypercube->findCloser(item, M, probes);
            clock_t nearest_end = clock();
            tHupercube = (nearest_end - nearest_start) / (double) CLOCKS_PER_SEC;  //count time to find the closest neighbor

            vector<string> Rnearest = hypercube->findRCloser(item, M, probes, r);
            output << "Query item: " << item->getName() << endl;
            output << "R-nearest neighbor:" << endl;
            for (unsigned int i = 0; i < Rnearest.size(); i++) {
                output << Rnearest[i] << endl;
            }
            output << "Nearest neighbor: "
                      ": " << closer_item.first << endl << "distance Hypercube: " << closer_item.second << endl <<
                   "distanceTrue: " << trueDist << endl;
            output << "tHupercube: " << tHupercube << endl;
            output << "tTrue: " << tTrue << endl << endl;
            avg_tHypercube+=tHupercube;

            double div = closer_item.second / trueDist;
            if (div > max_div)
                max_div = div;
            delete (item);
        }

        output << "Max Div: " << max_div << endl;
        int total_size = hypercube->size();
        output << "Total size of " << mode << " hypercube = " << total_size << endl;
        output << "Average tHypercube: "<< avg_tHypercube/n << endl;
        delete (hypercube);

        output.close();
        cout<<"Program finished! Results at "<<outputFile<<endl;
        cout<<"Please select your choise:"<<endl;
        cout<<"     To run again with different arguments: Press 1"<<endl;
        cout<<"     To stop the program: Press 2"<<endl;
        int input;
        cin >> input;
        if(input==2) break;
        if(argv==13) argv=7; //trick - if all args initially given from command line, go to proper case
        if(argv==7)  argv=1; //trick - k,L default, go to proper case
    }
}
