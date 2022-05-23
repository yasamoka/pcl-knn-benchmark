#pragma once

#include <benchmark/benchmark.h>

void BM_CUDA_KdTreeFLANN_Build_Tree(benchmark::State& state);
void BM_CUDA_KdTreeFLANN_Search(benchmark::State& state);
