
#define INTERPRETER

#ifdef INTERPRETER
#include "llvm/Support/DynamicAnalysis.h"
#else
#include "DynamicAnalysis.h"
#endif


TBV::TBV()
{
    tbv_map.resize(SplitTreeRange);
    e = true;
}

bool TBV::empty()
{
    return e;
}

void TBV::insert_node(uint64_t key, unsigned bitPosition)
{
    key = key % SplitTreeRange;
    e = false;
    tbv_map[key].BitVector[bitPosition] = 1;
}

void TBV::delete_node(uint64_t key, unsigned bitPosition)
{
    key = key % SplitTreeRange;
    tbv_map[key].BitVector[bitPosition] = 0;
}

bool TBV::get_node(uint64_t key, unsigned bitPosition)
{
    if (empty()) return false;
    key = key % SplitTreeRange;
    return (tbv_map[key].BitVector[bitPosition] == 1);
}

bool TBV::get_node_nb(uint64_t key, unsigned bitPosition)
{
    if (empty()) return false;
    key = key % SplitTreeRange;
    return (tbv_map[key].BitVector[bitPosition] == 0);
}

uint64_t BitScan(vector< TBV> &FullOccupancyCyclesTree, uint64_t key, unsigned bitPosition)
{
    uint64_t kLocal = key % SplitTreeRange;
    uint64_t chunk = kLocal / SplitTreeRange;
    
    while (chunk < FullOccupancyCyclesTree.size())
    {
        while (kLocal < SplitTreeRange)
        {
            if (FullOccupancyCyclesTree[chunk].get_node(kLocal, bitPosition)) return (kLocal + chunk * SplitTreeRange);
            kLocal++;
        }
        kLocal = 0;
        chunk++;
    }
    
    return key;
}
