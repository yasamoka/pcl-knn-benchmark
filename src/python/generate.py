from argparse import ArgumentParser
import json
from pathlib import Path

from jinja2 import Environment, FileSystemLoader, select_autoescape

from plots.build_tree import plot_build_tree
from plots.nn_search import plot_nn_search


parser = ArgumentParser(description="Generate benchmark document with plots")
parser.add_argument("-n", "--name", type=str, help="Comparison name")
parser.add_argument("-s", "--setup", action="append", help="Setup name", required=True)
parser.add_argument(
    "-c", "--config", action="append", help="Configuration filepath", required=True
)
parser.add_argument(
    "-b",
    "--benchmark",
    action="append",
    help="Benchmark results directory",
    required=True,
)
parser.add_argument("-o", "--output", type=str, help="Output directory", required=True)

args = parser.parse_args()
comparison_name = args.name
setup_names = args.setup
config_filepaths = args.config
benchmark_results_dirs = args.benchmark
output_dir = args.output

assert len(setup_names) == len(config_filepaths) == len(benchmark_results_dirs)
compare = len(setup_names) > 1
if compare:
    assert comparison_name

configs = []
for config_filepath in config_filepaths:
    with open(config_filepath) as config_file:
        config = json.load(config_file)
        configs.append(config)

extra_args = " ".join(
    [
        f'-s "{setup_name}" -c {config_filepath} -b {benchmark_results_dir}'
        for setup_name, config_filepath, benchmark_results_dir in zip(
            setup_names, config_filepaths, benchmark_results_dirs
        )
    ]
)

name_args = f' -n "{comparison_name}"' if compare else ""

page_generation_command = (
    f"PYTHONPATH=. python generate.py{name_args} {extra_args} -o {output_dir}"
)

build_tree_csv_filepaths = [
    Path(benchmark_results_dir, "build_tree.csv")
    for benchmark_results_dir in benchmark_results_dirs
]
fig_output_dir = Path(output_dir, "plots/build-tree")
build_tree_params = plot_build_tree(
    csv_filepaths=build_tree_csv_filepaths,
    setup_names=setup_names,
    fig_output_dir=fig_output_dir,
    compare=compare,
    shorten_names=True,
)

nn_search_csv_filepaths = [
    Path(benchmark_results_dir, "nn_search.csv")
    for benchmark_results_dir in benchmark_results_dirs
]
fig_output_dir = Path(output_dir, "plots/nn-search")
nn_search_params = plot_nn_search(
    csv_filepaths=nn_search_csv_filepaths,
    setup_names=setup_names,
    fig_output_dir=fig_output_dir,
    compare=compare,
    draw_line_plots=not compare,
    merge_multithreaded_cuda=compare,
    shorten_names=True,
)

plot_params = {"build_tree": build_tree_params, "nn_search": nn_search_params}

env = Environment(
    loader=FileSystemLoader("."),
    autoescape=select_autoescape(),
    trim_blocks=True,
    lstrip_blocks=True,
)
env.filters["zip"] = zip
env.filters["thousands"] = lambda x: "{:,}".format(x)

template = env.get_template("generate.j2")
output = template.render(
    comparison_name=comparison_name,
    setup_names=setup_names,
    configs=configs,
    page_generation_command=page_generation_command,
    build_tree_csv_filepaths=build_tree_csv_filepaths,
    nn_search_csv_filepaths=nn_search_csv_filepaths,
    plot_params=plot_params,
)

with open(Path(output_dir, "index.md"), "w") as output_file:
    output_file.write(output)
