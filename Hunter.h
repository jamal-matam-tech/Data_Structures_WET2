
#ifndef HUNTER_H
#define HUNTER_H

#include "UnionFindNode.h"
#include <memory>
#include "wet2util.h"

class Hunter{
  public:
  int hunterId;
  int squadId;
  NenAbility nenAbility;
  int aura;
  int fightsHad;
  int chronologicalIndex; // Index when this hunter was added (for partial nen ability)
  std::shared_ptr<UFNode> ufNode; 

  Hunter(int hunterId, int squadId, const NenAbility& nenAbility, int aura,
      int fightsHad) : hunterId(hunterId), squadId(squadId),
    nenAbility(nenAbility), aura(aura),
  fightsHad(fightsHad), chronologicalIndex(-1), ufNode(nullptr){};
  ~Hunter() = default;


};

#endif 