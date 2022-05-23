#pragma once

#include <managers/cloud.hpp>

template <typename Tree>
class KdTreeManager
{
    using Key = std::pair<size_t, int>;

    struct key_hash
    {
        std::size_t operator() (const Key& pair) const {
            return std::hash<size_t>()(pair.first) ^ std::hash<int>()(pair.second);
        }
    };

public:
    using Map = std::unordered_map<Key, Tree, key_hash>;

private:
    static Map tree_map_;

public:
    static Tree build_tree(CloudConstPtr cloud, int leaf_max_size)
    {
        Tree kdtree { true, leaf_max_size };
        kdtree.setInputCloud(cloud);
        return kdtree;
    }

    static Tree get_tree(size_t num_points, int leaf_max_size)
    {
        auto key = std::make_pair(num_points, leaf_max_size);
        auto itr = tree_map_.find(key);
        if (itr == tree_map_.end())
        {
            auto cloud = CloudManager::get_search_cloud(num_points);
            Tree tree = build_tree(cloud, leaf_max_size);
            tree_map_.insert({ key, tree });
            return tree;
        }
        else
        {
            return itr->second;
        }
    }
};

template <typename Tree>
typename KdTreeManager<Tree>::Map KdTreeManager<Tree>::tree_map_;
