BUSYBOX_DIR := third_party/busybox

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
	@if [ -d "$(BUSYBOX_DIR)" ]; then \
		$(MAKE) -C "$(BUSYBOX_DIR)" clean; \
	fi
