#pragma once

#include <pcl/point_types.h>

using Point = pcl::PointXYZ;
using Cloud = pcl::PointCloud<Point>;
using CloudPtr = std::shared_ptr<Cloud>;
using CloudConstPtr = std::shared_ptr<const Cloud>;
