# AGENTS.md

## Project Context

This repository is a UNIX / BusyBox final project.

The goal of this project is to build a small diagnostics toolkit that can be integrated into BusyBox as custom applets. The planned applets include:

- `bbfscheck`: filesystem usage and health checker
- `bbtop`: process/resource inspection tool
- `bbnetmon`: TCP connection monitoring tool

The repository is currently being reorganized on the branch:

```text
repo-style
```

The purpose of this branch is to clean up the repository structure before opening a Pull Request back to the main development branch.

---

## High-Level Goal

Refactor the repository layout so that:

1. BusyBox source code is managed as a `git submodule`.
2. Our own source code is clearly separated from upstream BusyBox code.
3. Integration into BusyBox is handled by scripts.
4. Tests, benchmarks, documentation, and demo materials are organized in dedicated directories.
5. The repository becomes easier for multiple team members to develop, review, and maintain.

---

## Important Design Decision

BusyBox should be treated as an external dependency, not as the main development directory.

Do not make the project structure depend on manually editing files directly inside the BusyBox source tree.

Instead, keep our own code in `src/`, then copy or sync the necessary files into the BusyBox source tree through an integration script.

---

## Target Repository Structure

The final repository should be organized roughly as follows:

```text
Final/
├── README.md
├── AGENTS.md
├── Makefile
├── .gitignore
│
├── src/
│   ├── applets/
│   │   ├── bbfscheck.c
│   │   ├── bbtop.c
│   │   └── bbnetmon.c
│   │
│   └── libdiag/
│       ├── fs_reader.c
│       ├── fs_reader.h
│       ├── proc_reader.c
│       ├── proc_reader.h
│       ├── net_reader.c
│       ├── net_reader.h
│       ├── formatter.c
│       ├── formatter.h
│       └── diag_common.h
│
├── busybox-meta/
│   ├── Config.src
│   ├── Kbuild.src
│   └── README.md
│
├── third_party/
│   └── busybox/              # git submodule
│
├── scripts/
│   ├── integrate_busybox.sh
│   ├── build_busybox.sh
│   ├── run_tests.sh
│   └── benchmark.sh
│
├── tests/
│   ├── smoke/
│   ├── integration/
│   ├── fixtures/
│   └── golden/
│
├── benchmarks/
│   ├── scripts/
│   ├── results/
│   └── README.md
│
├── docs/
│   ├── architecture.md
│   ├── development-guide.md
│   ├── busybox-integration.md
│   ├── benchmark-plan.md
│   ├── demo-script.md
│   └── report/
│
└── configs/
    └── busybox_diag_defconfig
```

This structure does not need to be implemented perfectly in one commit, but the refactor should move the repository toward this layout.

---

## BusyBox Submodule Policy

BusyBox source code should be added as a submodule under:

```text
third_party/busybox
```

Recommended command:

```bash
git submodule add https://github.com/mirror/busybox.git third_party/busybox
```

After adding the submodule, the repository should contain a `.gitmodules` file.

Do not vendor or copy the entire BusyBox source tree directly into this repository unless there is a strong reason.

If an existing `busybox/` directory already exists and contains copied upstream BusyBox code, migrate away from that layout.

Suggested migration:

1. Move our custom code from `busybox/diagnostics/` into `src/` and `busybox-meta/`.
2. Remove the copied BusyBox source tree from the repository.
3. Add BusyBox back as a submodule under `third_party/busybox/`.
4. Update documentation and scripts to use the new path.

---

## Source Code Organization Rules

### Applets

Custom BusyBox applets should live in:

```text
src/applets/
```

Each applet should focus on command-line behavior and user-facing output.

Examples:

```text
src/applets/bbfscheck.c
src/applets/bbtop.c
src/applets/bbnetmon.c
```

Applet files may contain BusyBox applet metadata comments such as:

```c
//applet:IF_BBFSCHECK(APPLET(bbfscheck, BB_DIR_USR_BIN, BB_SUID_DROP))
```

However, do not hard-code assumptions that only work inside the BusyBox source tree unless necessary.

---

### Shared Diagnostics Library

Shared logic should live in:

```text
src/libdiag/
```

The original monolithic `libdiag.c` should be split into smaller modules when possible:

