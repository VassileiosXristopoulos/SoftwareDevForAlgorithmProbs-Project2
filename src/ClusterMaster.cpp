#include <utility>

//
// Created by vassilis on 10/11/2018.
//

#include "../header/ClusterMaster.h"
#include "../header/Util.h"
#include <ctime>
#include <random>

default_random_engine generator;
normal_distribution<float> distribution(0,1);

/**
 * Constructor
 */
ClusterMaster::ClusterMaster(Config_info config_info, DataSetMap* set, int* V,string& metric,bool complete) {

    this->complete = complete; // for complete printing

    this->Clusters = vector<Cluster*>((unsigned long)config_info.k);
    for(int i=0;i<config_info.k;i++){
        Clusters[i] = new Cluster();
    }


    if(V[0] == 0 && V[1] == 0 && V[2] == 0){ // if no choise is given from user, run all cases
        Choises = vector<int>(3,1); // 3 positions, value 1 for all (1st choise)
        canRepeat = true;
    }
    else{ // choise is given from user, choose that choise
        for(int i=0;i<3;i++){
            Choises.push_back(V[i]);
        }
    }
    // we nead this dataset at every case because of initialization step
    this->Dataset = set; // Keep a simple array with the dataset
    switch (Choises[1]){
        case 2:
            lsh_master = new lsh(config_info.lsh_k,config_info.lsh_L,config_info.w, metric,set);
            break;
        case 3:
            hypercube_master = new cube(config_info.cube_k,config_info.w,config_info.cube_probes,config_info.cube_M,metric,
                    set);
            break;
        default:
            break;
    }
}



/**
 * Destructor
 */
ClusterMaster::~ClusterMaster() {

}



/**
 * Initialization function
 */
void ClusterMaster::Initialization() {
   switch (Choises[0]){

       case 1 :
           RandomSelection();
           break;

       case 2:
           kmeanspp();
           break;

       default:
           cout<< "Error: No initialization can be performed" <<endl; // TODO: implement function for errors at Util
           exit(0);

   }
}


/**
 * Initialize centers
 * Random selection (simple)
 */
void ClusterMaster::RandomSelection() {
    srand(time(0));
    for(unsigned int i=0; i<Clusters.size(); i++){
        // select a random Item from Map for each Cluster's centroid
        Item * item = Dataset->at(rand() % Dataset->size());
        item->SetCluster(i);
        Clusters[i]->SetCentroid(item);
        item->SetCluster(i);
    }
}

/**
 * Initialize centers
 * K-Means++
 */
