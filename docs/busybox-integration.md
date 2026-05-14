# BusyBox Integration

BusyBox should be added as a submodule at `third_party/busybox`.

```sh
git submodule add https://github.com/mirror/busybox.git third_party/busybox
git submodule update --init --recursive
```

After the submodule exists, run:

```sh
make integrate
```

This copies files from `src/applets/`, `src/libdiag/`, and `busybox-meta/` into `third_party/busybox/diagnostics/`.

If BusyBox does not yet include `diagnostics/Config.src` in its configuration tree, add the minimal BusyBox-side metadata hook as a small integration patch and document it in the PR.
