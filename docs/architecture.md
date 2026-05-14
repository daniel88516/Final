# Architecture

The repository separates project-owned diagnostics code from upstream BusyBox.

- `src/applets/` contains command-facing applet entry points.
- `src/libdiag/` contains reusable filesystem, process, network, and formatting logic.
- `busybox-meta/` contains BusyBox build metadata owned by this project.
- `third_party/busybox/` is reserved for the BusyBox git submodule.
- `scripts/integrate_busybox.sh` copies project-owned files into the BusyBox tree for builds.

The BusyBox submodule should be treated as an external dependency. Generated files copied into `third_party/busybox/diagnostics/` are not the primary source of truth.
