#include <pcl/kdtree/kdtree_flann.h>

#include <benchmarks/kdtree/base.hpp>
#include <benchmarks/kdtree/kdtree_flann.hpp>
#include <managers/kdtree.hpp>

using Point = pcl::PointXYZ;
using Tree = pcl::KdTreeFLANN<Point>;

class KdTreeFLANNManager : public KdTreeManager<Tree>
{};

void BM_KdTreeFLANN_Build_Tree(benchmark::State& state)
{
    BM_KdTree_Build_Tree<Tree>(state);
}

void BM_KdTreeFLANN_Search(benchmark::State& state)
{
    BM_KdTree_Search<KdTreeFLANNManager>(state);
}
