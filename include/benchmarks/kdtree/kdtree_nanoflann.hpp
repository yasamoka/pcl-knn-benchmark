#pragma once

#include <benchmark/benchmark.h>

void BM_KdTreeNanoflann_Build_Tree(benchmark::State& state);
void BM_KdTreeNanoflann_Search(benchmark::State& state);
