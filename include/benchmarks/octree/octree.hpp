#pragma once

#include <benchmark/benchmark.h>

void BM_CPU_Octree_Build_Tree(benchmark::State& state);
void BM_CPU_Octree_Search(benchmark::State& state);
