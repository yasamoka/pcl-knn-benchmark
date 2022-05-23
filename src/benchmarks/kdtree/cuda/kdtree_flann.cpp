#include <pcl/cuda/kdtree/kdtree_flann.h>

#include <benchmarks/kdtree/cuda/base.hpp>
#include <benchmarks/kdtree/cuda/kdtree_flann.hpp>
#include <managers/kdtree.hpp>

using Point = pcl::PointXYZ;
using Tree = pcl::cuda::KdTreeFLANN<Point>;

class CUDAKdTreeFLANNManager : public KdTreeManager<Tree>
{};

void BM_CUDA_KdTreeFLANN_Build_Tree(benchmark::State& state)
{
    BM_CUDA_KdTree_Build_Tree<Tree>(state);
}

void BM_CUDA_KdTreeFLANN_Search(benchmark::State& state)
{
    //BM_CUDA_KdTree_Search<CUDAKdTreeFLANNManager>(state);
    BM_CUDA_KdTree_Search<Tree>(state);
}
