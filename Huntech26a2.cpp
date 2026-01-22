// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"
#include "Hunter.h"
#include "squad.h"
#include <stdio.h>
#include <stdexcept>

Huntech::Huntech() {}

Huntech::~Huntech() {}

StatusType Huntech::add_squad(int squadId) {
    if (squadId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    if (squads.find(squadId) != nullptr) {
        return StatusType::FAILURE;
    }
    
    try {
        auto squadPtr = std::make_shared<Squad>(squadId);
        squads.insert(squadId, squadPtr);
        squadsByAura.insert(AuraKey(0, squadId), squadPtr);  // Initial aura is 0
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    
    return StatusType::SUCCESS;
}

StatusType Huntech::remove_squad(int squadId) {
    if (squadId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    auto squad_ptr = squads.find(squadId);
    if (squad_ptr == nullptr) {
        return StatusType::FAILURE;
    }
    
    if ((*squad_ptr)->getSquadSize() > 0) {
        return StatusType::FAILURE;
    }
    
    try {
        squads.remove(squadId);
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    
    return StatusType::SUCCESS;
}

StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{
  if (hunterId <= 0 || squadId <= 0 || aura < 0 || fightsHad < 0
      || !nenType.isValid())
        return StatusType::INVALID_INPUT;
  auto squad_ptr = squads.find(squadId);
  if(!squad_ptr || hunters.find(hunterId) != nullptr)
    return StatusType::FAILURE;
  try{
  Hunter* hunter = new Hunter(hunterId, squadId, nenType, aura, fightsHad);
  hunters.insert(hunterId, hunter);
  
  Squad* squad = squad_ptr->get();
  
  // Remove old aura key from squadsByAura
  AuraKey oldKey(squad->getTotalAura(), squadId);
  squadsByAura.remove(oldKey);
  
  hunter->ufNode = std::make_shared<UFNode>(squad);
  
  // Get the root of this squad
  UFNode* squadRoot = squad->getUFNode()->find();
  
  // Set hunter's delta to current totalNen of the squad
  hunter->ufNode.get()->setDelta(squadRoot->getTotalNen());
  
  // Hunter's parent is the squad root
  hunter->ufNode.get()->setParent(squadRoot);
  
  // Update squad root's totalNen
  squadRoot->setTotalNen(squadRoot->getTotalNen() + nenType);
  
  // Update squad statistics
  squad->incrementSize();
  squad->addAura(aura);
  squad->addNenAbility(nenType);
  
  // Insert new aura key into squadsByAura
  AuraKey newKey(squad->getTotalAura(), squadId);
  squadsByAura.insert(newKey, *squad_ptr);
   
  } catch(const std::bad_alloc&) { return StatusType::ALLOCATION_ERROR; }

    return StatusType::SUCCESS;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    if (squadId1 <= 0 || squadId2 <= 0 || squadId1 == squadId2) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    auto squad1_ptr = squads.find(squadId1);
    auto squad2_ptr = squads.find(squadId2);
    
    if (squad1_ptr == nullptr || squad2_ptr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    Squad* squad1 = squad1_ptr->get();
    Squad* squad2 = squad2_ptr->get();
    
    if (squad1->getSquadSize() == 0 || squad2->getSquadSize() == 0) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    try {
        int effectiveness1 = squad1->getSquadExp() + squad1->getTotalAura();
        int effectiveness2 = squad2->getSquadExp() + squad2->getTotalAura();
        
        if (effectiveness1 > effectiveness2) {
            return output_t<int>(squadId1);
        } else if (effectiveness1 < effectiveness2) {
            return output_t<int>(squadId2);
        }
        
        NenAbility nen1 = squad1->getNenAbility();
        NenAbility nen2 = squad2->getNenAbility();
        
        if (nen1 > nen2) {
            return output_t<int>(squadId1);
        } else if (nen1 < nen2) {
            return output_t<int>(squadId2);
        }
        
        if (squad1->getSquadSize() > squad2->getSquadSize()) {
            return output_t<int>(squadId1);
        } else if (squad1->getSquadSize() < squad2->getSquadSize()) {
            return output_t<int>(squadId2);
        }
        
        if (squadId1 < squadId2) {
            return output_t<int>(squadId1);
        } else {
            return output_t<int>(squadId2);
        }
        
    } catch (std::bad_alloc&) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
  if (hunterId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
  auto hunterPtr = hunters.find(hunterId);
    if (!hunterPtr)
		return output_t<int>(StatusType::FAILURE);
    try {
        // Trigger path compression - O(log* n)
        hunterPtr->ufNode->find();

        return output_t<int>(hunterPtr->fightsHad);
    } catch (std::bad_alloc&) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }

    return 0;

}

output_t<int> Huntech::get_squad_experience(int squadId) {
    if (squadId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    auto squad_ptr = squads.find(squadId);
    if (squad_ptr == nullptr) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    try {
        int experience = (*squad_ptr)->getSquadExp();
        return output_t<int>(experience);
    } catch (std::bad_alloc&) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    if (i <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    int numSquads = squadsByAura.getSize();
    if (numSquads == 0 || i > numSquads) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    try {
        // Use select to get the i-th squad in O(log n) time
        auto squadPtr = squadsByAura.select(i);
        if (!squadPtr || !*squadPtr) {
            return output_t<int>(StatusType::FAILURE);
        }
        
        int squadId = (*squadPtr)->getSquadId();
        return output_t<int>(squadId);
        
    } catch (std::bad_alloc&) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
  if (hunterId <= 0)
        return output_t<NenAbility>(StatusType::INVALID_INPUT);
  auto hunterPtr = hunters.find(hunterId);
    if (!hunterPtr)
		return output_t<NenAbility>(StatusType::FAILURE);
    
    try {
        // Trigger path compression - O(log* n)
        hunterPtr->ufNode->find();
        
        // Result = this hunter's own NenAbility + delta (NenAbility before this hunter)
        NenAbility result = hunterPtr->nenAbility + hunterPtr->ufNode->getDelta();
        
        return output_t<NenAbility>(result);
        
    } catch (std::bad_alloc&) {
        return output_t<NenAbility>(StatusType::ALLOCATION_ERROR);
    }
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    if (forcingSquadId <= 0 || forcedSquadId <= 0 || forcingSquadId == forcedSquadId) {
        return StatusType::INVALID_INPUT;
    }

    auto forcing_ptr = squads.find(forcingSquadId);
    auto forced_ptr  = squads.find(forcedSquadId);
    if (!forcing_ptr || !forced_ptr) {
        return StatusType::FAILURE;
    }

    Squad* forcing = forcing_ptr->get();
    Squad* forced  = forced_ptr->get();

    UFNode* forcingRoot = forcing->getUFNode()->find();
    UFNode* forcedRoot = forced->getUFNode()->find();

    if (forcingRoot == forcedRoot) {
        return StatusType::FAILURE;
    }

    try {
        Squad* forcingSq = forcingRoot->getSquad();
        Squad* forcedSq = forcedRoot->getSquad();

        int forcingEff = forcingSq->getSquadExp()
                 + forcingSq->getTotalAura()
                 + forcingSq->getNenAbility().getEffectiveNenAbility();

        int forcedEff = forcedSq->getSquadExp()
                 + forcedSq->getTotalAura()
                 + forcedSq->getNenAbility().getEffectiveNenAbility();

        NenAbility forcingTotal = forcingRoot->getTotalNen();
        NenAbility forcedTotal = forcedRoot->getTotalNen();

        Squad* winnerSq;
        
        // Determine winner
        if (forcingEff > forcedEff) {
            // Forcing squad wins
            winnerSq = forcingSq;
        } else {
            // Forced squad wins (but takes forcing's identity)
            winnerSq = forcingSq;  // Winner takes forcing squad identity!
        }
        
        // Remove old keys from squadsByAura
        AuraKey oldForcingKey(forcingSq->getTotalAura(), forcingSq->getSquadId());
        AuraKey oldForcedKey(forcedSq->getTotalAura(), forcedSq->getSquadId());
        squadsByAura.remove(oldForcingKey);
        squadsByAura.remove(oldForcedKey);
        
        // Merge squad stats into forcing squad
        forcingSq->addAura(forcedSq->getTotalAura());
        forcingSq->addNenAbility(forcedSq->getNenAbility());
        forcingSq->updateExp(forcingSq->getSquadExp() + forcedSq->getSquadExp());
        
        // Perform union by size
        if (forcingRoot->getSize() >= forcedRoot->getSize()) {
            // forcingRoot becomes/stays root
            forcedRoot->setParent(forcingRoot);
            forcedRoot->setDelta(forcingTotal);
            forcingRoot->setSize(forcingRoot->getSize() + forcedRoot->getSize());
            forcingRoot->setTotalNen(forcingTotal + forcedTotal);
            forcingRoot->setSquad(winnerSq);
        } else {
            // forcedRoot stays root (but represents forcing squad)
            forcingRoot->setParent(forcedRoot);
            forcingRoot->setDelta(-forcedTotal); // Compensate for existing nen
            forcedRoot->setSize(forcedRoot->getSize() + forcingRoot->getSize());
            forcedRoot->setTotalNen(forcedTotal + forcingTotal);
            forcedRoot->setSquad(winnerSq);  // Update to forcing squad
        }
        
        // Insert new key into squadsByAura
        AuraKey newKey(forcingSq->getTotalAura(), forcingSq->getSquadId());
        squadsByAura.insert(newKey, *forcing_ptr);

        return StatusType::SUCCESS;

    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}
