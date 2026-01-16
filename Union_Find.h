
#include <memory>
#include "UnionFindNode.h"

#ifndef UNION_FIND_H
#define UNION_FIND_H

class UnionFind {
public:
    static std::shared_ptr<UFNode> find(std::shared_ptr<UFNode> x);

    static void unite(std::shared_ptr<UFNode> a, std::shared_ptr<UFNode> b);
};

#endif //UNION_FIND_H
