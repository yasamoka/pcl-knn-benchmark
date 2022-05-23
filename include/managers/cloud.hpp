#pragma once

#include <execution>
#include <random>
#include <unordered_map>

#include <Eigen/Dense>
#include <benchmark/benchmark.h>
#include <pcl/io/auto_io.h>

#include <types.hpp>

template <class T>
struct FixtureTraits {};

class CloudManager
{
    using Map = std::unordered_map<size_t, CloudPtr>;
    
    static Map search_cloud_map_, query_cloud_map_;
    
    static CloudPtr generate_cloud(size_t num_points)
    {
        std::random_device rd;
        std::mt19937 gen { rd() };
        std::uniform_real_distribution<float> dis { 0.0f, 1.0f };
        
        auto cloud = std::make_shared<Cloud>();
        cloud->points.resize(num_points);
        std::generate(
            cloud->points.begin(),
            cloud->points.end(),
            [&dis, &gen]()
            {
                return pcl::PointXYZ(
                    dis(gen),
                    dis(gen),
                    dis(gen)
                );
            }
        );
        return cloud;
    }

    static CloudConstPtr get_cloud(size_t num_points, std::unordered_map<size_t, CloudPtr>& cloud_map)
    {
        auto itr = cloud_map.find(num_points);
        if (itr == cloud_map.end())
        {
            auto cloud = generate_cloud(num_points);
            cloud_map.insert({ num_points, cloud });
            return cloud;
        }
        else
        {
            return itr->second;
        }
    }

public:
    static CloudConstPtr get_search_cloud(size_t num_points)
    {
        return get_cloud(num_points, search_cloud_map_);
    }

    static CloudConstPtr get_query_cloud(size_t num_points)
    {
        return get_cloud(num_points, query_cloud_map_);
    }
};
