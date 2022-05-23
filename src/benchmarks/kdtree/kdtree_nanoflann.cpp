#include <pcl/kdtree/kdtree_nanoflann.h>

#include <benchmarks/kdtree/base.hpp>
#include <benchmarks/kdtree/kdtree_nanoflann.hpp>
#include <managers/kdtree.hpp>

using Point = pcl::PointXYZ;
using Tree = pcl::KdTreeNanoflann<Point>;

class KdTreeNanoflannManager : public KdTreeManager<Tree>
{};

void BM_KdTreeNanoflann_Build_Tree(benchmark::State& state)
{
    BM_KdTree_Build_Tree<Tree>(state);
}

void BM_KdTreeNanoflann_Search(benchmark::State& state)
{
    BM_KdTree_Search<KdTreeNanoflannManager>(state);
}
