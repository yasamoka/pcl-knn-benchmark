from pathlib import Path
from typing import Dict, List, Optional

import matplotlib.pyplot as plt
import numpy as np

from plots.utils import (
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


def plot_nn_search(
    csv_filepaths: List[Path],
    setup_names: List[str] = [],
    display_scale: float = 1,
    draw_titles: bool = False,
    fig_output_dir: Optional[Path] = None,
    compare: bool = False,
    draw_line_plots: bool = True,
    merge_multithreaded_cuda: bool = False,
    shorten_names: bool = False,
):
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
    if shorten_names:
        df[["", "", "name", ""]] = df.name.str.split(
            r"BM_(.*)KdTree(.+)_Search", regex=True, expand=True
        )
    else:
        df[["", "name", ""]] = df.name.str.split(
            r"BM_(.+)_Search", regex=True, expand=True
        )

    ### conversions ###
    df[params] = df[params].astype(int, copy=False)

    if merge_multithreaded_cuda:
        df.loc[df.threading.isin((1, 2)), "threading"] = 3

    ### transforms ###
    if compare:
        setup_name = (
            df.setup_name.str.split(" ").str[-1] if shorten_names else df.setup_name
        )
        df.name = setup_name + " - " + df.name

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
    name_set = df.name.unique()

    best_max_leaf_size_params = []
    detailed_params = []

    for num_query_points in num_query_points_set:
        subset_df1 = df[df.num_query_points == num_query_points]

        threading_set = sorted(subset_df1.threading.unique())

        if draw_line_plots:
            for num_search_points in num_search_points_set:
                subset_df2 = subset_df1[
                    subset_df1.num_search_points == num_search_points
                ]

                for threading in threading_set:
                    fig, ax = plt.subplots(1, 1)

                    if draw_titles:
                        fig.suptitle(
                            "NN search\n# search points = {:,}, # query points = {:,}, K = {}, {}".format(
                                num_search_points,
                                num_query_points,
                                k,
                                THREADING_MAP[threading],
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

                    save_figure(
                        fig_output_dir,
                        Path(
                            "line",
                            f"num_search_{num_search_points}_num_query_{num_query_points}_k_{k}_{THREADING_FILENAME_MAP[threading]}.svg",
                        ),
                    )

                detailed_params.append(
                    {
                        "num_search_points": num_search_points,
                        "num_query_points": num_query_points,
                        "k": k,
                        "threading": threading_set,
                    }
                )

        for threading in threading_set:
            bar_fig, ax = plt.subplots(1, 1)

            if draw_titles:
                bar_fig.suptitle(
                    "NN search\n# query points = {:,}, K = {}, {}".format(
                        num_query_points, k, THREADING_MAP[threading]
                    ),
                    fontsize=13,
                    fontweight="bold",
                )

            subset_df2 = (
                subset_df1[subset_df1.threading == threading]
                .groupby(groupby_params)
                .min()
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
                p = ax.barh(
                    y=np.arange(len(subset_df3)) + bar_offset,
                    width=subset_df3.real_time,
                    height=bar_width,
                    label=name,
                )
                ax.bar_label(p, fmt="%.2f", label_type="edge")

            ax.set_ylabel("# search points")
            ax.set_xlabel("Time (ms)")
            ax.set_yticks(
                ticks=np.arange(len(num_search_points_set)),
                labels=[
                    "{:,}".format(num_search_points)
                    for num_search_points in num_search_points_set
                ],
            )
            ax.invert_yaxis()
            ax.legend()

            save_figure(
                fig_output_dir,
                Path(
                    "bar",
                    f"num_query_{num_query_points}_k_{k}_{THREADING_FILENAME_MAP[threading]}.svg",
                ),
            )

        best_max_leaf_size_params.append(
            {"num_query_points": num_query_points, "k": k, "threading": threading_set}
        )

    plot_params = {"best_max_leaf_size": best_max_leaf_size_params}

    if draw_line_plots:
        plot_params["detailed"] = detailed_params

    return plot_params


if __name__ == "__main__":
    parser = get_default_argparser()
    parser.add_argument(
        "--draw-line-plots",
        action="store_true",
        help="Draw line plots for max leaf size",
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
        draw_titles,
        show,
        fig_output_dir,
        compare,
    ) = get_default_args(args)
    draw_line_plots = args.draw_line_plots
    merge_multithreaded_cuda = args.merge_multithreaded_cuda

    plot_nn_search(
        csv_filepaths,
        setup_names,
        display_scale,
        draw_titles,
        fig_output_dir,
        compare,
        draw_line_plots,
        merge_multithreaded_cuda,
    )

    if show:
        plt.show()
