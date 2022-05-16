#include <pcl/kdtree/kdtree_flann.h>

#include <base.hpp>

class KdTreeFLANNManager
{
public:
    using Tree = pcl::KdTreeFLANN<pcl::PointXYZ>;
    using Map = std::unordered_map<size_t, Tree>;

private:
    static Map tree_map_;

public:
    static Tree build_tree(CloudConstPtr cloud)
    {
        Tree kdtree;
        kdtree.setInputCloud(cloud);
        return kdtree;
    }

    static Tree get_tree(size_t num_points)
    {
        auto itr = tree_map_.find(num_points);
        if (itr == tree_map_.end())
        {
            auto cloud = CloudManager::get_search_cloud(num_points);
            Tree tree = build_tree(cloud);
            tree_map_.insert({ num_points, tree });
            return tree;
        }
        else
        {
            return itr->second;
        }
    }
};

KdTreeFLANNManager::Map KdTreeFLANNManager::tree_map_;

static void BM_KdTreeFLANN_Search(benchmark::State& state)
{
    BM_Search<KdTreeFLANNManager>(state);
}

BENCHMARK(BM_KdTreeFLANN_Search)
    ->ArgsProduct({
        benchmark::CreateRange(10000, 1000000, 10),
        benchmark::CreateRange(1000, 1000000, 10),
        { 1 },
        { 0, 1 }
    })
    ->Unit(benchmark::TimeUnit::kMillisecond)
;
