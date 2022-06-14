
<head>
    <link rel="stylesheet" type="text/css" href="https://yasamoka.github.io/pcl-knn-benchmark/stylesheets/page.css"/>
</head>

# NVIDIA GeForce GTX 1080Ti

------

## Configuration


| **Parameter** | **Value** |
| PCI-E Bus | 3.0 x16 |

## Benchmarks

### Build tree

| **Parameter** | **Value** |
| \# search points | 100,000 |

<a href="plots/build-tree/num_search_100000.svg">
    <img src="plots/build-tree/num_search_100000.svg" alt="Build tree, # search points = 100,000"/>
</a>

| **Parameter** | **Value** |
| \# search points | 1,000,000 |

<a href="plots/build-tree/num_search_1000000.svg">
    <img src="plots/build-tree/num_search_1000000.svg" alt="Build tree, # search points = 1,000,000"/>
</a>

| **Parameter** | **Value** |
| \# search points | 10,000,000 |

<a href="plots/build-tree/num_search_10000000.svg">
    <img src="plots/build-tree/num_search_10000000.svg" alt="Build tree, # search points = 10,000,000"/>
</a>

### NN search

#### Best max leaf size

| **Parameter** | **Value** |
| \# query points | 100,000 |
| K | 1 |

##### CUDA:

<a href="plots/nn-search/bar/num_query_100000_k_1_cuda.svg">
    <img src="plots/nn-search/bar/num_query_100000_k_1_cuda.svg" alt="NN search, # query points = 100,000, K = 1, CUDA"/>
</a>

------

| **Parameter** | **Value** |
| \# query points | 1,000,000 |
| K | 1 |

##### CUDA:

<a href="plots/nn-search/bar/num_query_1000000_k_1_cuda.svg">
    <img src="plots/nn-search/bar/num_query_1000000_k_1_cuda.svg" alt="NN search, # query points = 1,000,000, K = 1, CUDA"/>
</a>

#### Detailed

| **Parameter** | **Value** |
| \# search points | 10,000 |
| \# query points | 100,000 |
| K | 1 |

##### CUDA:
<a href="plots/nn-search/line/num_search_10000_num_query_100000_k_1_cuda.svg">
    <img src="plots/nn-search/line/num_search_10000_num_query_100000_k_1_cuda.svg" alt="NN search, # search points = 10,000, # query points = 100,000, K = 1, CUDA"/>
</a>

------

| **Parameter** | **Value** |
| \# search points | 100,000 |
| \# query points | 100,000 |
| K | 1 |

##### CUDA:
<a href="plots/nn-search/line/num_search_100000_num_query_100000_k_1_cuda.svg">
    <img src="plots/nn-search/line/num_search_100000_num_query_100000_k_1_cuda.svg" alt="NN search, # search points = 100,000, # query points = 100,000, K = 1, CUDA"/>
</a>

------

| **Parameter** | **Value** |
| \# search points | 1,000,000 |
| \# query points | 100,000 |
| K | 1 |

##### CUDA:
<a href="plots/nn-search/line/num_search_1000000_num_query_100000_k_1_cuda.svg">
    <img src="plots/nn-search/line/num_search_1000000_num_query_100000_k_1_cuda.svg" alt="NN search, # search points = 1,000,000, # query points = 100,000, K = 1, CUDA"/>
</a>

------

| **Parameter** | **Value** |
| \# search points | 10,000,000 |
| \# query points | 100,000 |
| K | 1 |

##### CUDA:
<a href="plots/nn-search/line/num_search_10000000_num_query_100000_k_1_cuda.svg">
    <img src="plots/nn-search/line/num_search_10000000_num_query_100000_k_1_cuda.svg" alt="NN search, # search points = 10,000,000, # query points = 100,000, K = 1, CUDA"/>
</a>

------

| **Parameter** | **Value** |
| \# search points | 10,000 |
| \# query points | 1,000,000 |
| K | 1 |

##### CUDA:
<a href="plots/nn-search/line/num_search_10000_num_query_1000000_k_1_cuda.svg">
    <img src="plots/nn-search/line/num_search_10000_num_query_1000000_k_1_cuda.svg" alt="NN search, # search points = 10,000, # query points = 1,000,000, K = 1, CUDA"/>
</a>

------

| **Parameter** | **Value** |
| \# search points | 100,000 |
| \# query points | 1,000,000 |
| K | 1 |

##### CUDA:
<a href="plots/nn-search/line/num_search_100000_num_query_1000000_k_1_cuda.svg">
    <img src="plots/nn-search/line/num_search_100000_num_query_1000000_k_1_cuda.svg" alt="NN search, # search points = 100,000, # query points = 1,000,000, K = 1, CUDA"/>
</a>

------

| **Parameter** | **Value** |
| \# search points | 1,000,000 |
| \# query points | 1,000,000 |
| K | 1 |

##### CUDA:
<a href="plots/nn-search/line/num_search_1000000_num_query_1000000_k_1_cuda.svg">
    <img src="plots/nn-search/line/num_search_1000000_num_query_1000000_k_1_cuda.svg" alt="NN search, # search points = 1,000,000, # query points = 1,000,000, K = 1, CUDA"/>
</a>

------

| **Parameter** | **Value** |
| \# search points | 10,000,000 |
| \# query points | 1,000,000 |
| K | 1 |

##### CUDA:
<a href="plots/nn-search/line/num_search_10000000_num_query_1000000_k_1_cuda.svg">
    <img src="plots/nn-search/line/num_search_10000000_num_query_1000000_k_1_cuda.svg" alt="NN search, # search points = 10,000,000, # query points = 1,000,000, K = 1, CUDA"/>
</a>
## Generation

### Page (with plots)

```bash
PYTHONPATH=. python generate.py -s "NVIDIA GeForce GTX 1080Ti" -c ../../configs/desktop/1080ti.json -b ../../results/desktop/1080ti -o ../../docs/setups/desktop/1080ti
```

### Plots
```shell
PYTHONPATH=. python plots/build_tree.py -b ../../results/desktop/1080ti/build_tree.csv
PYTHONPATH=. python plots/nn_search.py -b ../../results/desktop/1080ti/nn_search.csv --draw-line-plots
```
