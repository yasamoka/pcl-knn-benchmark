import matplotlib.pyplot as plt
import numpy as np

from utils import (
    assert_column_single_valued,
    generate_bar_offsets,
    get_default_argparser,
    get_default_args,
    merge_dfs,
    run_default_cleanup,
    run_default_assertions,
    save_figure,
    set_display_scale,
)


THREADING_MAP = ["Single-threaded", "Multithreaded", "CUDA", "Multithreaded / CUDA"]
THREADING_FILENAME_MAP = ["st", "mt", "cuda", "mt_cuda"]

parser = get_default_argparser()
parser.add_argument(
    "--show-line-plots", action="store_true", help="Show line plots for max leaf size"
)
parser.add_argument(
    "--merge-multithreaded-cuda",
    action="store_true",
    help="Merge multithreaded and CUDA benchmark results",
)
args = parser.parse_args()
(
    csv_filepaths,
    setup_names,
    display_scale,
    show,
    fig_output_dir,
    compare,
) = get_default_args(args)
show_line_plots = args.show_line_plots
merge_multithreaded_cuda = args.merge_multithreaded_cuda

set_display_scale(display_scale)
df = merge_dfs(csv_filepaths, setup_names)

params = [
    "num_search_points",
    "num_query_points",
    "k",
    "threading",
    "max_leaf_size",
]
groupby_params = ["name", "num_search_points", "num_query_points"]
single_valued_params = ["k"]
category_width = 0.7

### splits ###
df[["name"] + params + [""]] = df.name.str.split("/", expand=True)
df[["", "name", ""]] = df.name.str.split(r"BM_(.+)_Search", regex=True, expand=True)

### conversions ###
df[params] = df[params].astype(int, copy=False)

if merge_multithreaded_cuda:
    df.loc[df.threading.isin((1, 2)), "threading"] = 3

### transforms ###
if compare:
    df.name = df.setup_name + " - " + df.name

### assertions ###
run_default_assertions(df)
for column_name in single_valued_params:
    assert_column_single_valued(df[column_name])

### constants ###
k = df.k[0]

### cleanups ###
run_default_cleanup(df)
df.drop(single_valued_params, axis="columns", inplace=True)

### variables ###
num_search_points_set = sorted(df.num_search_points.unique())
num_query_points_set = sorted(df.num_query_points.unique())
max_leaf_size_set = sorted(df.max_leaf_size.unique())
threading_set = sorted(df.threading.unique())
name_set = df.name.unique()

for num_query_points in num_query_points_set:
    subset_df1 = df[df.num_query_points == num_query_points]

    if show_line_plots:
        for num_search_points in num_search_points_set:
            subset_df2 = subset_df1[subset_df1.num_search_points == num_search_points]

            for threading in threading_set:
                fig, ax = plt.subplots(1, 1)

                fig.suptitle(
                    "NN search\n#search points = {:,}, #query points = {:,}, K = {}, {}".format(
                        num_search_points, num_query_points, k, THREADING_MAP[threading]
                    ),
                    fontweight="bold",
                )

                subset_df3 = subset_df2[subset_df2.threading == threading]

                for name in df.name.unique():
                    subset_df4 = subset_df3[subset_df3.name == name]
                    (p,) = ax.plot(
                        subset_df4.max_leaf_size, subset_df4.real_time, label=name
                    )
                    ax.axhline(
                        y=subset_df4.real_time.min(),
                        color=p.get_color(),
                        linestyle="--",
                    )

                ax.set_xscale("log")
                ax.set_xlabel("Max leaf size")
                ax.set_ylabel("Time (ms)")
                ax.set_xticks(
                    ticks=max_leaf_size_set,
                    labels=[
                        "{:,}".format(max_leaf_size)
                        for max_leaf_size in max_leaf_size_set
                    ],
                )
                ax.set_ylim(bottom=0)
                ax.legend()

    for threading in threading_set:
        bar_fig, ax = plt.subplots(1, 1)

        bar_fig.suptitle(
            "NN search\n# query points = {:,}, K = {}, {}".format(
                num_query_points, k, THREADING_MAP[threading]
            ),
            fontsize=13,
            fontweight="bold",
        )

        subset_df2 = (
            subset_df1[subset_df1.threading == threading].groupby(groupby_params).min()
        )
        subset_df2.reset_index(inplace=True)

        names = [name for name in name_set if name in subset_df2.name.unique()]

        num_bars = len(names)
        if num_bars == 0:
            continue

        bar_width = category_width / num_bars
        bar_offsets = generate_bar_offsets(bar_width, num_bars)

        for name, bar_offset in zip(names, bar_offsets):
            subset_df3 = subset_df2[subset_df2.name == name]
            p = ax.bar(
                x=np.arange(len(subset_df3)) + bar_offset,
                height=subset_df3.real_time,
                width=bar_width,
                label=name,
            )
            ax.bar_label(p, fmt="%.2f", label_type="edge")

        ax.set_xlabel("# search points")
        ax.set_ylabel("Time (ms)")
        ax.set_xticks(
            ticks=np.arange(len(num_search_points_set)),
            labels=[
                "{:,}".format(num_search_points)
                for num_search_points in num_search_points_set
            ],
        )
        ax.legend()

        save_figure(
            fig_output_dir,
            f"nn_search_num_query_{num_query_points}_k_{k}_{THREADING_FILENAME_MAP[threading]}.svg",
        )

if show:
    plt.show()
