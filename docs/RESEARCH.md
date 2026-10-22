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
- <https://github.com/decompals/old-gcc>
- <https://github.com/Fuuzetsu/tenchu-decomp/blob/master/docs/toolchain.md>

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

GCC 2.7.2 PSX with `-O2` is the leading game-code hypothesis, based on the
matching pipeline proven in the read-only BattleTanx: Global Assault PS1 project
and exact Future Cop units. The callback wrappers at `0x80091480`,
`0x800914B0`, `0x800914E0`,
`0x80091510` and `0x80091574`, plus the two-call loop at `0x8001EB6C`, match
only after selecting the pinned 2.7.2 compiler profile. The C behavior was
unchanged; the decisive differences were scheduling and epilogue placement.
Compiler selection therefore remains per-unit evidence rather than a global
assumption. Switch lowering, division, GTE scheduling, and small-data behavior
still require representative matches.

The compiler bank also includes reproducible GCC 2.8.0 PSX from the pinned
`decompals/old-gcc` release. PsyQ 4.3's original `CC1PSX.EXE` identifies itself
as `2.8.0 SN32 Build 4.0.0007`, and contemporary EA decompilation research
independently identifies that release as an EA compiler. A full sweep across
the current 129 candidates found no exact unit and no output improvement over
2.8.1, so 2.8.0 remains available for later original-object attribution but is
not selected by any current matching unit.

A separate full-candidate sweep identified Cygnus GCC 2.7.2-970404 as a
distinct and necessary lane. It is distributed as `gcc-2.7.2-cdk.tar.gz` in
the pinned `decompals/old-gcc` release and is not interchangeable with the
stock GCC 2.7.2 PSX build. Rebuilding every outstanding candidate with this
lane produced 45 new byte-identical functions totaling 5,756 bytes. These
matches include earlier one- and two-instruction near misses, confirming that
their register allocation, shared returns, and scheduling differences came
from compiler provenance rather than incorrect game logic. The bootstrap and
compiler-sweep tools pin and test this lane directly.

An audited reconstruction sweep of the `0x80040000..0x8005FFFF` and
`0x80070000..0x8008FFFF` regions produced 116 additional matching C functions
totaling 10,200 bytes. Every unit was rebuilt with the pinned compiler and
assembler lanes and compared byte-for-byte with the USA executable before it
was added to the matching manifest. Address-bearing references are resolved at
link time so `%hi`/`%lo` carry behavior matches retail, including small-data
units compiled with `-G4` and the recovered `$gp` value `0x8009F214`.

## Cross-project executable comparison

`tools/cross_corpus_matches.py` compares audited Future Cop function ranges
against another user-supplied PS1 code image. Exact hits are reported
separately from a conservative near-match lane that masks only the 26-bit
destination fields of MIPS `J` and `JAL` instructions.

An initial read-only scan against the BattleTanx: Global Assault PS1 code image
found 29 exact occurrences representing 23 distinct Future Cop functions of at
least 24 bytes, plus 185 jump-normalized occurrences. Thirteen of the exact
functions were not yet present in the matching manifest. The largest exact
shared sequences were 260 and 128 bytes. These are candidate correspondences,
not matching-decompilation credit: a function counts only after independently
written C passes the USA retail-byte verifier.

The 56-byte shared routine at `0x8008B05C` was identified from PsyQ library
symbol corpora as `_padGetActSize`. Its recovered C computes the controller
command-buffer extent from fields at offsets `0xE3`, `0xE9`, and `0xEC`; the
result passes the full retail-byte verifier.

Read-only comparison with `hansbonini/psx_tomba` revision
`fb3b36715f6966c099be95564c155bd842003b76` independently confirmed the
LIBPAD 4.2 object ownership, signed temporary types, and the complete
`0xF0`-byte controller-port layout. That repository was used as reference
evidence only; no files were copied into this tree.

The same SDK source-shape comparison resolved the 260-byte routine at
`0x8008E61C` as `CdIntToPos`. GCC emits the retail register allocation only
when the binary-coded-decimal conversion is expressed as the original nested
inline helper; a macro or cached `/75` intermediate produces different code.