void ClusterMaster::kmeanspp() {
    int initialCentroid = rand() % Dataset->size();
    Clusters[0]->SetCentroid(Dataset->at(initialCentroid)); // pick uniformly first centroid
    Dataset->at(initialCentroid)->SetCluster(0);
    DataSetMap* nonCentroidMap = new DataSetMap(*Dataset); // get copy of Dataset, keep only the non centroids
    nonCentroidMap->erase(Dataset->at(initialCentroid)); //erase from map, keep only the non centroids

    double maxDi = 0;
    vector<double>Di; // minimum distance from centroids
    // the idea is that we compute every D(i) once and for all and when we set a centroid we erase the element
    for(unsigned int i=0;i<nonCentroidMap->size();i++){ // for each non centroid (compute minimum distace)

        //initially, it is the distance from the "first" centroid
        double minDist = Util::EucledianDistance(nonCentroidMap->at(i)->getContent(),Clusters[0]->GetCentroid()
        ->getContent());

        for(unsigned int j = 1; j < Clusters.size(); j++){ // search minimum
            if(Clusters[j]->GetCentroid() != nullptr){
                double dist = Util::EucledianDistance(nonCentroidMap->at(i)->getContent(),Clusters[j]->GetCentroid()
                        ->getContent());
                if(dist < minDist){
                    minDist = dist;
                }
            }
        }
        // here, minimum is found and is minDist
        Di.push_back(minDist); // Di[i] has the D(i) of non centroid i
        if(minDist > maxDi){
            maxDi = minDist;
        }
    }

    for(unsigned int i=0; i<Di.size(); i++){ // normalize D(i)'s
        Di[i] /= maxDi;
    }

    // perform the choosing of centroids
    for(int i=1;i<Clusters.size(); i++){ // until all the clusters have centroids
        vector<double>P((unsigned long)(nonCentroidMap->size() + 1),0); // P[0] = 0

        for(int j = 1; j < P.size(); j++){ // fill P(r)
            for(int k = 0; k < j ; k++){ // sum of D(i) from start until r
                P[j] += Di[k];
            }
        }
        // now we have P(r) array
        float rand_x = (float) (rand()) / ((float) (RAND_MAX / P[P.size()-1]) ); // pick random x ~ [0,P(n-t)]
        int newCentroid = -1;
        for(unsigned int j=1;j<P.size(); j++){
            if(rand_x > P[j-1] && rand_x <= P[j]){
                newCentroid = j-1; // get the position (in Di or in nonCentroidMap) of the new selected centroid
            }
        }

        if(newCentroid == -1){
            cout << "Error occured while performing kmeans++" <<endl;
            exit(0);
        }

        Clusters[i]->SetCentroid(nonCentroidMap->at(newCentroid));
        nonCentroidMap->at(newCentroid)->SetCluster(i);
        // now keep only the non centroid in Di and nonCentroidMap - that way the indexing will be consistent
        nonCentroidMap->erase(nonCentroidMap->at(newCentroid));
        Di.erase(Di.begin()+newCentroid);
    }

}


/**
 * Assign each point to new Cluster
 * Lloyd's assignement
 */
void ClusterMaster::LloydsAssignment() {
    bool noChanges = true;
    for(int i=0; i<Dataset->size(); i++){ // for each item

        Item * dataSetItem = Dataset->at(i);

        /*--------- compute the distance from the closest cluster ------------*/

        /* set min as the distance of the item with the "first" centroid */
        int min = (int)Util::EucledianDistance(dataSetItem->getContent(),Clusters[0]->GetCentroid()->getContent());
        int closerCluster = 0; //set Cluster 0 as the closest

        /* find minimum distance */

        for(int j = 1; j<Clusters.size(); j++ ){
            int dist = (int)Util::EucledianDistance(dataSetItem->getContent(),Clusters[j]->GetCentroid()->getContent());
            if(dist<min){
                min = dist;
                closerCluster = j;
            }
        }
        /*--------------------------------------------------------------------*/


        // closest cluster is "closerCluster"


        // if the item is not already in it's closest cluster
        if(!Clusters[closerCluster]->Contains(dataSetItem->getName())){
            if(dataSetItem->GetCluster() > 0 ){ // if item is in some cluster (i.e. we are not in 1st step)
                // delete the item from it's previous cluster
                Clusters[dataSetItem->GetCluster()]->DeleteMember(dataSetItem->getName());
            }
            dataSetItem->SetCluster(closerCluster); // set the item's cluster to be the new cluster
            Clusters[closerCluster]->InsertMember(dataSetItem); //insert the item to it's new cluster
        }

    }
}




/**
 * Assignment function
 */
void ClusterMaster::Assignement() {
    switch (Choises[1]){
        case 1:
            LloydsAssignment();
            break;
        case 2:
            RangeSearchAssignment(*(new string("lsh")));
            // LSH
            break;
        case 3:
            RangeSearchAssignment(*(new string("hypercube")));
            //cube
            break;
        default:
            cout<< "Error: No Assignment can be performed" <<endl; // TODO: implement function for errors at Util
            exit(0);
    }
}




/**
 * Perform clustering
 */
