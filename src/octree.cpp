#include <pcl/octree/octree_search.h>

#include <base.hpp>

class OctreeManager
{
public:
    using Key = std::pair<size_t, float>;
    using Tree = pcl::octree::OctreePointCloudSearch<Point>;

    struct KeyHash
    {
        std::size_t operator() (const Key& pair) const {
            return std::hash<size_t>()(pair.first) ^ std::hash<float>()(pair.second);
        }
    };

    using Map = std::unordered_map<Key, Tree, KeyHash>;
    
private:
    static Map tree_map_;

    static Tree build_tree(CloudConstPtr cloud, float resolution)
    {
        Tree octree { resolution };
        octree.setInputCloud(cloud);
        octree.addPointsFromInputCloud();
        return octree;
    }

public:
    static Tree get_tree(size_t num_points, float resolution)
    {
        auto itr = tree_map_.find({num_points, resolution });
        if (itr == tree_map_.end())
        {
            auto cloud = CloudManager::get_search_cloud(num_points);
            Tree tree = build_tree(cloud, resolution);
            tree_map_.insert({ { num_points, resolution }, tree });
            return tree;
        }
        else
        {
            return itr->second;
        }
    }
};

OctreeManager::Map OctreeManager::tree_map_;

static void BM_Octree_Search(benchmark::State& state)
{
    float resolution = state.range(4);
    BM_Search<OctreeManager>(state, resolution);
}

BENCHMARK(BM_Octree_Search)
    ->ArgsProduct({
        benchmark::CreateRange(10000, 1000000, 10),
        { 1000 },
        { 1 },
        { 0 },
        { 128 }
    })
    ->Unit(benchmark::TimeUnit::kMillisecond)
;
