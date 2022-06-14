#include <pcl/octree/octree_search.h>

#include <benchmarks/base.hpp>
#include <benchmarks/octree/octree.hpp>

using Tree = pcl::octree::OctreePointCloudSearch<Point>;

class OctreeManager
{
public:
    using Key = std::pair<size_t, float>;

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

void BM_CPU_Octree_Build_Tree(benchmark::State& state)
{
    size_t num_search_points = state.range(0);
    float resolution = state.range(1);

    CloudConstPtr search_cloud = CloudManager::get_search_cloud(num_search_points);
    
    for (auto _ : state)
    {
        state.PauseTiming();
        Tree tree { resolution };
        state.ResumeTiming();
        tree.setInputCloud(search_cloud);
        tree.addPointsFromInputCloud();
    }
}

void BM_CPU_Octree_Search(benchmark::State& state)
{
    float resolution = state.range(4);
    BM_Search<OctreeManager>(state, resolution);
}
