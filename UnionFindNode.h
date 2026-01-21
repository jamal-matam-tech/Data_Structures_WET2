#ifndef UNIONFINDNODE_H
#define UNIONFINDNODE_H

#include "wet2util.h"

class Squad;
class UFNode {
    private:
    UFNode* parent;           
    int size;                 
    Squad* squad;
    NenAbility partialNenAbility; // Cumulative NenAbility from this node to root
    NenAbility delta;            // NenAbility that existed before this hunter (relative to parent)
    NenAbility totalNen;          // Total NenAbility in squad (only meaningful for roots)
    
    
    public:

    UFNode(Squad* squadPtr);
    ~UFNode() = default;
    UFNode* find();
    static void unionSets(UFNode* node1, UFNode* node2);
    Squad* getSquad() const;
    int getSize() const;
    UFNode* getParent() const;
    bool isRoot() const;
    void setParent(UFNode* p);
    void setSquad(Squad* s);
    void setSize(int s);
    void incrementSize();
    NenAbility getPartialNenAbility() const;
    
    // Delta-based weighted UF
    NenAbility getDelta() const;
    void setDelta(const NenAbility& d);
    NenAbility getTotalNen() const;
    void setTotalNen(const NenAbility& t);
};

#endif //UNIONFINDNODE_H
