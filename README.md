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
  - [Vector](#vector)
- [Examples](#examples)
- [Project Information](#project-information)
  - [Where to get help](#where-to-get-help)
  - [Contribution guidelines](#contribution-guidelines)
  - [Dependencies](#dependencies)
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
operations and walk/clear helpers.


### Vector

Header: **collect-c/vec.h** (terse macros in **collect-c/terse/vec.h**).

Contiguous growable vector (`collect_c_vec_t`) of typed elements, with
push/pop and storage management APIs.


## Examples

Example programs are not yet published in this alpha. See [FAQ.md](./FAQ.md)
for notes on tests and scratch programs.


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
* [**xTests**](https://github.com/synesissoftware/xTests/);

and may optionally recognise:

* [**shwild**](https://github.com/synesissoftware/shwild/);


### Related projects

Other custom collection projects:

* [**collect-rs**](https://github.com/synesissoftware/collect-rs/) — Special and custom Collections and Containers, for Rust;


### License

**collect-c** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.


<!-- ########################### end of file ########################### -->
