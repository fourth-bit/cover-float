import argparse
from cover_float._core import run_test_vector


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", type=str, help="Path to the input test vector file")
    parser.add_argument("output_file", type=str, help="Path to the output cover vector file")
    parser.add_argument(
        "--suppress-error-check",
        action="store_true",
        help="Suppress error checking between expected and actual results",
    )
    args = parser.parse_args()

    with open(args.input_file, "r") as infile, open(args.output_file, "w") as outfile:
        for line in infile:
            line = line.strip()
            if not line or line.startswith("//"):
                continue  # Skip empty lines and comments
            result = run_test_vector(line, args.suppress_error_check)
            outfile.write(result)