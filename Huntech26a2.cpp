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
        squads.insert(squadId, std::unique_ptr<Squad>(new Squad(squadId)));
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
        // do path compression to hold the O(Log*n) amoritized complexity
        UnionFind::find(hunterPtr->ufNode);

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

// this is incorrect , check
output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    if (i <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    int numSquads = squads.getSize();
    if (numSquads == 0) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    try {
        struct SquadGroup {
            UFNode* root;
            int representativeId;
            unsigned int collectiveAura;
        };
        
        SquadGroup* groups = new SquadGroup[numSquads];
        int groupCount = 0;
        
        squads.inOrderTraverse([&](int key, std::unique_ptr<Squad>* valPtr) {
            if (valPtr && *valPtr) {
                Squad* squad = valPtr->get();
            UFNode* root = squad->getUFNode()->find();
            
            int foundIndex = -1;
            for (int j = 0; j < groupCount; j++) {
                if (groups[j].root == root) {
                    foundIndex = j;
                    break;
                }
            }
            
            if (foundIndex == -1) {
                groups[groupCount].root = root;
                groups[groupCount].representativeId = root->getSquad()->getSquadId();
                groups[groupCount].collectiveAura = squad->getTotalAura();
                groupCount++;
            } else {
                groups[foundIndex].collectiveAura += squad->getTotalAura();
            }
            }
        });
        
        for (int j = 0; j < groupCount - 1; j++) {
            for (int k = 0; k < groupCount - j - 1; k++) {
                if (groups[k].collectiveAura < groups[k + 1].collectiveAura) {
                    SquadGroup temp = groups[k];
                    groups[k] = groups[k + 1];
                    groups[k + 1] = temp;
                }
            }
        }
        
        if (i > groupCount) {
            delete[] groups;
            return output_t<int>(StatusType::FAILURE);
        }
        
        int result = groups[i - 1].representativeId;
        delete[] groups;
        
        return output_t<int>(result);
        
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

//needs fixing
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

    UFNode* C = forcing->getUFNode()->find(); // forcing root
    UFNode* A = forced->getUFNode()->find();  // other root

    if (C == A) {
        return StatusType::FAILURE;
    }

    try {
        Squad* Csq = C->getSquad();
        Squad* Asq = A->getSquad();

        int Ceff = Csq->getSquadExp()
                 + Csq->getTotalAura()
                 + Csq->getNenAbility().getEffectiveNenAbility();

        int Aeff = Asq->getSquadExp()
                 + Asq->getTotalAura()
                 + Asq->getNenAbility().getEffectiveNenAbility();

        NenAbility C_total = C->getTotalNen();
        NenAbility A_total = A->getTotalNen();

        if (Ceff > Aeff) {
            // C (forcing) stays root
            A->setParent(C);
            A->setDelta(C_total);

            C->setTotalNen(C_total + A_total);
            C->setSquad(Csq);
        } else {
            // A stays root (YOUR SPECIAL CASE)
            C->setParent(A);

            // compensate so A's hunters don't gain C's nen
            C->setDelta(-A_total);

            A->setTotalNen(A_total + C_total);

            // IMPORTANT: squad id becomes forcing squad id
            A->setSquad(Csq);
        }

        // merge squad stats
        Csq->addAura(Asq->getTotalAura());
        Csq->addNenAbility(Asq->getNenAbility());
        Csq->updateExp(Csq->getSquadExp() + Asq->getSquadExp());

        return StatusType::SUCCESS;

    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}
