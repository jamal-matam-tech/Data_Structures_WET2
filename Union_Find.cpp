
#include "Union_Find.h"

std::shared_ptr<UFNode> UnionFind::find(std::shared_ptr<UFNode> x) {
    if (!x) return nullptr;
    
    if (x->getParent() != x.get()) {
        x->setParent(find(std::shared_ptr<UFNode>(x->getParent(), [](UFNode*){})).get());
    }
    
    return std::shared_ptr<UFNode>(x->getParent(), [](UFNode*){});
}

void UnionFind::unite(std::shared_ptr<UFNode> a, std::shared_ptr<UFNode> b) {
    if (!a || !b) return;
    
    UFNode::unionSets(a.get(), b.get());
}
