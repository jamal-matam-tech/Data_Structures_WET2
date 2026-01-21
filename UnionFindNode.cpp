

#include "UnionFindNode.h"
#include "squad.h"

UFNode::UFNode(Squad* squadPtr) 
    : parent(this),      
      size(1),           
      squad(squadPtr),
      partialNenAbility(NenAbility::zero()),
      delta(NenAbility::zero()),
      totalNen(NenAbility::zero())
{
}

UFNode* UFNode::find() {
    if (parent == this) {
        // Root - delta should be 0
        return this;
    }
    
    // Store old parent's delta before path compression
    UFNode* oldParent = parent;
    NenAbility oldParentDelta = oldParent->delta;
    
    // Recursively find root and compress path
    parent = parent->find();
    
    // Accumulate: add old parent's delta to ours
    // After parent->find(), oldParent's delta is relative to root
    if (oldParent != parent) {
        delta += oldParentDelta;
    }
    
    return parent;
}

void UFNode::unionSets(UFNode* node1, UFNode* node2) {
    UFNode* root1 = node1->find();
    UFNode* root2 = node2->find();
    
    if (root1 == root2) {
        return;
    }
    
    if (root1->size < root2->size) {
        root1->parent = root2;
        root2->size += root1->size;
    } else {
        root2->parent = root1;
        root1->size += root2->size;
    }
}

Squad* UFNode::getSquad() const {
    return squad;
}

int UFNode::getSize() const {
    return size;
}

UFNode* UFNode::getParent() const {
    return parent;
}

bool UFNode::isRoot() const {
    return parent == this;
}

void UFNode::setParent(UFNode* p) {
    parent = p;
}

void UFNode::setSquad(Squad* s) {
    squad = s;
}

void UFNode::incrementSize() {
    size++;
}

NenAbility UFNode::getPartialNenAbility() const {
    return partialNenAbility;
}

NenAbility UFNode::getDelta() const {
    return delta;
}

void UFNode::setDelta(const NenAbility& d) {
    delta = d;
}

NenAbility UFNode::getTotalNen() const {
    return totalNen;
}

void UFNode::setTotalNen(const NenAbility& t) {
    totalNen = t;
}
