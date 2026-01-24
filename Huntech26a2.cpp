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
        squadPtr->setUFNode(new UFNode(squadPtr));  // Create UFNode with shared_ptr
        squads.insert(squadId, squadPtr);
        squadsByAura.insert(AuraKey(0, squadId), squadPtr);  
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
    

    
    try {
         // Mark as inactive
        (*squad_ptr)->setActive(false);
        AuraKey auraKey((*squad_ptr)->getTotalAura(), squadId);
        squadsByAura.remove(auraKey);
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
  

  AuraKey oldKey(squad->getTotalAura(), squad->getSquadId());
  squadsByAura.remove(oldKey);
  
hunter->ufNode = std::make_shared<UFNode>(*squad_ptr);


  UFNode* squadRoot = squad->getUFNode()->find();
  

  // Adjust for possible root offsets (see force_join union-by-size)
  NenAbility rootOffset = squadRoot->getDelta();
  int rootFightsOffset = squadRoot->getFightsDelta();

  hunter->ufNode.get()->setDelta(squadRoot->getTotalNen() - rootOffset);
  

  hunter->ufNode.get()->setFightsDelta(-(squadRoot->getSquad()->getTotalFights() + rootFightsOffset));
  

  hunter->ufNode.get()->setParent(squadRoot);
  

  squadRoot->setTotalNen(squadRoot->getTotalNen() + nenType);
  squadRoot->incrementSize();
  

  squad->incrementSize();
  squad->addAura(aura);
  squad->addNenAbility(nenType);
  

  AuraKey newKey(squad->getTotalAura(), squad->getSquadId());
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
        
        UFNode* root1 = squad1->getUFNode();
        UFNode* root2 = squad2->getUFNode();


        root1->getSquad()->incrementFights();
        root2->getSquad()->incrementFights();

        int result = 0;


        if (effectiveness1 > effectiveness2) {
            squad1->updateExp(squad1->getSquadExp() + 3);
            result = 1;
        } else if (effectiveness1 < effectiveness2) {
            squad2->updateExp(squad2->getSquadExp() + 3);
            result = 3;
        } else {

            NenAbility nen1 = squad1->getNenAbility();
            NenAbility nen2 = squad2->getNenAbility();

            if (nen1 > nen2) {
                squad1->updateExp(squad1->getSquadExp() + 3);
                result = 2;
            } else if (nen1 < nen2) {
                squad2->updateExp(squad2->getSquadExp() + 3);
                result = 4;
            } else {
                squad1->updateExp(squad1->getSquadExp() + 1);
                squad2->updateExp(squad2->getSquadExp() + 1);
                result = 0;
            }
        }



        return output_t<int>(result);

    } catch (const std::bad_alloc&) {
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

        UFNode* root = hunterPtr->ufNode->find();
        

        int totalFights = hunterPtr->fightsHad + 
                         hunterPtr->ufNode->getFightsDelta() + 
                         root->getFightsDelta() +
                         root->getSquad()->getTotalFights();
        
        return output_t<int>(totalFights);
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


  UFNode* root = hunterPtr->ufNode->find();
     if (root->getSquad() == nullptr || !root->getSquad()->isActive())
        return output_t<NenAbility>(StatusType::FAILURE);
    
    try {
        
        // result = this hunter's own NenAbility + delta (NenAbility before this hunter) + the root
        // because in the path compression it doesnt sum the delta of the root and store it in the hunter, which after find
        // will be the parent of the huneter(path compression)
        NenAbility result = hunterPtr->nenAbility + hunterPtr->ufNode->getDelta() + root->getDelta();
        
        return output_t<NenAbility>(result);
        
    } catch (std::bad_alloc&) {
        return output_t<NenAbility>(StatusType::ALLOCATION_ERROR);
    }
}