void ClusterMaster::Clustering() {
    do{
        this->Initialization();
        while( notFinished ){
            this->Assignement();
            this->Update();
        }
        PrintResults();
        SetNextChoise();
        ResetDataset();
        notFinished = true;
    }while(canRepeat);

}



/**
 * Update centers of each Cluster
 */
void ClusterMaster::Update() {
    bool noChanges = true;
    for(auto const& cluster_i : Clusters){
        noChanges &= cluster_i->Update(Choises[2]);
    }
    notFinished = !noChanges;
}

void ClusterMaster::SetNextChoise() {
    if(Choises[0]==1 && Choises[1] == 1 && Choises[2] == 1){ // [1,1,1]
        Choises[2] = 2;
    }
    else if(Choises[0]==1 && Choises[1] == 1 && Choises[2] == 2){ // [1,1,2]
        Choises[1] = 2;
        Choises[2] = 1;
    }
    else if(Choises[0]==1 && Choises[1] == 2 && Choises[2] == 1){ // [1,2,1]
        Choises[2] = 2;
    }
    else if(Choises[0]==1 && Choises[1] == 2 && Choises[2] == 2){ // [1,2,2]
        Choises[1] = 3;
        Choises[2] = 1;
    }
    else if(Choises[0]==1 && Choises[1] == 3 && Choises[2] == 1){ // [1,3,1]
        Choises[2] = 2;
    }
    else if(Choises[0]==1 && Choises[1] == 3 && Choises[2] == 2){ // [1,3,2]
        Choises[0] = 2;
        Choises[1] = 1;
        Choises[2] = 1;
    }
    else if(Choises[0]==2 && Choises[1] == 1 && Choises[2] == 1){ // [2,1,1]
        Choises[2] = 2;
    }
    else if(Choises[0]==2 && Choises[1] == 1 && Choises[2] == 2){ // [2,1,2]
        Choises[1] = 2;
        Choises[2] = 1;
    }
    else if(Choises[0]==2 && Choises[1] == 2 && Choises[2] == 1){ // [2,2,1]
        Choises[2] = 2;
    }
    else if(Choises[0]==2 && Choises[1] == 2 && Choises[2] == 2){ // [2,2,2]
        Choises[1] = 3;
        Choises[2] = 1;
    }
    else if(Choises[0]==2 && Choises[1] == 3 && Choises[2] == 1){ // [2,3,1]
        Choises[2] = 2;
    }
    else if(Choises[0]==2 && Choises[1] == 3 && Choises[2] == 2){ // [2,3,2]
        canRepeat = false; // terminate
    }


}



/**
 * For repeating case
 * Reset all clusters
 */
void ClusterMaster::ResetDataset() {
    for(int i=0;i<Clusters.size();i++){
        delete (Clusters[i]);
        Clusters[i] = new Cluster(); //remake all Clusters
    }
    for(int i=0 ; i< Dataset->size();i++){
        Dataset->at(i)->SetCluster(-1); // set -1 to "belongs to cluster" field of each item
    }
}

