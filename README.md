# collect-c <!-- omit in toc -->

Special and custom Collections and Containers, for C


![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![GitHub release](https://img.shields.io/github/v/release/synesissoftware/collect-c.svg)](https://github.com/synesissoftware/collect-c/releases/latest)
[![Last Commit](https://img.shields.io/github/last-commit/synesissoftware/collect-c)](https://github.com/synesissoftware/collect-c/commits/master)
[![CI](https://github.com/synesissoftware/collect-c/actions/workflows/ci.yml/badge.svg)](https://github.com/synesissoftware/collect-c/actions/workflows/ci.yml)


## Table of Contents <!-- omit in toc -->

- [Introduction](#introduction)
- [Installation](#installation)
- [Components](#components)
  - [Circular Queue](#circular-queue)
  - [Doubly-linked List](#doubly-linked-list)
  - [Tree-map](#tree-map)
  - [Vector](#vector)
  - [Common facilities](#common-facilities)
- [Examples](#examples)
- [Helper scripts](#helper-scripts)
- [Project Information](#project-information)
  - [Where to get help](#where-to-get-help)
  - [Contribution guidelines](#contribution-guidelines)
  - [Dependencies](#dependencies)
    - [Tests-only Dependencies](#tests-only-dependencies)
  - [Related projects](#related-projects)
  - [License](#license)


## Introduction

**collect-c** provides special-purpose collection and container types for
standard C (C99 and later). The library ships as a small, standalone C API
with no non-standard dependencies for application use. Unit tests are built
with **xTests** and related Synesis packages.


## Installation

Detailed instructions — via **CMake**, or by bundling sources — are provided
in the accompanying [INSTALL.md](./INSTALL.md) file.


## Components


### Circular Queue

Header: **collect-c/circq.h** (terse macros in **collect-c/terse/circq.h**).

Fixed-capacity circular queue (`collect_c_cq_t`) with push/pop at either end,
optional element free callbacks, and either heap or caller-supplied storage.


### Doubly-linked List

Header: **collect-c/dlist.h** (terse macros in **collect-c/terse/dlist.h**).

Intrusive-style doubly-linked list (`collect_c_dlist_t`) with front/back
operations, insert/erase/find, spare-node reuse, and walk/clear helpers.


### Tree-map

Header: **collect-c/tmap.h** (terse macros in **collect-c/terse/tmap.h**).

Binary tree-map (`collect_c_tmap_t`) of key/value pairs with insert, find,
and entry/node walk in forward, backward, or downward order. Stock key
comparators are provided for common integer and C-string types.


### Vector

Header: **collect-c/vec.h** (terse macros in **collect-c/terse/vec.h**).

Contiguous growable vector (`collect_c_vec_t`) of typed elements, with
push/pop and storage management APIs.


### Common facilities

* **collect-c/common/mem_api.h** — pluggable alloc/realloc/free used by all
  heap-backed containers (defaults to the C standard library);
* **collect-c/util/limits.h** — type-detecting min/max macros used internally
  (e.g. circular-queue index wrap safety);


## Examples

Example programs under **examples/** are not yet published in this alpha.
Unit, component, scratch, and performance programs under **test/** exercise
the containers (including **test.scratch.tmap**). See [FAQ.md](./FAQ.md).


## Helper scripts

CMake build helpers (**prepare_cmake.sh**, **build_cmake.sh**, and so on)
are documented in [INSTALL.md](./INSTALL.md).

**scripts/binary_split.rb** is an optional Ruby tool that prints every
integer in `[first, exclusive-last)` in binary-chop order — as if the
values were laid out in a (mostly) balanced binary tree. **tmap** is a
plain binary search tree, not self-balancing, so insertion order
determines tree shape. **test.performance.tmap** pastes these sequences
into the `BBT_*` arrays to compare balanced insert and walk against
ascending (degenerate) order.

`--format` selects the output form:

* **C-compatible-list** (`c`) — a C initializer, e.g. `{ 2, 1, 3 }`;
* **plain-sequence** (`p`) — space-separated numbers (the default), e.g. `2 1 3`;
* **ruby-list** (`r`) — a Ruby array, e.g. `[ 2, 1, 3 ]`

Ruby gems are listed in **Gemfile**. From the project root:

```bash
$ bundle install
$ bundle exec ./scripts/binary_split.rb 1 1001
$ bundle exec ./scripts/binary_split.rb --format c 1 1001
```


## Project Information


### Where to get help

* GitHub Issues: https://github.com/synesissoftware/collect-c/issues
* Project page: https://github.com/synesissoftware/collect-c


### Contribution guidelines

Contributions are welcomed. Please open an issue or pull request against
https://github.com/synesissoftware/collect-c.


### Dependencies

The **C** library itself has no non-standard dependencies.

#### Tests-only Dependencies

For unit-testing (and for example programs when present), **collect-c**
depends on:

* [**STLSoft**](https://github.com/synesissoftware/STLSoft/) 1.11+;
* [**cstring**](https://github.com/synesissoftware/cstring/);
* [**Diagnosticism**](https://github.com/synesissoftware/Diagnosticism/);
* [**xTests**](https://github.com/synesissoftware/xTests/) 0.26.4+;

and may optionally recognise:

* [**shwild**](https://github.com/synesissoftware/shwild/);


### Related projects

Other custom collection projects:

* [**collect-rs**](https://github.com/synesissoftware/collect-rs/) — Special and custom Collections and Containers, for Rust;


### License

**collect-c** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.


<!-- ########################### end of file ########################### -->
