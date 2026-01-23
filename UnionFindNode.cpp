#include "UnionFindNode.h"
#include "squad.h"
#include <memory> // for std::shared_ptr

UFNode::UFNode(std::shared_ptr<Squad> squadPtr)  // Updated to use shared_ptr
    : parent(this),
      size(1),
      squad(squadPtr),  // squad is now a shared_ptr
      partialNenAbility(NenAbility::zero()),
      delta(NenAbility::zero()),
      totalNen(NenAbility::zero()),
      fightsDelta(0)
{
}

//check
UFNode* UFNode::find() {
   /* if (parent == this) {
        return this;
    }

    // Store old parent before path compression
    UFNode* oldParent = parent;

    // Recursively find root and compress path
    UFNode* root = oldParent->find();

    // Accumulate deltas (both nen and fights)
    if (oldParent->parent != oldParent) {
        delta += oldParent->delta;
    }
    fightsDelta = fightsDelta + oldParent->fightsDelta;

    // Point directly to root
    parent = root;
    return root;*/
    if (parent == this) return this;

    UFNode* curr = this;
    NenAbility sumNen = NenAbility::zero();
    int sumF = 0;

    // Pass 1: find root and calculate total sums along the path (EXCLUDING the root)
    while (curr->parent != curr) {
        sumNen += curr->delta;
        sumF += curr->fightsDelta;
        curr = curr->parent;
    }
    UFNode* root = curr;

    // Pass 2: path compression and delta updates
    curr = this;
    while (curr != root) {
        NenAbility oldDelta = curr->delta;
        int oldF = curr->fightsDelta;

        curr->delta = sumNen;
        curr->fightsDelta = sumF;

        sumNen -= oldDelta;
        sumF -= oldF;

        UFNode* next = curr->parent;
        curr->parent = root;
        curr = next;
    }

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

std::shared_ptr<Squad> UFNode::getSquad() const {  // Return shared_ptr
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

void UFNode::setSquad(std::shared_ptr<Squad> s) {  // Set shared_ptr
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

int UFNode::getFightsDelta() const {
    return fightsDelta;
}

void UFNode::setFightsDelta(int f) {
    fightsDelta = f;
}