```text
fs_reader.c / fs_reader.h       # statvfs, mount, filesystem-related logic
proc_reader.c / proc_reader.h   # /proc process and CPU parsing
net_reader.c / net_reader.h     # /proc/net/tcp, tcp6 parsing
formatter.c / formatter.h       # table and human-readable output helpers
diag_common.h                   # shared structs, constants, common helpers
```

Avoid putting unrelated logic into one large shared file.

---

## BusyBox Metadata

BusyBox build metadata should be kept outside the submodule in:

```text
busybox-meta/
```

This directory should contain files such as:

```text
busybox-meta/Config.src
busybox-meta/Kbuild.src
```

The integration script should copy these files into the correct BusyBox directory during integration.

For example, after running integration, the generated BusyBox-side layout may become:

```text
third_party/busybox/diagnostics/
├── bbfscheck.c
├── bbtop.c
├── bbnetmon.c
├── fs_reader.c
├── proc_reader.c
├── net_reader.c
├── formatter.c
├── diag_common.h
├── Config.src
└── Kbuild.src
```

The generated files inside the BusyBox submodule should not be treated as the primary source of truth.

The primary source of truth is always:

```text
src/
busybox-meta/
```

---

## Scripts

All repeatable development actions should be placed under:

```text
scripts/
```

At minimum, provide or prepare space for:

```text
scripts/integrate_busybox.sh
scripts/build_busybox.sh
scripts/run_tests.sh
scripts/benchmark.sh
```

### `scripts/integrate_busybox.sh`

This script should:

1. Check that `third_party/busybox/` exists.
2. Create `third_party/busybox/diagnostics/` if needed.
3. Copy files from `src/applets/` into `third_party/busybox/diagnostics/`.
4. Copy files from `src/libdiag/` into `third_party/busybox/diagnostics/`.
5. Copy `busybox-meta/Config.src` and `busybox-meta/Kbuild.src`.
6. Patch or update BusyBox build metadata only if necessary.
7. Print clear status messages.
8. Fail fast on errors.

Use POSIX-compatible shell where practical.

The script should be safe to run multiple times.

### `scripts/build_busybox.sh`

This script should:

1. Enter `third_party/busybox/`.
2. Run the required BusyBox configuration step.
3. Build BusyBox.
4. Print the final BusyBox binary path and size.

### `scripts/run_tests.sh`

This script should run available smoke and integration tests.

### `scripts/benchmark.sh`

This script should run benchmark cases and store results under:

```text
benchmarks/results/
```

---

## Makefile

Add a top-level `Makefile` if it does not already exist.

Recommended targets:

```makefile
.PHONY: init-submodule integrate build test bench clean

init-submodule:
	git submodule update --init --recursive

integrate:
	./scripts/integrate_busybox.sh

build:
	./scripts/build_busybox.sh

test:
	./scripts/run_tests.sh

bench:
	./scripts/benchmark.sh

clean:
	$(MAKE) -C third_party/busybox clean
```

The exact commands may be adjusted based on the actual BusyBox build flow.

---

## Tests

Move tests into:

```text
tests/
```

Suggested layout:

```text
tests/
├── smoke/
│   └── test_runtime_env.sh
│
├── integration/
│   ├── test_bbfscheck.sh
│   ├── test_bbtop.sh
│   └── test_bbnetmon.sh
│
├── fixtures/
│   ├── proc_net_tcp.sample
│   ├── proc_pid_stat.sample
│   └── mountinfo.sample
│
└── golden/
    ├── bbfscheck_summary.expected
    └── bbnetmon_summary.expected
```

The existing environment-check script can be moved to:

```text
tests/smoke/test_runtime_env.sh
```

Smoke tests should check whether required Linux interfaces are available, such as:

```text
/proc/stat
/proc/1/stat
/proc/1/status
/proc/net/tcp
/proc/net/tcp6
```

Integration tests should execute the actual applets after BusyBox is built.

---

## Benchmarks

Benchmark-related files should live under:

```text
benchmarks/
```

Suggested layout:

```text
benchmarks/
├── README.md
├── scripts/
│   ├── bench_bbfscheck.sh
│   ├── bench_bbtop.sh
│   └── bench_bbnetmon.sh
└── results/
```

