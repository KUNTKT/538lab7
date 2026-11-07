# EE538 — Heaps and Hashing Lab

- Name:
- Student ID:
- Compiler and version: clang via Zig 0.17.1 (`zig c++ -std=c++17 -Wall -Wextra -Wpedantic`)
- Extra credit attempted: Yes

Read [Assignment.html](Assignment.html) for all requirements and commands.
Complete this file with brief answers and your test results.

## Implementation

1. Heap: In what order does your bottom-up build repair the heap?
   Why is the total cost O(n), rather than O(n log n)?

   Answer: `build` copies the input, then calls `sift_down` on every internal
   node from the last one (index `n/2 - 1`) back to the root (index 0). The loop
   counts `i` from `n/2` down to 1 and uses `i - 1`, so the unsigned index never
   underflows. Each node's subtrees are already heaps when it is repaired. The
   cost is O(n) because sift-down work is bounded by a node's height, not by
   log n: about n/2 nodes are leaves (0 work), n/4 have height 1, n/8 have
   height 2, and so on. The sum n·Σ h/2^(h+1) converges to O(n). Only the few
   nodes near the root pay the full O(log n).

2. Hash table: How does a collision differ from a duplicate key?
   How do `put` and `erase` maintain the correct `size_`?

   Answer: A collision is two *different* keys that hash to the same bucket
   (e.g. `a`, `h`, `o` all map to bucket 6). They are separate entries in the
   same chain and are told apart by comparing the full key strings. A duplicate
   is the *same* key inserted again, which updates the existing entry's value.
   `put` scans the chain. If it finds an equal key it overwrites the value and
   returns without touching `size_`. Otherwise it appends a new entry and
   increments `size_`. `erase` decrements `size_` only when it actually removes
   a matching entry (then returns `true`). For an absent key it changes nothing
   and returns `false`.

## Testing

| Provided checks | Passed / total |
|---|---|
| Required heap and hash-table tests | 11 / 11 |
| Provided SHA-256 helper tests | 4 / 4 |
| Optional log-chain tests | 6 / 6 |

The sample sessions from Assignment.html also produce the expected output
(heap: `0 -2 0`; hash table: `25 true NOT_FOUND 30 2`; log chain: `true false true`).

| Your test | Commands / operations | Expected result | Actual result |
|---|---|---|---|
| Heap | `BUILD 6 3 -1 3 -5 0 -1`, `PUSH -5`, then `POP` ×7, `EMPTY`, `POP` | Pops in nondecreasing order with duplicates/negatives: `-5 -5 -1 -1 0 3 3`, then `true`, then an error on the empty pop | `-5 -5 -1 -1 0 3 3`, `true`, `ERROR: empty heap` |
| Hash table | `PUT a 1`, `PUT h 2`, `PUT o 3` (all bucket 6), `ERASE a` (head of chain), `FIND h`, `FIND o`, `PUT "" -1`, `FIND ""`, `ERASE missing`, `SIZE` | `true`, `2`, `3`, `-1`, `false`, `3`. The surviving colliding keys are still found, -1 is a real value, and the missing erase leaves the size unchanged | `true`, `2`, `3`, `-1`, `false`, `3` |

## Extra credit (if attempted)

Why must the trusted final digest be kept separately from editable history?
What happens if an attacker changes a record and recomputes later digests?

Answer: Every digest depends on all earlier records. If you edit one record,
its stored digest and every digest after it stop matching what `verify`
recomputes. But the chain only proves it is consistent with itself. An attacker
who can edit the history can also recompute `d_i … d_n` after the change and
produce a chain that checks out internally, with a different final digest. So
the anchor must be stored somewhere the attacker cannot write. Comparing the
recomputed final digest against that saved anchor catches the forgery, because
the rewritten chain's final digest will not match the original. If the anchor
were stored with the history, the attacker could just replace it too. The chain
detects tampering. It does not prevent edits, encrypt records, or authenticate
the writer.

## Known issues and references

No known incomplete behavior. All required and optional TODOs are implemented.
References: course Assignment.html and the provided headers/tests only.
