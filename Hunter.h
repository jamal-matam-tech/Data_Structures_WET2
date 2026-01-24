
#ifndef HUNTER_H
#define HUNTER_H

#include "UnionFindNode.h"
#include <memory>
#include "wet2util.h"

class Squad;

class Hunter{
  public:
  int hunterId;
  int squadId;
  NenAbility nenAbility;
  int aura;
  int fightsHad;
  std::shared_ptr<UFNode> ufNode; 
  std::shared_ptr<Squad> squadPtr;

  Hunter(int hunterId, int squadId, const NenAbility& nenAbility, int aura,
      int fightsHad) : hunterId(hunterId), squadId(squadId),
    nenAbility(nenAbility), aura(aura),
  fightsHad(fightsHad), ufNode(nullptr), squadPtr(nullptr){};
  ~Hunter() = default;


};

#endif 
