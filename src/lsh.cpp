
// Created by vassilis on 12/10/2018.
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
#include "../header/Item.h"
#include "../header/lsh/EucledianHashTable.h"
#include "../header/lsh/CosineHashTable.h"
#include "../header/Util.h"

using namespace std;
extern int d,n;
int r,L,k,w=400;
default_random_engine generator;
normal_distribution<float> distribution(0,1);
int main(int argv,char **argc){
    srand(time(NULL));

    string inputFile, queryFile, outputFile,radius;

    while(1) {
        Lsh_arguments arguments = Util::getLshArguments(argv, argc);
        k = arguments.k;
        L = arguments.L;
        inputFile = arguments.inputFile;
        queryFile = arguments.queryFile;
        outputFile = arguments.outputFIle;


        DataSetMap Map;
        //read dataset file and save it to a structure with all the dataset items.
        // every entry in the HashTables will be a pointer to this Map
        string mode = Map.InsertFile(inputFile);
        AHashTable *TableArray[L];
        for (int i = 0; i < L; i++) { //construct the L HashTables
            if (mode.compare("eucledian") == 0) {
                TableArray[i] = new EucledianHashTable(k, n / 2);
            } else {
                TableArray[i] = new CosineHashTable(n / 2, k);
            }
            for (int j = 0; j < Map.size(); j++) {
                TableArray[i]->add(Map.at(j));
            }
        }


        ifstream input_q(queryFile);
        getline(input_q, radius); // get radius (i.e. first line)
        r = stoi(radius.substr(radius.find(":") + 1));

        double max_div = 0, avg_tLSH = 0;
        string line;
        ofstream output;
        output.open(outputFile);
        if(!output.good()){
            cout << "Output file given does not exist"<<endl;
            exit(0);
        }
        for (std::string FileLine; getline(input_q, FileLine);) { //for each line of the query file
            double tLSH, tTrue;

            line = FileLine.substr(0, FileLine.size() - 1); //trim the last character of line -- get pure the id's
            // and the coordinates
            vector<string> element = Util::Split(line); // get string vector which has at index 0 the id and then the
            // coordinates

            Item *item = new Item(element); //construct an object with information about the query item
            pair<Item *, double> closestNeighboor(NULL, -1.0);
            vector<string> closerNneighboors;

            for (int i = 0; i < L; i++) { //for each hashtable
                vector<string> Nneighboors = TableArray[i]->findNcloserNeighbors(item);

                clock_t nearest_start = clock();
                pair<Item *, double> neighboor = TableArray[i]->findCloserNeighbor(item);
                clock_t nearest_end = clock();
                tLSH = (nearest_end - nearest_start) / (double) CLOCKS_PER_SEC; //count time to find the closest neighbor

                if (neighboor.second > 0) {

                    if (closestNeighboor.second == -1 || neighboor.second < closestNeighboor.second) {
                        closestNeighboor = neighboor;
                    }

                    for (unsigned int j = 0; j < Nneighboors.size(); j++) {
                        closerNneighboors.push_back(Nneighboors[j]);
                    }
                }
            }
            if (closestNeighboor.second > 0) {
                output << "Query item: " << item->getName() << endl;
                //sort and remove duplicates from set of closer neighbors
                sort(closerNneighboors.begin(), closerNneighboors.end());
                closerNneighboors.erase(unique(closerNneighboors.begin(), closerNneighboors.end()), closerNneighboors
                        .end());

                output << "R-near neighbor:" << endl;
                if (closerNneighboors.size() == 0)
                    output << "none" << endl;


                for (unsigned int j = 0; j < closerNneighboors.size(); j++) {
                    if (closerNneighboors[j].compare(closestNeighboor.first->getName()) != 0) {
                        output << closerNneighboors[j] << endl;
                    }
                }

                output << "Nearest neighbor: " << closestNeighboor.first->getName() << endl <<
                       "distanceLSH: " << closestNeighboor.second << endl;

                clock_t true_start = clock();
                double trueDist = Map.TrueDistance(item, mode);
                clock_t true_end = clock();
                tTrue = (true_end - true_start) / (double) CLOCKS_PER_SEC;

                output << "distanceTrue: " << trueDist << endl;
                output << "tLSH: " << tLSH << endl;
                output << "tTrue: " << tTrue << endl;
                output << endl;
                avg_tLSH += tLSH;
                double div = closestNeighboor.second / trueDist;
                if (div > max_div) {
                    max_div = div;
                }
            } else {
                if (closerNneighboors.size() > 0) {
                    cout << "Error occured at finding neighboors. Found Ncloser without finding closer" << endl;
                    exit(0);
                }
            }
            delete (item);

        }
        output << "Max Div: " << max_div << endl;
        int total_size = 0;
        for (int i = 0; i < L; i++) {
            total_size += TableArray[i]->size();
        }
        output << "Total size of " << mode << " hashtable = " << total_size << endl;
        for (int i = 0; i < L; i++)
            delete (TableArray[i]);
        output << "Average tLSH: "<< avg_tLSH/n << endl;
        output.close();
        cout<<"Program finished! Results at "<<outputFile<<endl;
        cout<<"Please select your choise:"<<endl;
        cout<<"     To run again with different arguments: Press 1"<<endl;
        cout<<"     To stop the program: Press 2"<<endl;
        int input;
        cin >> input;
        if(input==2) break;
        if(argv==11) argv=5; //trick - if all args initially given from command line, go to proper case
        if(argv==7)  argv=1; //trick - k,L default, go to proper case
    }

}


