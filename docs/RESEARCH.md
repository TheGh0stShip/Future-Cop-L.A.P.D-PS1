# Research log

Evidence is recorded with exact revisions so later work can distinguish facts
from inherited assumptions.

## Same-title projects checked on 2026-10-21

- `alexbeavs-ps1-ports/future-cop-lapd-recomp`, revision
  `fe6e0cd39d55dc413ae77444fc65512c1ce22ffe`, is an incomplete static
  recompilation configured for European `SLES-01449`. Its published status is
  a 0.1.0 release candidate and does not claim full-game completion, audio
  fidelity, or multiplayer verification. It provided the initial PSXRecomp
  configuration shape, but no generated code counts toward matching C.
- `Ghoster738/Future-Cop-MIT`, revision
  `3d77b21c2032baa2a0d536f04c1937c7d0e4a51d`, is an incomplete engine
  reimplementation. It reports no gameplay, but includes mission, map, model,
  animation, texture, audio, and video readers for multiple game platforms.
  Its PS1 mission parser recognizes chunked IFF resources including `CTRL`,
  `SHOC`, `SWVR`, `MISC`, `VAGM`, `VAGB`, and MDEC-backed resources. This is
  useful format evidence for later data typing, not executable source.

Sources:

- <https://github.com/alexbeavs-ps1-ports/future-cop-lapd-recomp>
- <https://github.com/Ghoster738/Future-Cop-MIT>
- <https://forum.mattkc.com/viewtopic.php?p=2504>

## USA retail evidence

- Disc track: 351,718,080 bytes; MD5
  `57712fcfb39a6528672356bc53e2b0a9`; SHA-1
  `e27204e4415a6eb8179fbb0c6294149e829d507e`; CRC32 `dd21d336`.
- ISO volume: `SLUS-00739`; 72 filesystem entries.
- Boot executable: `SLUS_007.39`, 589,824 bytes; SHA-256
  `892f8482d3c9b163f45afae50676808e5673bdb7d6c079663cc1fdf6826b236f`.
- Loaded image: `0x80010000..0x8009f800`; entry `0x80096520`; initial stack
  `0x801ffff0`; header declares no separate data or BSS range.
- Static analysis currently reports 1,887 primary function candidates, 93
  recovered jump tables, 189 unresolved indirect transfers, and 94.5% code
  coverage. Splat independently finds 1,488 provisional direct-call starts.
  Neither count is authoritative until boundaries are reviewed.
- Embedded Sony revision strings include `bios.c` 1.86 (1997-03-28), `intr.c`
  1.76 (1997-02-12), and `sys.c` 1.135 (1997-09-02). These identify source
  member revisions, not by themselves a complete PsyQ SDK release.

## Compiler hypothesis

GCC 2.8.1 PSX with `-O2 -G0` is the leading hypothesis, based on the matching
pipeline proven in the read-only BattleTanx: Global Assault PS1 project and the
first fifty exact Future Cop functions. GCC 2.7.2 PSX remains pinned for
per-unit testing. These trivial matches do not establish the compiler for game-owned
non-leaf functions; stack layout, register allocation, switch lowering,
division, GTE scheduling, and small-data behavior still require representative
matches.
