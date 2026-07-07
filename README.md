# avoEngine

avoEngine is a small hobby database engine written in C++ that is evolving from a simple table layer into a more complete storage engine.

## What has been implemented so far

- A basic table abstraction that supports insert, find, remove, and row counting.
- A B-tree implementation for key/value storage and basic lookup/removal behavior.
- A pager and page layer for reading and writing fixed-size pages to disk.
- A slotted-page storage structure for tuple layout and slot management.
- Initial hash-related components, including a choice vector and basic hashing utilities.
- A smoke-test suite that verifies core table behavior end to end.

## Current status

The engine is in a working prototype stage. The current test suite passes, and the core insert/find/remove flow is functioning.

## How to build and test

```bash
make test
```

This builds the project and runs the existing smoke tests.

## Where to go next

The next priorities are:

1. Finish the linear-hash manager implementation and connect it to real tuple storage.
2. Complete the slotted-page compaction and overflow-page handling logic.
3. Add more comprehensive tests for edge cases, persistence, and larger datasets.
4. Improve the hashing and tuple layout code to better match a real database engine design.

