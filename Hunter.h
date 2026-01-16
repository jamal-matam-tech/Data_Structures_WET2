
#ifndef HUNTER_H
#define HUNTER_H

#include "UnionFindNode.h"
#include <memory>
#include "wet2util.h"

class Hunter{
  public:
  int hunterId;
  int squadId;
  int aura;
  int fightsHad;
  NenAbility nenAbility;
  std::shared_ptr<UFNode> ufNode; // pointer to the hunter's node in
  // the inverted tree that is used by the squad that the hunter is in

  Hunter(int hunterId, int squadId, int aura, int fightsHad,
         const NenAbility& nenAbility) : hunterId(hunterId), squadId(squadId),
  aura(aura), fightsHad(fightsHad), nenAbility(nenAbility),  ufNode(nullptr){};
  ~Hunter() = default;


};

#endif //HUNTER_H