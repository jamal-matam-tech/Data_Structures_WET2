
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
  std::shared_ptr<UFNode> ufNode; // pointer to the hunter's node in
  // the inverted tree that is used by the squad that the hunter is in

  Hunter(int hunterId, int squadId, const NenAbility& nenAbility, int aura,
      int fightsHad) : hunterId(hunterId), squadId(squadId),
    nenAbility(nenAbility), aura(aura),
  fightsHad(fightsHad),  ufNode(nullptr){};
  ~Hunter() = default;


};

#endif //HUNTER_H