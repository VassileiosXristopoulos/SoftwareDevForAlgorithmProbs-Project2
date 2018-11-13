//
// Created by vassilis on 10/11/2018.
//

#include "../header/Cluster.h"
#include "../header/Util.h"

Cluster::~Cluster() {
    Centroid = nullptr;
}

/**
 * Sets an Item as Centroid of the Cluster
 */
void Cluster::SetCentroid(Item * item) {
    Centroid = item;
}



/**
 * Returns the Centroid of the Cluster
 */
Item *Cluster::GetCentroid() {
    return Centroid;
}



/**
 * Returns true if an Item is member of this Cluster
 */
bool Cluster::Contains(string item) {
    return Members.find(item) != Members.end();
}



/**
 * Delete a Member and return it
 */
void Cluster::DeleteMember(string item) {
    Members.erase(item);
}



/**
 * Add a new Member
 */
void Cluster::InsertMember(Item * item) {
    Members.insert(pair<string,Item*>(item->getName(),item));
}



/**
 * Range Search
 */
bool Cluster::ReplaceElements(vector<Item *>) {
    return false;
}



/**
 * Update Members
 */
void Cluster::Update(int algorithm) {
    switch (algorithm){
        case 1:
            kmeans();
            break;
        case 2:
            PAM();
            break;
        default:
            cout<< "Error: No Update can be performed" <<endl; // TODO: implement function for errors at Util
            exit(0);
    }


}

int Cluster::size() {
    return this->Members.size();
}

void Cluster::PAM() {
    vector<pair<double,string>>distances;
    for(auto const& i : Members) { // for each element of Cluster
        Item * element_i = i.second;

        double sum =0; // sum of distances of items from element_i (to be centroid)
        for(auto const& j : Members){ // get every member of cluster
            Item * element_j = j.second;
            if(element_i->getName() != element_j->getName()){ // except itself
                sum += Util::EucledianDistance(element_i->getContent(),element_j->getContent());
            }
        }
        distances.push_back(make_pair(sum,i.first));
    }

    double min = distances[0].first;
    string newCentroid = Members.begin()->first;
    for(int j=1; j<distances.size(); j++){
        if(distances[j].first<min){
            min = distances[j].first;
            newCentroid = distances[j].second;
        }
    }
    Centroid = Members.find(newCentroid)->second;
}



/**
 * Update Members
 * K-Means
 */
void Cluster::kmeans() {
    if(Members.empty()) return;

    Item * newCentroid = new Item("");
    int totalElements = static_cast<int>(Members.size());
    int dimensions = static_cast<int>(Members.begin()->second->getContent().size());
    vector<int>Points = vector<int>(dimensions,0); // set size to be the same with Item's content, init all with 0

    for(auto const& i : Members){ // for each element of Cluster
        Item * element_i = i.second;
        for(unsigned int j=0; j< dimensions ; j++){ // for each point of the element
            Points[j] += element_i->GetPoint(j);
        }
    }
    for (int l = 0; l < dimensions; ++l) {
        Points[l] /= totalElements;
    }

    newCentroid->SetContent(Points);
    this->Centroid = newCentroid;
}

void Cluster::FlushClusterMembers() {
    this->Members.clear();
}