Jump-destination-normalized comparison then identified the adjacent LIBPAD 4.2
load-information routines at `0x8008AE8C` and `0x8008AF10`. Their reconstructed
C contributes another 464 exact bytes. The dispatcher selects command emitters
from `sequence_step`; the response decoder fills mode and actuator metadata,
tracks combination storage, and rejects command buffers larger than 128 bytes.
The associated actuator-information dispatcher at `0x8008B168` and six-byte
actuator-map builder at `0x8008B508` add a further 368 exact bytes under the
2.7.2 compiler lane.

## PsyQ object signatures

`tools/scan_psyq_signatures.py` scans a PS-X EXE against the masked full-object
patterns in `lab313ru/psx_psyq_signatures`, revision
`e9e46e7e133ef275a79bfce650924f98edb086bc`. It accepts only objects with one
complete payload match, ignores placeholder labels, and proposes names only at
preserved function starts. Catalogue hashes are included in its JSON output.
This is deliberately stricter than matching short function byte strings,
which collide frequently among kernel stubs and SDK wrappers.

A scan across all available catalogue versions found 1,407 unique
version/object matches and 209 named function-start proposals. Of those, 199
have a single name across every matching provider. The results identify the
upper executable as contiguous LIBPAD, LIBAPI/LIBC, LIBCARD, LIBCD, LIBETC,
LIBGPU, LIBGTE, LIBPRESS, and LIBSPU code, while preserving version ambiguity
where different catalogues contain identical objects. These names are
attribution evidence and a reconstruction work queue; they do not count as
matching C by themselves.

The first source-backed LIBSPU probe, `SpuGetTransferMode` at `0x80098F78`,
also established that SDK objects require their own build profiles. The
corresponding 4.0 reconstruction metadata uses `-O3 -g0 -G0
-funsigned-char`, rather than the game's usual `-O2` profile. Its recovered C
reproduces 15 of 16 instructions with the Cygnus 2.7.2 lane; only the register
chosen for the initial global-address materialization differs. It remains a
candidate pending an exact original-compiler lane or a natural source-shape
resolution and receives no matching credit.

The same LIBSPU profile exactly reproduces `SpuSetIRQAddr` at `0x80098BB8`
and the interrupt callback wrapper at `0x80098C38`, adding 100 verified bytes.
Declaring the transfer-address shadow register at `0x8009EEBC` volatile prevents
the stock 2.7.2 compiler from coalescing its store and reload address. That
recovers the missing instruction and makes `SpuSetTransferStartAddr` at
`0x80098EE8` an exact 92-byte function match. Its following four-byte zero is
object alignment and is not counted as code. The callback setter remains
unresolved.

Three additional LIBSPU wrappers match under the same Cygnus `-O3` profile:
the DMA callback bridge at `0x800979F8`, the cold-start initializer at
`0x80097A28`, and `SpuSetReverbVoice` at `0x80098788`. Their 104 verified bytes
exclude trailing object-alignment zeros.

Six small LIBCD entry points add another 196 verified bytes: `CdFlush`,
`CdSync`, `CdReady`, `CdGetSector`, `CdDataCallback`, and `CdDataSync`.
Five match the Cygnus 2.7.2 SDK profile, while `CdGetSector` selects 2.8.1
with otherwise identical object flags. The linked SDK region therefore mixes
compiler provenance even among adjacent public API wrappers.

`CdGetSector2` and the three small LIBPRESS entry points `DecDCTout`,
`DecDCTinCallback`, and `DecDCToutCallback` contribute another 136 verified
bytes. `CdGetSector2` selects 2.8.1 like its sibling, while the LIBPRESS
wrappers select the Cygnus 2.7.2 `-O3` profile.
The intervening 212-byte initializer at `0x8008B094` is `_padLoadActInfo`; it
lays out the mode, capability, and combination tables within a caller-provided
buffer and installs the two command-processing callbacks.
Its 628-byte response processor at `0x8008B210` reconstructs mode-table rows,
five-byte actuator capability records, and variable-length combination data.
It matches under GCC 2.7.2 and confirms `D_800A3B80` as the active combination
payload cursor.
The 104-byte `_padSetActAlign` routine at `0x8008B484` completes the path that
installs the actuator command emitter and six-byte map response handler.
The 332-byte main-mode chain at `0x8008B5D0..0x8008B71B` reconstructs
`_padSetMainMode`, its command emitter, and its completion handler. The setup
uses GCC 2.8.1; both callbacks match the 2.7.2 scheduling lane.
