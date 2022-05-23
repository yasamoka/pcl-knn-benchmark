import os
from pathlib import Path

import matplotlib.pyplot as plt

from utils import (
    get_default_argparser,
    get_default_args,
    merge_dfs,
    run_default_cleanup,
    run_default_assertions,
    save_figure,
    set_display_scale,
)


parser = get_default_argparser()
args = parser.parse_args()
(
    csv_filepaths,
    setup_names,
    display_scale,
    show,
    fig_output_dir,
    compare,
) = get_default_args(args)

set_display_scale(display_scale)
df = merge_dfs(csv_filepaths, setup_names)

params = ["num_search_points", "max_leaf_size"]

### splits ###
df[["name"] + params + [""]] = df.name.str.split("/", expand=True)
df[["", "name", ""]] = df.name.str.split(r"BM_(.+)_Build_Tree", regex=True, expand=True)

### conversions ###
df[params] = df[params].astype(int, copy=False)

### transforms ###
if compare:
    df.name = df.setup_name + " - " + df.name

### assertions ###
run_default_assertions(df)

### cleanups ###
run_default_cleanup(df)
df.drop(["setup_name"], axis="columns", inplace=True)

for num_search_points in df.num_search_points.unique():
    subset_df1 = df[df.num_search_points == num_search_points]

    fig = plt.figure()
    ax = fig.add_subplot(111)

    fig.suptitle(
        f"Build tree\n# search points = {num_search_points}", fontweight="bold"
    )

    ax.set_xscale("log")
    ax.set_xlabel("Max leaf size")
    ax.set_ylabel("Time (ms)")

    for name in df.name.unique():
        subset_df2 = subset_df1[subset_df1.name == name]
        ax.plot(subset_df2.max_leaf_size, subset_df2.real_time, label=name)

    xticks = subset_df1.max_leaf_size.unique()
    ax.set_xticks(ticks=xticks, labels=xticks)
    ax.set_ylim(bottom=0)

    ax.legend()

    save_figure(fig_output_dir, f"build_tree_num_search_{num_search_points}.svg")

if show:
    plt.show()
