# CircularController Makefile
#
# Targets:
#   make all   — build all projects
#   make build — build CircularController (moteino) + Bringup (nano)

PIO ?= pio

.PHONY: all build

all: build

build:
	@echo "=== Building CircularController (moteino) ==="
	cd CircularController && $(PIO) run -e moteino
	@echo "=== Building Bringup (nano) ==="
	cd Bringup && $(PIO) run -e nano
