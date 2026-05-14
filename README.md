# BusyBox Diagnostics Toolkit

This repository contains a small Linux diagnostics toolkit intended to be integrated into BusyBox as custom applets.

Planned applets:

- `bbfscheck`: filesystem usage and health summary
- `bbtop`: process and resource summary
- `bbnetmon`: TCP connection summary

BusyBox is treated as an external dependency under `third_party/busybox`. Project-owned source lives under `src/`, and scripts copy that source into BusyBox when needed.

## Repository Structure

```text
Final/
├── src/
│   ├── applets/          # BusyBox applet entry points
│   └── libdiag/          # shared diagnostics helpers
├── busybox-meta/         # project-owned BusyBox Config/Kbuild snippets
├── third_party/          # external dependencies; BusyBox submodule goes here
├── scripts/              # integration, build, test, and benchmark commands
├── tests/                # smoke, integration, fixtures, and golden files
├── benchmarks/           # benchmark scripts and local results
├── docs/                 # detailed project documentation
└── configs/              # BusyBox config files
```

## Initialize BusyBox

Add BusyBox as a submodule if it is not already present:

```sh
git submodule add https://github.com/mirror/busybox.git third_party/busybox
git submodule update --init --recursive
```

If the submodule already exists in the repository, initialize it with:

```sh
make init-submodule
```

## Integrate Applets

Copy project-owned diagnostics sources into the BusyBox tree:

```sh
make integrate
```

This refreshes `third_party/busybox/diagnostics/` from `src/applets/`, `src/libdiag/`, and `busybox-meta/`.

## Build

```sh
make build
```

The build script enters `third_party/busybox/`, configures BusyBox, builds it, and prints the resulting binary path.

## Test

```sh
make test
```

The current tests include Linux runtime smoke checks and placeholder integration tests for the planned applets.

## Benchmarks

```sh
make bench
```

Local benchmark output is written under `benchmarks/results/local-*`.

## Documentation

See:

- `docs/architecture.md`
- `docs/development-guide.md`
- `docs/busybox-integration.md`
- `docs/benchmark-plan.md`
- `docs/demo-script.md`
