//
// Created by vassilis on 10/11/2018.
//

#include "../header/Cluster.h"

Cluster::~Cluster() {

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
 * K-Means
 */
void Cluster::Update() {
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

int Cluster::size() {
    return this->Members.size();
}


