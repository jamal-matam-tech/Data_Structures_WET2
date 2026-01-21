#ifndef SQUAD_H
#define SQUAD_H

#include "wet2util.h"

class UFNode;
class Squad{
    private:
    int squadId;
    unsigned int totalAura;
    UFNode* ufNode;                  
    int squadSize;
    int squadExp;
    NenAbility totalNenAbility;
    
    public:
    Squad(int id);
    ~Squad();
    
    int getSquadId() const;
    unsigned int getTotalAura() const;
    int getSquadSize() const;
    int getSquadExp() const;
    UFNode* getUFNode() const;
    NenAbility getNenAbility() const;
    
    void addAura(unsigned int aura);          
    void removeAura(unsigned int aura);       
    void incrementSize();                     
    void decrementSize();                     
    void updateExp(int exp);                  
    void setUFNode(UFNode* node);
    void addNenAbility(const NenAbility& nen);
    void removeNenAbility(const NenAbility& nen);           
};








#endif 
