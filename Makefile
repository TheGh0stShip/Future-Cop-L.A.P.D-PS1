BIN := Future\ Cop\ -\ L.A.P.D.\ (USA)/Future\ Cop\ -\ L.A.P.D.\ (USA).bin
ISO := build/disc/future-cop-usa.iso
FILES := build/disc/files
PYTHON ?= python3
VENV := .venv
VENV_PYTHON := $(VENV)/bin/python
VENV_STAMP := $(VENV)/.installed
EXE := baseroms/us/SLUS_007.39

.PHONY: all setup toolchain verify split function-inventory verify-matching report check extract inventory clean

all: split

$(VENV_STAMP): requirements.txt
	$(PYTHON) -m venv $(VENV)
	$(VENV_PYTHON) -m pip install --upgrade pip
	$(VENV_PYTHON) -m pip install -r requirements.txt
	@touch "$@"

$(EXE): $(FILES)/.extracted
	@mkdir -p "$(dir $(EXE))"
	@chmod u+w "$@" 2>/dev/null || true
	install -m 0644 "$(FILES)/SLUS_007.39" "$@"

setup: $(VENV_STAMP) $(EXE)

toolchain:
	tools/bootstrap_toolchain.sh

verify: $(EXE)
	$(PYTHON) tools/verify_exe.py "$(EXE)"

split: setup verify
	$(VENV_PYTHON) -m splat split config/us/splat.yaml

function-inventory: split
	$(PYTHON) tools/inventory_functions.py asm/us/main.s build/us/function_candidates.json

verify-matching: verify toolchain
	PATH="$(CURDIR)/.toolchain/mipsel-binutils/root/usr/bin:$$PATH" \
	LD_LIBRARY_PATH="$(CURDIR)/.toolchain/mipsel-binutils/root/usr/lib/x86_64-linux-gnu$${LD_LIBRARY_PATH:+:$$LD_LIBRARY_PATH}" \
	$(PYTHON) tools/verify_matching_units.py

report:
	$(PYTHON) tools/generate_decomp_report.py

check: verify function-inventory verify-matching report
	$(PYTHON) -m unittest discover -s tests -t . -v

extract: $(FILES)/.extracted

$(ISO): tools/extract_mode2.py | $(BIN)
	@mkdir -p build/disc
	python3 tools/extract_mode2.py "$<" "$@"

$(FILES)/.extracted: $(ISO)
	@mkdir -p "$(FILES)"
	@chmod -R u+w "$(FILES)" 2>/dev/null || true
	xorriso -osirrox on -indev "$(ISO)" -extract / "$(abspath $(FILES))"
	@touch "$@"

inventory: extract
	@mkdir -p build/inventory
	find "$(FILES)" -type f ! -name .extracted -printf '%P\t%s\n' | sort > build/inventory/files.tsv
	find "$(FILES)" -type f ! -name .extracted -print0 | sort -z | xargs -0 sha256sum > build/inventory/sha256.txt
	python3 tools/inspect_psx_exe.py "$(FILES)/SLUS_007.39" > build/inventory/psx-exe.txt

clean:
	rm -rf build
