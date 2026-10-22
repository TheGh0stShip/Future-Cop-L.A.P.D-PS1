# Future Cop: L.A.P.D. (USA) matching decompilation

[![Code](https://img.shields.io/badge/code-24912%2F555552-red)](https://bytematch.dev/TheGh0stShip/Future-Cop-L.A.P.D-PS1)
[![Functions](https://img.shields.io/badge/functions-319%2F1869-red)](https://bytematch.dev/TheGh0stShip/Future-Cop-L.A.P.D-PS1)
[![Verify](https://github.com/TheGh0stShip/Future-Cop-L.A.P.D-PS1/actions/workflows/verify.yml/badge.svg)](https://github.com/TheGh0stShip/Future-Cop-L.A.P.D-PS1/actions/workflows/verify.yml)

An in-progress, byte-matching C reconstruction of the North American
PlayStation release (`SLUS-00739`). The target is a user-supplied
`Future Cop - L.A.P.D. (USA).cue` / `.bin`; no retail game data belongs in the
repository.

## Current state

The current scaffold verifies the exact USA executable, extracts the disc,
splits 100% of the executable with Splat, inventories provisional function
starts, pins the PSX compiler toolchain, and compares reviewed C units directly
to retail bytes. 319 functions totaling 24,912 bytes currently match. Provisional assembly,
generated static-recompiler output, and boundary guesses do not count as
matching C.

```sh
make function-inventory
make verify-matching
make report
make check
```

The Makefile expects the image in `Future Cop - L.A.P.D. (USA)/`. Retail inputs,
extracted files, assembly, toolchains, and build output are ignored. Matching
source lives under `src/`; its address, size, compiler profile, and symbols are
declared in `config/us/matching_units.json`.

`make report` writes an objdiff v2 progress report to `build/report.json`.
The default-branch workflow publishes it as `SLUS-00739_report` for
[ByteMatch](https://bytematch.dev/TheGh0stShip/Future-Cop-L.A.P.D-PS1) and
compatible public progress trackers. The current denominator is the audited
static-analysis catalogue: 1,880 non-data, non-alias function ranges covering
555,552 bytes. Only units that pass the retail-byte verifier receive progress.

For unattended local candidate searches, see the
[offline decompilation worker](docs/OFFLINE_DECOMP.md). It runs resumable,
parallel compiler-guided campaigns against a local proposal command, keeps all
generated work under ignored `build/` paths, and places only exact-byte results
in a review inbox. It never edits tracked source or updates progress itself.

## Prior art gate

Public research found two relevant projects as of 2026-10-21:

- `alexbeavs-ps1-ports/future-cop-lapd-recomp`, a European SLES-01449 static
  recompilation at version 0.1.0 RC. Its own status does not claim full-game,
  audio, or multiplayer verification.
- `Ghoster738/Future-Cop-MIT`, an incomplete engine reimplementation with asset,
  model, map, and mission readers but no gameplay.

The European recomp is useful structural prior art but is not byte-identical to
this USA target. Its generated output is not matching C and contributes no
bytes to the matching total.
