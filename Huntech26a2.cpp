// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"
#include "HASH_TABLE.h"

Huntech::Huntech() {}

Huntech::~Huntech() {}

StatusType Huntech::add_squad(int squadId) {
    return StatusType::FAILURE;
}

StatusType Huntech::remove_squad(int squadId) {
    return StatusType::FAILURE;
}

StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{
  if (hunterId <= 0 || squadId <= 0 || aura < 0 || fightsHad < 0
      || nenType.isValid())
        return StatusType::INVALID_INPUT;
  Squad** squad_ptr = squads.find(squadId);
  if(!squad_ptr || hunters.find(hunterId) != nullptr)
    return StatusType::FAILURE;
  try{
  Hunter* hunter = new Hunter(hunterId, squadId, nenType, aura, fightsHad);
  hunters.insert(hunterId, hunter);
  hunter->ufNode = std::make_shared<UFNode>(*squad_ptr);
   (*squad_ptr)->addHunter(*hunter);
  } catch( std::bad_alloc ) { return StatusType::ALLOCATION_ERROR; }

    return StatusType::SUCCESS;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    return 0;
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
  if (hunterId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
  auto hunterPtr = hunters.find(hunterId);
    if (!hunterPtr)
		return output_t<int>(StatusType::FAILURE);
    try {
        // do path compression to hold the O(Log*n) amoritized complexity
        UnionFind::find(hunterPtr->ufNode);

        return output_t<int>(hunterPtr->fightsHad);
    } catch (std::bad_alloc&) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }

    return 0;

}

output_t<int> Huntech::get_squad_experience(int squadId) {
    return 0;
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    return 0;
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
  if (hunterId <= 0)
        return output_t<NenAbility>(StatusType::INVALID_INPUT);
  auto hunterPtr = hunters.find(hunterId);
    if (!hunterPtr)
		return output_t<NenAbility>(StatusType::FAILURE);
    return NenAbility();
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    return StatusType::FAILURE;
}
