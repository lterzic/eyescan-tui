# Eyescan Terminal UI
Low resource terminal visualization tool for eye diagrams.

## Build
Requires CMake version 3.22 or higher.

From project folder run:
```
cmake -S . -B build
cmake --build build
```

## Usage
Tool accepts the following arguments (no built-in help menu yet):
- `--dir` One or more paths to directories containing scan files.
- `--files` One or more paths to specific scan files.
- `--slr` SLR ID to display - default 0.
- `--site` Site ID to display - default 0.
- `--cols` Number of columns to display - default 2.

### Example
Once built, to run the tool on the scans in this repository, run:
```
./build/eyescan-tui --cols 4 --dir ./scans/csv --site 1
```
