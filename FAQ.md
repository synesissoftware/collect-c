# collect-c - FAQ <!-- omit in toc -->

The FAQ list is under (constant) development. If you post a question on the
Issues forum (https://github.com/synesissoftware/collect-c/issues)
it will be used to create one.


## Table of Contents <!-- omit in toc -->

- [Q1: "How do I build collect-c?"](#q1-how-do-i-build-collect-c)
- [Q2: "How do I install collect-c?"](#q2-how-do-i-install-collect-c)
- [Q3: "How do I use collect-c?"](#q3-how-do-i-use-collect-c)
- [Q4: "Does collect-c have its own unit-tests?"](#q4-does-collect-c-have-its-own-unit-tests)
- [Q5: "Where are the examples?"](#q5-where-are-the-examples)


# FAQs: <!-- omit in toc -->


## Q1: "How do I build collect-c?"

See [INSTALL.md](./INSTALL.md) for the recommended **CMake** flow
(**prepare_cmake.sh**, then **build_cmake.sh**).

For a minimal library install with no external dependencies:

```bash
$ ./prepare_cmake.sh --disable-examples --disable-testing -m
```

For a full build including tests, install **STLSoft** 1.11+, **cstring**,
**Diagnosticism**, and **xTests** first, then:

```bash
$ ./prepare_cmake.sh -m
```

Execute `$ ./prepare_cmake.sh --help` for the full set of options.


## Q2: "How do I install collect-c?"

See [INSTALL.md](./INSTALL.md) for details of how to install **collect-c**.


## Q3: "How do I use collect-c?"

Include the container header you need — for example **collect-c/circq.h**,
**collect-c/dlist.h**, **collect-c/tmap.h**, or **collect-c/vec.h** — and
link against **libcollect-c.core** (the **CMake** target is
`collect-c::core`). The headers are C99; they are not currently intended for
direct C++ compilation. Optional custom allocators are configured via
**collect_c_mem_api_t** on each container instance.


## Q4: "Does collect-c have its own unit-tests?"

Yes. Unit-test programs live under **test/unit/** and are built when
`BUILD_TESTING` is enabled. Run them with **run_all_unit_tests.sh** (see
`--unit-only` / `--component-only`). Building tests requires **STLSoft**,
**cstring**, **Diagnosticism**, and **xTests** **0.26.4**+.


## Q5: "Where are the examples?"

Example programs under **examples/** are not yet published in this alpha.
Scratch and performance programs under **test/** (e.g. **test.scratch.tmap**,
**test.performance.dlist**) may be useful while examples are prepared.


<!-- ########################### end of file ########################### -->
