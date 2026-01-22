

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
        // Root - return self
        return this;
    }
    
    // Store old parent before path compression
    UFNode* oldParent = parent;
    
    // Recursively find root and compress path
    UFNode* root = oldParent->find();
    
    // Accumulate: add old parent's delta to ours
    // After parent->find(), oldParent's delta is relative to root
    delta = delta + oldParent->delta;
    
    // Point directly to root
    parent = root;
    return root;

}

void UFNode::unionSets(UFNode* node1, UFNode* node2) {
    UFNode* root1 = node1->find();
    UFNode* root2 = node2->find();
    
    if (root1 == root2) {
        return;
    }
    
    // Union by size: attach smaller to larger
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

void UFNode::setSize(int s) {
    size = s;
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
