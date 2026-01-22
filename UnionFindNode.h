#ifndef UNIONFINDNODE_H
#define UNIONFINDNODE_H

#include "wet2util.h"

class Squad;
class UFNode {
    private:
    UFNode* parent;           
    int size;                 
    std::shared_ptr<Squad> squad;
    NenAbility partialNenAbility; // Cumulative NenAbility from this node to root
    NenAbility delta;            // NenAbility that existed before this hunter (relative to parent)
    NenAbility totalNen;          // Total NenAbility in squad (only meaningful for roots)
    int fightsDelta;             // Fights that existed when this node joined (for accumulation)
    
    
    public:

    UFNode(std::shared_ptr<Squad> squadPtr);
    ~UFNode() = default;
    UFNode* find();
    static void unionSets(UFNode* node1, UFNode* node2);
    std::shared_ptr<Squad> getSquad() const;
    int getSize() const;
    UFNode* getParent() const;
    bool isRoot() const;
    void setParent(UFNode* p);
    void setSquad(std::shared_ptr<Squad> s);
    void setSize(int s);
    void incrementSize();
    NenAbility getPartialNenAbility() const;
    
    
    NenAbility getDelta() const;
    void setDelta(const NenAbility& d);
    NenAbility getTotalNen() const;
    void setTotalNen(const NenAbility& t);
    
    // Fight tracking
    int getFightsDelta() const;
    void setFightsDelta(int f);
};

#endif //UNIONFINDNODE_H
