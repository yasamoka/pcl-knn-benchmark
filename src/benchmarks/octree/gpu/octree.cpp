#include <pcl/gpu/octree/octree.hpp>

#include <benchmarks/base.hpp>
#include <benchmarks/octree/gpu/octree.hpp>

using OctreeDevice = pcl::gpu::Octree;
using OctreeDevicePtr = std::shared_ptr<OctreeDevice>;
using OctreeDeviceConstPtr = std::shared_ptr<const OctreeDevice>;

using OctreeCloud = pcl::gpu::Octree::PointCloud;
using OctreeCloudPtr = std::shared_ptr<OctreeCloud>;

class OctreeDeviceWrapper
{
    OctreeDevicePtr octree_device_;
    OctreeCloudPtr cloud_device_;

public:
    OctreeDeviceWrapper() :
        octree_device_(std::make_shared<OctreeDevice>())
    {}

    ~OctreeDeviceWrapper()
    {
        //octree_device_->clear();
    }
    
    void setCloud(OctreeCloudPtr cloud_device)
    {
        cloud_device_ = cloud_device;
        octree_device_->setCloud(*cloud_device_);
    }

    void build()
    {
        octree_device_->build();
    }

    OctreeDeviceConstPtr octree_device()
    {
        return octree_device_;
    }
};

class GPUOctreeManager
{
public:
    using Queries = pcl::gpu::Octree::Queries;
    using QueriesPtr = std::shared_ptr<Queries>;
    using QueriesConstPtr = std::shared_ptr<const Queries>;

public:
    using Map = std::unordered_map<size_t, OctreeDeviceWrapper>;

private:
    static Map tree_map_;

    static OctreeDeviceWrapper build_octree(CloudConstPtr cloud)
    {
        OctreeCloudPtr cloud_device = std::make_shared<OctreeCloud>();
        cloud_device->upload(cloud->points);

        OctreeDeviceWrapper octree_device_wrapper;
        octree_device_wrapper.setCloud(cloud_device);
        octree_device_wrapper.build();
        return octree_device_wrapper;
    }

public:
    static QueriesPtr get_queries(size_t num_points)
    {
        CloudConstPtr cloud_host = CloudManager::get_query_cloud(num_points);
        QueriesPtr queries_device = std::make_shared<Queries>();
        queries_device->upload(cloud_host->points);
        return queries_device;
    }
    
    static OctreeDeviceConstPtr get_octree(size_t num_points)
    {
        auto itr = tree_map_.find(num_points);
        if (itr == tree_map_.end())
        {
            auto cloud = CloudManager::get_search_cloud(num_points);
            auto octree_device_wrapper = build_octree(cloud);
            tree_map_.insert({ num_points, octree_device_wrapper });
            auto octree_device = octree_device_wrapper.octree_device();
            return octree_device;
        }
        else
        {
            auto octree_device_wrapper = itr->second;
            return octree_device_wrapper.octree_device();
        }
    }
};

GPUOctreeManager::Map GPUOctreeManager::tree_map_;

void BM_GPU_Octree_Build_Tree(benchmark::State& state)
{
    size_t num_search_points = state.range(0);

    CloudConstPtr search_cloud = CloudManager::get_search_cloud(num_search_points);
    
    for (auto _ : state)
    {
        state.PauseTiming();
        OctreeDeviceWrapper octree_device_wrapper;
        state.ResumeTiming();
        OctreeCloudPtr cloud_device = std::make_shared<OctreeCloud>();
        cloud_device->upload(search_cloud->points);
        octree_device_wrapper.setCloud(cloud_device);
        octree_device_wrapper.build();
    }
}

void BM_GPU_Octree_Search(benchmark::State& state)
{
    size_t num_search_points = state.range(0);
    size_t num_query_points = state.range(1);
    constexpr int k = 1;
    
    auto octree_device = GPUOctreeManager::get_octree(num_search_points);
    auto queries_device = GPUOctreeManager::get_queries(num_query_points);

    pcl::gpu::NeighborIndices result_device;
    pcl::gpu::Octree::ResultSqrDists result_sqr_distances;

    for (auto _ : state)
    {
        octree_device->nearestKSearchBatch(*queries_device, k, result_device, result_sqr_distances);
    }
}
