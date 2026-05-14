# Development Guide

Start from the repository root.

```sh
git submodule update --init --recursive
make integrate
make build
make test
```

Edit diagnostics applet behavior under `src/applets/` and shared logic under `src/libdiag/`.
