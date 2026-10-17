# Playback queries, command wrappers and packet return ABI

This continuation builds on the sixteen-function synchronization/audio batch
submitted as PR 24. Its contribution boundary is commit
`429287f7f2405f1bbb1c611911b4143b7a013827`; functions already in that PR are
not counted again as new work here.

| Address | Recovered behavior |
| --- | --- |
| `00558A40` | Return zero when the service/state is unavailable; otherwise bracket the float getter `00557EB0` with the context wait/signal helpers. |
| `00558AC0` | The corresponding integer query through `00557EB8`. |
| `005670B0` | Map a selector through `005657E0`; on success, submit operation 5 with an eight-byte payload containing the two input values. |
| `00567128` | Map a selector and submit operation 6 with no payload. |
| `00567718` | Transfer an intrusive queue node through the pending-tail slot and return its payload address, preserving the original access sequence. |
| `0055F4F8` | Build the default packet prefix in a local record and return the complete record by value. |

## Query and command models

Both query wrappers use the same opaque `PlaybackQueryState` and the global
enable/state guards. Their callees load a float at `+0x780` and an integer
at `+0x77C`. Their application-level meanings remain unknown. The shared
helper argument is not named as a semaphore identifier: `00578500` derives
its wait context internally. The wrappers preserve the result across the
signal helper and return zero without entering the helpers when unavailable.

The command wrappers share an identical partial `AudioNode` view, with a
dispatch gate at `+0x14` and a query-state pointer at `+0x60`. `005657E0`
maps the selector to a stored result and returns zero on success, or three
when the result is `-1`. Both wrappers skip dispatch after a failed lookup.

The dispatcher `00565C30` takes six arguments: owner, mapped value, payload,
payload byte count, operation and flag. Its code copies the payload into
its command buffer before submitting the command, so the wrapper's payload
can be an ordinary local. `005670B0` has two distinct locals: a two-word
`ControlPayload` and the lookup result. An early numerically matching draft
bundled these into a padded artificial record; that model was rejected.
The natural separate-local version matches without invented padding.

## Intrusive queue slots

The queue layout agrees with the already recovered `005676B0`: a head and
tail slot for each of two lists. Tail members point to a `Node *` slot, not
to an opaque byte region. Appending therefore uses `*pendingTail = node`
and advances the slot to `&node->next`. The empty-list tail points to the
queue's own head slot.

The original clears `node->next`, writes through the pending tail, and then
reloads `node->next` to update the source head. Those pointers may alias;
caching an earlier next value or replacing the sequence with a conventional
dequeue would change behavior. The source preserves these accesses under
the original interrupt helpers. The returned address is four bytes after
the node on the PS2; this routine does not allocate storage or establish a
new payload lifetime.

## Returning a packet record

The ABI and copy instructions establish a record of `0xAE` bytes with
two-byte alignment. Its first two fields are the packet length and type;
the remaining layout is left unknown. A normal C++ function returning this
record by value reproduces the hidden destination pointer, local packet
builder call and compiler-generated full-record copy. No manual copy loop,
extra staging buffer or register assignment is needed.

The default builder initializes only the prefix described by the length
field. Bytes beyond that prefix remain unspecified, matching the original;
the factory does not silently zero the rest of the returned record.

## Validation and deferred work

All six final sources passed instruction matching after semantic review and
readability cleanup. A temporary host harness compiles the actual two
command wrappers with AddressSanitizer and UndefinedBehaviorSanitizer. Its
fixtures cover disabled dispatch, failed lookup, both exact payload words,
operation/size/flag arguments, and the payload-free command.

The worker loop `00558330` remains at 39/65 differing instructions; the
initialization/thread-setup candidate `005585C8` remains at 65/69. Both are
excluded from `src/`. Their partial state meanings and frame/register
differences are recorded in the attempts diary for later investigation.

## Complete-build proof

The non-partial build of clean source commit
`1cd1c9cbcd96845f17c48d843d1164d22cbf8c1a` passed with
`tools/build.py --jobs 2`. All six additions have the exact status `linked`,
contributing **824 additional linked code bytes** beyond the PR 24 branch
boundary above. In address order their sizes are 124, 112, 268, 116, 88 and
116 bytes. The sixteen functions from PR 24 also remain individually linked.

The build covers 29,079 sources and 4,169,884 linked code bytes, with 55,106
verified source data bytes, no orphan sources and no duplicate sources.
Both segments are identical to the original:

- `.text`: `28422e7e20a6298224294255355487b9a22cef31`
- `.data`: `c28c833f9716467545bf362f321e9acc876c3603`

All 85 repository tests, the command-wrapper sanitizer fixtures, assembly
policy checks and source organization check passed. README progress was
regenerated from this full build with `tools/report.py` and
`tools/update_readme.py`. The upstream refresh still resolved to
`6e18ca3768811c5338401f42b6e13e619e502428`; PR 24 remained open and both of
its GitHub check jobs had passed. This continuation stays on a separate
branch while additional related work is accumulated.