Benchmark design should compare our tools with common Linux tools where appropriate:

```text
bbfscheck  vs df / find / stat
bbtop      vs ps / top
bbnetmon   vs ss / netstat
```

Useful benchmark metrics:

- execution time
- memory usage
- binary size before and after BusyBox integration
- output correctness
- behavior inside container or Kubernetes-like environments

Do not commit large benchmark output files unless they are small, meaningful, and used in the final report.

---

## Documentation

Documentation should live under:

```text
docs/
```

Recommended files:

```text
docs/architecture.md
docs/development-guide.md
docs/busybox-integration.md
docs/benchmark-plan.md
docs/demo-script.md
docs/report/
```

### README.md

The top-level README should be concise and should include:

1. Project overview
2. Features
3. Repository structure
4. How to initialize the BusyBox submodule
5. How to integrate custom applets
6. How to build
7. How to run tests
8. How to run benchmarks
9. Links to detailed docs

Do not put the entire final report into README.md.

---

## `.gitignore`

Update `.gitignore` for a C / BusyBox project.

Recommended entries:

```gitignore
# Build outputs
*.o
*.a
*.so
*.d
*.cmd
*.su
*.dSYM/
build/
out/
dist/

# BusyBox generated files
third_party/busybox/.config
third_party/busybox/.config.old
third_party/busybox/busybox
third_party/busybox/applets/applet_tables
third_party/busybox/include/applet_tables.h
third_party/busybox/include/usage_compressed.h

# Local benchmark results
benchmarks/results/local-*/

# Editor / OS files
.vscode/
.idea/
*.swp
*.swo
*~
.DS_Store
Thumbs.db
```

Do not ignore `.gitmodules`.

Do not ignore project source files under `src/`.

---

## Branch and PR Workflow

This restructuring work is being done on:

```text
repo-style
```

Expected workflow:

```bash
git checkout repo-style
# reorganize files
git status
git add .
git commit -m "refactor(repo): reorganize project structure"
git push origin repo-style
```

Then open a Pull Request back to the main development branch.

The PR should clearly explain:

1. Why BusyBox was moved to a submodule.
2. Why project-owned code was moved to `src/`.
3. How to initialize the submodule.
4. How to run integration and build scripts.
5. Whether any functional code changed or only file layout changed.

---

## Commit Style

Use clear commit messages.

Examples:

```text
refactor(repo): separate project source from BusyBox source
build: add BusyBox submodule integration script
docs: add development guide for repository layout
test: move runtime environment check to tests/smoke
bench: add benchmark directory structure
```

Prefer small commits if possible:

1. Move source files.
2. Add submodule.
3. Add scripts.
4. Update docs.
5. Update tests.

Do not mix large functional rewrites with repository restructuring unless necessary.

---

## Safety Rules

When modifying this repository:

1. Do not delete project-owned code unless it has been moved to the new structure.
2. Do not directly edit upstream BusyBox files unless the integration process requires a small patch.
3. Do not commit generated object files, compiled binaries, or large benchmark outputs.
4. Do not break existing applet metadata comments.
5. Do not assume the user wants a complete rewrite of applet logic.
6. Focus this branch on repository structure, build flow, and documentation.
7. Keep the changes reviewable.

---

## Expected Final State

After the refactor, a new developer should be able to run:

```bash
git clone --recurse-submodules https://github.com/daniel88516/Final.git
cd Final
make integrate
make build
make test
```

Or, if the repository was cloned without submodules:

```bash
git submodule update --init --recursive
make integrate
make build
make test
```

The repository should make it clear which files belong to the project and which files belong to upstream BusyBox.

---

## Notes for Coding Agents

When performing the restructuring:

- Inspect the current repository before changing files.
- Preserve existing code and comments.
- Prefer moving files over rewriting them.
- Update relative include paths after moving files.
- Update scripts and documentation to match the new paths.
- Keep the branch focused on structure and developer workflow.
- If a build or test cannot be completed, document the reason clearly in the PR description.

The main objective is not to finish every applet feature.

The main objective is to make the repository clean, maintainable, and ready for collaborative BusyBox applet development.