/*StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    if (forcingSquadId <= 0 || forcedSquadId <= 0 || forcingSquadId == forcedSquadId) {
        return StatusType::INVALID_INPUT;
    }

    auto forcing_ptr = squads.find(forcingSquadId);
    auto forced_ptr  = squads.find(forcedSquadId);
    if (!forcing_ptr || !forced_ptr) {
        return StatusType::FAILURE;
    }

    // Copy shared_ptr values before any tree mutations to avoid dangling pointers
    std::shared_ptr<Squad> forcing_shared = *forcing_ptr;
    std::shared_ptr<Squad> forced_shared  = *forced_ptr;

    Squad* forcing = forcing_shared.get();
    Squad* forced  = forced_shared.get();

    UFNode* forcingRoot = forcing->getUFNode()->find();
    UFNode* forcedRoot  = forced->getUFNode()->find();

    if (forcingRoot == forcedRoot) {
        return StatusType::FAILURE;
    }

    try {
        auto forcingSq = forcingRoot->getSquad();
        auto forcedSq  = forcedRoot->getSquad();

        int forcingEff =
            forcingSq->getSquadExp() +
            forcingSq->getTotalAura() +
            forcingSq->getNenAbility().getEffectiveNenAbility();

        int forcedEff =
            forcedSq->getSquadExp() +
            forcedSq->getTotalAura() +
            forcedSq->getNenAbility().getEffectiveNenAbility();

        // forcing must win
        if (forcingEff <= forcedEff) {
            return StatusType::FAILURE;
        }


         // Remove forced squad from AVL trees
        squads.remove(forcedSquadId);

        // Remove both squads from aura ranking
        squadsByAura.remove(AuraKey(forcingSq->getTotalAura(), forcingSq->getSquadId()));
        squadsByAura.remove(AuraKey(forcedSq->getTotalAura(),  forcedSq->getSquadId()));

        NenAbility forcingTotalNen = forcingRoot->getTotalNen();
        NenAbility forcedTotalNen  = forcedRoot->getTotalNen();

        int forcingFights = forcingSq->getTotalFights();
        int forcedFights  = forcedSq->getTotalFights();

        // Union by size (STAYS)
        if (forcingRoot->getSize() >= forcedRoot->getSize()) {
            // forcingRoot stays root
            forcedRoot->setParent(forcingRoot);
            forcedRoot->setDelta(forcingTotalNen);
            forcedRoot->setFightsDelta(forcedFights - forcingFights);

            forcingRoot->setSize(forcingRoot->getSize() + forcedRoot->getSize());
            forcingRoot->setTotalNen(forcingTotalNen + forcedTotalNen);
            forcingRoot->setSquad(forcingSq);

            // Mark forced squad as inactive
        forcedSq->setActive(false);

            // Merge squad stats INTO forcing squad
            forcingSq->addAura(forcedSq->getTotalAura());
            forcingSq->addNenAbility(forcedSq->getNenAbility());
            forcingSq->updateExp(forcingSq->getSquadExp() + forcedSq->getSquadExp());
        } *//*else {
            // forcedRoot stays root
            forcingRoot->setParent(forcedRoot);
            forcedRoot->setDelta(forcedRoot->getDelta()+forcingTotalNen);
            forcingRoot->setDelta(forcingRoot->getDelta()-forcedRoot->getDelta());
            forcingRoot->setFightsDelta(forcingFights - forcedFights);

            forcedRoot->setSize(forcedRoot->getSize() + forcingRoot->getSize());
            // Merge squad stats INTO forcing squad
            forcingSq->addAura(forcedSq->getTotalAura());
            forcingSq->addNenAbility(forcedSq->getNenAbility());
            forcingSq->updateExp(forcingSq->getSquadExp() + forcedSq->getSquadExp());
            forced->setSquadId(forcing->getSquadId()); // identity stays forcing
            forcing->setUFNode(forced->getUFNode());
        }// not fixing the squad'id when uniting to the bigger (in the else case),
        // there are mistakes in the nenabiilitydelta in the else case, read dry part.*/
/*

        // Insert forcing squad back
        squadsByAura.insert(
            AuraKey(forcingSq->getTotalAura(), forcingSq->getSquadId()),
            forcing_shared
        );

        return StatusType::SUCCESS;

    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}*/
StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    if (forcingSquadId <= 0 || forcedSquadId <= 0 || forcingSquadId == forcedSquadId) {
        return StatusType::INVALID_INPUT;
    }

    auto forcing_ptr = squads.find(forcingSquadId);
    auto forced_ptr  = squads.find(forcedSquadId);
    if (!forcing_ptr || !forced_ptr) {
        return StatusType::FAILURE;
    }

    // Copy shared_ptr values before mutating the AVL tree to avoid dangling pointers
    std::shared_ptr<Squad> forcing_shared = *forcing_ptr;
    std::shared_ptr<Squad> forced_shared  = *forced_ptr;

    Squad* forcing = forcing_shared.get();
    Squad* forced  = forced_shared.get();

    UFNode* forcingRoot = forcing->getUFNode()->find();
    UFNode* forcedRoot  = forced->getUFNode()->find();

    if (forcingRoot == forcedRoot) {
        return StatusType::FAILURE;
    }

    try {
        auto forcingSq = forcingRoot->getSquad();
        auto forcedSq  = forcedRoot->getSquad();

        int forcingEff =
            forcingSq->getSquadExp() +
            forcingSq->getTotalAura() +
            forcingSq->getNenAbility().getEffectiveNenAbility();

        int forcedEff =
            forcedSq->getSquadExp() +
            forcedSq->getTotalAura() +
            forcedSq->getNenAbility().getEffectiveNenAbility();

        //
        if (forcingEff <= forcedEff) {
            return StatusType::FAILURE;
        }




        squads.remove(forcedSquadId);

        squadsByAura.remove(AuraKey(forcingSq->getTotalAura(), forcingSq->getSquadId()));
        squadsByAura.remove(AuraKey(forcedSq->getTotalAura(),  forcedSq->getSquadId()));

        NenAbility forcingTotalNen = forcingRoot->getTotalNen();
        NenAbility forcedTotalNen  = forcedRoot->getTotalNen();

        NenAbility forcingOffsetNen = forcingRoot->getDelta();
        NenAbility forcedOffsetNen  = forcedRoot->getDelta();

        int forcingFights = forcingSq->getTotalFights();
        int forcedFights  = forcedSq->getTotalFights();

        int forcingOffsetF = forcingRoot->getFightsDelta();
        int forcedOffsetF  = forcedRoot->getFightsDelta();

        // Union by size (larger root stays)
        UFNode* root = nullptr;
        UFNode* child = nullptr;
        bool rootIsForcing = (forcingRoot->getSize() >= forcedRoot->getSize());
        if (rootIsForcing) {
            root = forcingRoot;
            child = forcedRoot;
        } else {
            root = forcedRoot;
            child = forcingRoot;
        }

        // Apply offsets to keep partial nen and fights correct (forcing squad identity wins)
        if (rootIsForcing) {
            // root is forcingRoot, keep its offsets as-is
            child->setParent(root);
            child->setDelta(forcedOffsetNen + forcingTotalNen - forcingOffsetNen);
            child->setFightsDelta(forcedOffsetF + forcedFights - forcingOffsetF - forcingFights);
        } else {
            // root is forcedRoot, shift its offsets to account for forcing squad identity
            NenAbility newRootOffsetNen = forcedOffsetNen + forcingTotalNen;
            int newRootOffsetF = forcedOffsetF + forcedFights - forcingFights;

            root->setDelta(newRootOffsetNen);
            root->setFightsDelta(newRootOffsetF);

            child->setParent(root);
            child->setDelta(forcingOffsetNen - newRootOffsetNen);
            child->setFightsDelta(forcingOffsetF - newRootOffsetF);
        }

        root->setSize(forcingRoot->getSize() + forcedRoot->getSize());
        root->setTotalNen(forcingTotalNen + forcedTotalNen);
        root->setSquad(forcing_shared);




        forcingSq->addAura(forcedSq->getTotalAura());
        forcingSq->addNenAbility(forcedSq->getNenAbility());
        forcingSq->updateExp(
            forcingSq->getSquadExp() + forcedSq->getSquadExp()
        );


        // Mark forced squad inactive
        forcedSq->setActive(false);

        squadsByAura.insert(
            AuraKey(forcingSq->getTotalAura(), forcingSq->getSquadId()),
            forcing_shared
        );

        return StatusType::SUCCESS;

    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}
