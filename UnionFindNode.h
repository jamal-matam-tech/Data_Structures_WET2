#ifndef UNIONFINDNODE_H
#define UNIONFINDNODE_H


#include "wet2util.h"
#include <memory>

class Squad;
class UFNode {
    private:
    UFNode* parent;           
    int size;                 
    Squad* squad;
    NenAbility partialNenAbility;
    NenAbility delta;
    NenAbility totalNen;
    int fightsDelta;
    
    
    public:

    UFNode(const std::shared_ptr<Squad>& squadPtr);
    ~UFNode() = default;
    UFNode* find();
    static void unionSets(UFNode* node1, UFNode* node2);
    Squad* getSquad() const;
    int getSize() const;
    UFNode* getParent() const;
    bool isRoot() const;
    void setParent(UFNode* p);
    void setSquad(const std::shared_ptr<Squad>&  s);
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
