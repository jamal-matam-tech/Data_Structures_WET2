

#include "squad.h"
#include "UnionFindNode.h"
#include "wet2util.h"

Squad::Squad(int id) 
    : squadId(id),
      totalAura(0),
      ufNode(nullptr),
      squadSize(0),
      squadExp(0),
      totalFights(0),
      totalNenAbility(NenAbility::zero())
{
    ufNode = new UFNode(this);
}

Squad::~Squad() {
    if (ufNode != nullptr) {
        delete ufNode;
        ufNode = nullptr;
    }
}

int Squad::getSquadId() const {
    return squadId;
}

unsigned int Squad::getTotalAura() const {
    return totalAura;
}

int Squad::getSquadSize() const {
    return squadSize;
}

int Squad::getSquadExp() const {
    return squadExp;
}

int Squad::getTotalFights() const {
    return totalFights;
}

UFNode* Squad::getUFNode() const {
    return ufNode;
}

NenAbility Squad::getNenAbility() const {
    return totalNenAbility;
}

void Squad::addAura(unsigned int aura) {
    totalAura += aura;
}

void Squad::removeAura(unsigned int aura) {
    if (totalAura >= aura) {
        totalAura -= aura;
    }
}

void Squad::incrementSize() {
    squadSize++;
}

void Squad::decrementSize() {
    if (squadSize > 0) {
        squadSize--;
    }
}

void Squad::updateExp(int exp) {
    squadExp = exp;
}

void Squad::incrementFights() {
    totalFights++;
}

void Squad::setUFNode(UFNode* node) {
    ufNode = node;
}

void Squad::addNenAbility(const NenAbility& nen) {
    totalNenAbility += nen;
}

void Squad::removeNenAbility(const NenAbility& nen) {
    totalNenAbility -= nen;
}
