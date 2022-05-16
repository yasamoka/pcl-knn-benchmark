#include <base.hpp>

class BruteForceTreeManager
{
    class BruteForceTree
    {
        CloudConstPtr cloud_;
    
    public:
        void setInputCloud(CloudConstPtr cloud)
        {
            cloud_ = cloud;
        }

        void nearestKSearch(const Point& point, int k, pcl::Indices& indices, std::vector<float>& sqr_distances)
        {
            if (k == 1)
            {
                std::vector<float> all_sqr_distances(cloud_->points.size());
                std::transform(
                    cloud_->points.cbegin(),
                    cloud_->points.cend(),
                    all_sqr_distances.begin(),
                    [&point](const Point& cloud_point)
                    {
                        std::array<float, 3> dists {
                            point.x - cloud_point.x,
                            point.y - cloud_point.y,
                            point.z - cloud_point.z
                        };
                        return std::accumulate(
                            dists.cbegin(),
                            dists.cend(),
                            0.0f,
                            [](const float& accum, const float& dist)
                            { return accum + dist * dist; }
                        );
                    }
                );
                
                auto min_dist_itr = std::min_element(
                    all_sqr_distances.cbegin(),
                    all_sqr_distances.cend()
                );

                size_t index = std::distance(min_dist_itr, all_sqr_distances.cbegin());
                
                indices[0] = index;
                sqr_distances[0] = *min_dist_itr;
            }
            else
            {
                std::vector<std::pair<size_t, float>> all_sqr_distances(cloud_->points.size());
                for (size_t i = 0; i < cloud_->points.size(); ++i)
                {
                    const Point& cloud_point = cloud_->points[i];
                    
                    std::array<float, 3> dists {
                        point.x - cloud_point.x,
                        point.y - cloud_point.y,
                        point.z - cloud_point.z
                    };
                    float sqr_distance = std::accumulate(
                        dists.cbegin(),
                        dists.cend(),
                        0.0f,
                        [](const float& accum, const float& dist)
                        { return accum + dist * dist; }
                    );

                    all_sqr_distances[i] = { i, sqr_distance };
                }
                
                std::nth_element(
                    all_sqr_distances.begin(),
                    all_sqr_distances.begin() + k,
                    all_sqr_distances.end()
                );
                
                std::transform(
                    all_sqr_distances.cbegin(),
                    all_sqr_distances.cbegin() + k,
                    indices.begin(),
                    [](const auto& pair)
                    { return pair.first; }
                );

                std::transform(
                    all_sqr_distances.cbegin(),
                    all_sqr_distances.cbegin() + k,
                    sqr_distances.begin(),
                    [](const auto& pair)
                    { return pair.second; }
                );
            }
        }
    };

public:
    using Tree = BruteForceTree;
    using Map = std::unordered_map<size_t, Tree>;

private:
    static Map tree_map_;

public:
    static Tree build_tree(CloudConstPtr cloud)
    {
        Tree brute_force_tree;
        brute_force_tree.setInputCloud(cloud);
        return brute_force_tree;
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

BruteForceTreeManager::Map BruteForceTreeManager::tree_map_;

static void BM_BruteForce_Search(benchmark::State& state)
{
    BM_Search<BruteForceTreeManager>(state);
}

BENCHMARK(BM_BruteForce_Search)
    ->ArgsProduct({
        benchmark::CreateRange(10000, 1000000, 10),
        { 1000 },
        { 1 },
        { 0, 1 }
    })
    ->Unit(benchmark::TimeUnit::kMillisecond)
;
