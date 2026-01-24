#ifndef SQUAD_H
#define SQUAD_H

#include "wet2util.h"

// key for ordering squads by collective aura
struct AuraKey {
    int totalAura;
    int squadId;
    
    AuraKey(int aura, int id) : totalAura(aura), squadId(id) {}
    
    bool operator<(const AuraKey& other) const {
        if (totalAura != other.totalAura) {
            return totalAura < other.totalAura;
        }
        return squadId < other.squadId;  // tie-break by smaller squadId
    }
    
    bool operator>(const AuraKey& other) const {
        return other < *this;
    }
    
    bool operator==(const AuraKey& other) const {
        return totalAura == other.totalAura && squadId == other.squadId;
    }
};

class UFNode;
class Squad{
    private:
    int squadId;
    unsigned int totalAura;
    UFNode* ufNode;                  
    int squadSize;
    int squadExp;
    int totalFights;
    NenAbility totalNenAbility;
    bool active;
    
    public:
    Squad(int id);
    ~Squad();
    
    int getSquadId() const;
    unsigned int getTotalAura() const;
    int getSquadSize() const;
    int getSquadExp() const;
    int getTotalFights() const;
    UFNode* getUFNode() const;
    NenAbility getNenAbility() const;
    bool isActive() const;
    
    void addAura(unsigned int aura);          
    void removeAura(unsigned int aura);       
    void incrementSize();                     
    void decrementSize();                     
    void updateExp(int exp);
    void incrementFights();
    void setUFNode(UFNode* node);
    void addNenAbility(const NenAbility& nen);
    void removeNenAbility(const NenAbility& nen);
    void setActive(bool active);
    void setSquadId(int squadId);
};








#endif 
