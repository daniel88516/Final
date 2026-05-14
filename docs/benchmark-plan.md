# Benchmark Plan

Track execution time, memory usage, output correctness, and BusyBox binary size.

Initial comparisons:

- `bbfscheck` against `df`, `find`, and `stat`
- `bbtop` against `ps` and `top`
- `bbnetmon` against `ss` and `netstat`

Prefer small, reproducible benchmark cases. Store local run output under `benchmarks/results/local-*`.