void ClusterMaster::RangeSearchAssignment(string& method) {

    int items_returned;
    // copy of dataset for only non assigned items
    // when an item is assigned somewhere, we delete it from this set
    // reason is we need a fast way to see if there are any unassigned items in order to stop looping
    DataSetMap * nonAssignedItems = this->Dataset;

    for(int i=0;i<Clusters.size();i++){ // clear all Clusters (except the centroid obviously)
        Clusters[i]->FlushClusterMembers(); //in this algorithm we compute all members from 0
    }
    /*----------------------ATTENTION: Until Loops are finished no item is inserted to cluster.  --------------------*/
    /*----------------------Reason is we would have to insert and delete from list.  --------------------*/
    /*----------------------Only when the algorithm decides the partitioning, we assign to lists  --------------------*/
    double r = 0.1;
    do{

        items_returned = 0;


        for(int i=0;i<Clusters.size();i++){ // for each cluster
            vector<Item*>Ncloser = GenericFindinRange(method,Clusters[i]->GetCentroid(),r); // get items in range
            items_returned += Ncloser.size();
            for( int j=0;j<Ncloser.size(); j++){ // for each item in range
                if( Ncloser[j]->GetCluster() == -1){  // if items dont belong to some cluster
                    Ncloser[j]->SetCluster(i);
                    nonAssignedItems->erase(Ncloser[j]); // keep only the non assigned items
                }
                else{
                    // compute distance from it's current cluster's centroid
                    double distFromOwner = Util::EucledianDistance(Ncloser[j]->getContent(),
                            Clusters[Ncloser[j]->GetCluster()]->GetCentroid()->getContent());
                    // compute distance from the cluster which got it in range as well
                    double distFromcurr = Util::EucledianDistance(Ncloser[j]->getContent(),
                            Clusters[i]->GetCentroid()->getContent());
                    if(distFromOwner>distFromcurr){ // if found closer cluster, assign to it
                        Ncloser[j]->SetCluster(i);
                    }
                }
            }
        }

        r *= 2;
    }while(nonAssignedItems->size()>0 && items_returned > 0); // loop until no more items unassigned from search or no
    // item changed

    for(int i=0;i<Dataset->size();i++){
        if(Dataset->at(i)->GetCluster()==-1){ // if there is any item with no cluster
            int min_cluster = 0; // number indicating which cluster is the closest
            // minimum distance from cluster
            double min_dist = Util::EucledianDistance(Dataset->at(i)->getContent(),Clusters[0]->GetCentroid()
            ->getContent());
            for(int j=1;j<Clusters.size();j++){ // find the closest cluster
                double dist = Util::EucledianDistance(Dataset->at(i)->getContent(),Clusters[j]->GetCentroid()
                        ->getContent());
                if(dist<min_dist){
                    min_dist = dist;
                    min_cluster = j;
                }
            }
            Clusters[min_cluster]->InsertMember(Dataset->at(i));
            Dataset->at(i)->SetCluster(min_cluster); // assign to it's closest cluster
        }
        else{ // finally assign to the clusters that the range search decided
            Clusters[Dataset->at(i)->GetCluster()]->InsertMember(Dataset->at(i));
        }

    }

}

void ClusterMaster::PrintResults() {
    cout <<"Algorithm " << Choises[0] <<"."<<Choises[1]<<"."<<Choises[2]<<endl<<endl;
    for(int i=0; i<Clusters.size();i++){
        cout <<"CLUSTER-"<<i+1<<" { size:"<<Clusters[i]->size() <<" Centroid: ";
        if(Choises[2] == 1){ //kmeans-update
            cout << "[ ";
            for(int j=0;j<Clusters[i]->GetCentroid()->getContent().size();j++){
                cout<<Clusters[i]->GetCentroid()->getContent()[j]<<" ";
            }
            cout<<"] }"<<endl;
        }
        else if(Choises[2] == 2){ //k-medoids update
           cout <<Clusters[i]->GetCentroid()->getName() <<" }"<<endl;
        }
    }
    cout<<endl;
    if(complete){ //complete printing
        cout << "/*------------------- Items in each cluster -----------------*/"<<endl;
        for(int i=0; i<Clusters.size();i++) {
            cout << "CLUSTER-" << i + 1 <<" { ";
            for(int j=0;j<Clusters[i]->GetMembers().size();j++){
                cout << Clusters[i]->GetMembers()[j]<<" ";
            }
            cout<<" }"<<endl;
        }
    }
    cout<<endl<<endl<<endl;
}

vector<Item *> ClusterMaster::GenericFindinRange(string &method, Item * centroid, double r) {
    if(method == "lsh"){
        return lsh_master->FindItemsInRange(centroid,r);
    }
    else if(method == "hypercube"){
        return hypercube_master->FindItemsInRange(centroid,r);
    }
}




