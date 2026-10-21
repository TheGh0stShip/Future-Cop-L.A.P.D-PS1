# Offline decompilation worker

`tools/offline_decomp.py` runs resumable, compiler-guided C reconstruction jobs
using local compute. It selects small unmatched functions, sends one function at
a time to a local proposal command, compiles each response with the pinned PS1
toolchain, scores it against `SLUS_007.39`, and feeds the best result into the
next attempt.

The worker writes only beneath the ignored `build/us/offline-decomp/` directory.
It does not edit tracked source, modify matching manifests, update progress,
invoke Git, or publish results. An item appears in `inbox/` only after its entire
declared function range matches the verified executable byte-for-byte. Review is
still required before copying it into `src/game/` and the matching manifest.

## Prerequisites

Prepare the normal project inputs once:

```sh
make setup toolchain split function-inventory
```

The proposal command must be an executable available locally. For every attempt
the worker writes one JSON request to its standard input and expects one JSON
response on standard output:

```json
{
  "source": "int func_80000000(void) { return 0; }\n",
  "symbols": {"D_800A0000": "0x800A0000"},
  "note": "short optional rationale"
}
```

The request includes the target address and size, generated assembly oracle,
known address-named symbols, compiler settings, prior best C source, and its
byte/word score and mismatch table. Diagnostic text from a failed compilation
is returned with the next request. Repeated source is skipped, compiled output
is fingerprinted, and state is replaced atomically after every attempt. The
command must emit reconstructed C; assembly-bearing source is rejected and does
not count as decompilation.

`tools/local_proposer.py` is a ready-made adapter for local servers exposing a
chat-completions JSON endpoint. It accepts only an unencrypted loopback URL, so
the default configuration cannot send game material off the machine. Start the
local server separately, then use the adapter as the proposal command:

```sh
python3 tools/offline_decomp.py run \
  --backend-command 'python3 tools/local_proposer.py --model local' \
  --jobs 4 --targets 16 --attempts 100
```

The default endpoint is `http://127.0.0.1:8080/v1/chat/completions`. Override it
with `--url` when the local service uses a different loopback port or path.

## Run and resume

Start with a conservative batch:

```sh
python3 tools/offline_decomp.py run \
  --backend-command '/absolute/path/to/local-proposer --profile ps1' \
  --jobs 4 --targets 16 --attempts 100 --max-size 0x80
```

`--attempts` is a cumulative ceiling. Running the same command again resumes
from each target's `state.json`; raise the ceiling to continue. Restrict a run
to named functions with repeatable `--function func_80012345` arguments.

Inspect saved progress without invoking the compiler or proposal command:

```sh
python3 tools/offline_decomp.py status
```

Exact results are placed in:

```text
build/us/offline-decomp/inbox/<function>/candidate.c
build/us/offline-decomp/inbox/<function>/unit.json
```

Before promotion, inspect the C for accurate types, intent, boundaries, and
maintainability, then add it to `config/us/matching_units.json` and run the full
project verification milestone. The inbox proof is deliberately not a tracker
update.

The campaign structure follows lessons from the project's read-only reference
set: resumable shortlists and bounded searches, concurrent cached scoring,
strict separation between research evidence and credited source, and semantic
review after byte equality. No source from those projects is imported by this
worker.

## Parallel-use rules

Use a separate `--output` directory for each concurrently launched controller.
One controller already supports multiple `--jobs`; two controllers must never
share state. Generated workspaces contain retail-derived oracle text and remain
under ignored `build/` paths.

The controller enforces that `--output` remains beneath this repository's
ignored `build/` directory. Candidate sources should include shared headers as
`"game/types.h"` or `"game/command_state.h"`; the pinned preprocessing lane
supplies the project include directory.
