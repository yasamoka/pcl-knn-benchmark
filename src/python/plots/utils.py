from argparse import ArgumentParser
import os
from pathlib import Path
from typing import Callable, Dict, List, Optional, Tuple

import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def set_display_scale(scale: int) -> None:
    mpl.rcParams["figure.dpi"] = 96 * scale


def get_default_argparser() -> ArgumentParser:
    parser = ArgumentParser()
    parser.add_argument(
        "-b",
        "--benchmark",
        action="append",
        help="Benchmark results filepath",
        required=True,
    )
    parser.add_argument(
        "-s", "--setup", action="append", help="Setup name", required=False
    )
    parser.add_argument("--display-scale", type=float, help="Display scale", default=1)
    parser.add_argument(
        "--do-not-draw-titles", action="store_false", help="Do not draw titles"
    )
    parser.add_argument(
        "--do-not-show", action="store_false", help="Do not show figures"
    )
    parser.add_argument(
        "-o", "--output", help="Output directory to save figures", default=None
    )
    return parser


def get_default_args(args) -> Tuple[str, str]:
    csv_filepaths = [Path(filepath) for filepath in args.benchmark]
    setup_names = args.setup
    display_scale = args.display_scale
    draw_titles = args.do_not_draw_titles
    show = args.do_not_show
    fig_output_dir = Path(args.output) if args.output is not None else None

    compare = not (len(csv_filepaths) == 1 and setup_names is None)

    if compare:
        assert len(csv_filepaths) == len(setup_names)
    else:
        setup_names = [""]

    return (
        csv_filepaths,
        setup_names,
        display_scale,
        draw_titles,
        show,
        fig_output_dir,
        compare,
    )


def merge_dfs(csv_filepaths: List[Path], setup_names: List[str]) -> pd.DataFrame:
    csv_dfs = []
    for csv_filepath, setup_name in zip(csv_filepaths, setup_names):
        csv_df = pd.read_csv(csv_filepath)
        csv_df["setup_name"] = setup_name
        csv_dfs.append(csv_df)

    df = pd.concat(csv_dfs)
    df.reset_index(drop=True, inplace=True)
    return df


def assert_column_single_valued(column: pd.Series):
    assert (column == column[0]).all()


def run_default_assertions(df: pd.DataFrame):
    for column_name in ("iterations", "time_unit"):
        assert_column_single_valued(df[column_name])


def run_default_cleanup(df: pd.DataFrame):
    df.drop(
        [
            "iterations",
            "cpu_time",
            "time_unit",
            "bytes_per_second",
            "items_per_second",
            "label",
            "error_occurred",
            "error_message",
        ],
        axis="columns",
        inplace=True,
    )


def generate_bar_offsets(bar_width: float, num_bars: int):
    bar_half_offset = bar_width / 2
    extremum = bar_half_offset * num_bars
    return np.arange(
        start=-extremum + bar_half_offset,
        stop=extremum + bar_half_offset,
        step=bar_width,
    )


def label(
    ax: mpl.axis.Axis,
    df: pd.DataFrame,
    column_formatter_map: Dict[str, Callable],
    axis_label_map: Dict[str, str],
):
    useful_column_names = []
    redundant_column_names = []

    for column_name in column_formatter_map:
        if (df[column_name] == df[column_name][0]).all():
            redundant_column_names.append(column_name)
        else:
            useful_column_names.append(column_name)

    if len(useful_column_names) == 1:
        column_name = useful_column_names[0]
        labels = df[column_name]
        ax.set_xlabel(axis_label_map[column_name])
    else:
        labels_lines = [
            [
                column_formatter_map[column_name](row[column_name])
                for column_name in useful_column_names
            ]
            for _, row in df.iterrows()
        ]

        labels = ["\n".join(label_lines) for label_lines in labels_lines]

    ax.set_xticks(ticks=np.arange(len(labels)), labels=labels)

    title_lines = [
        column_formatter_map[column_name](df[column_name][0])
        for column_name in redundant_column_names
    ]

    ax.set_title(", ".join(title_lines))


def save_figure(output_dir: Optional[Path], relative_filepath: Path) -> None:
    if output_dir:
        fig_filepath = Path(output_dir, relative_filepath)
        os.makedirs(fig_filepath.parent, exist_ok=True)
        plt.savefig(fig_filepath, bbox_inches="tight", format="svg")
