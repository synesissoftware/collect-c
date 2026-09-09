# collect-c - Installation and Use <!-- omit in toc -->

**collect-c** is a classic-form C library, with implementation files under
**src** and headers under **include/collect-c**. Once installed, include the
appropriate headers (for example **collect-c/circq.h**, **collect-c/dlist.h**,
**collect-c/vec.h**) and link against the library (the **CMake** target is
`collect-c::core`).

The **C** API has no non-standard dependencies. Building the project's tests
(and example programs, when present) additionally requires **STLSoft**,
**cstring**, **Diagnosticism**, and **xTests** (and may optionally recognise
**shwild**).


## Table of Contents <!-- omit in toc -->

- [CMake](#cmake)
- [Bundled](#bundled)


## CMake

The primary choice for installation is by use of **CMake**.

1. Obtain the latest distribution of **collect-c**, from
   https://github.com/synesissoftware/collect-c/, e.g.

   ```bash
   $ mkdir -p ~/open-source
   $ cd ~/open-source
   $ git clone https://github.com/synesissoftware/collect-c/
   ```

2. Prepare the CMake configuration, via the **prepare_cmake.sh** script.

   For a minimal library install (no test dependencies required):

   ```bash
   $ cd ~/open-source/collect-c
   $ ./prepare_cmake.sh --disable-examples --disable-testing -v
   ```

   For a full build including tests, install **STLSoft** 1.11+, **cstring**,
   **Diagnosticism**, and **xTests** via their own **CMake** scripts first,
   then:

   ```bash
   $ cd ~/open-source/collect-c
   $ ./prepare_cmake.sh -v
   ```

   Useful optional flags:

   * `--disable-examples` / `-E` — omit examples (`BUILD_EXAMPLES=OFF`);
   * `--disable-testing` / `-T` — omit tests (`BUILD_TESTING=OFF`);
   * `--no-shwild` — do not recognise **shwild** (`NO_SHWILD`);
   * `--stlsoft-root-dir` / `-s` — pass an **STLSoft** source-tree root when
     it is not available as an installed **CMake** package;

   (**Hint**: execute `$ ./prepare_cmake.sh --help` for more information.)

3. Run a build of the generated **CMake**-derived build files via the
   **build_cmake.sh** script, as in:

   ```bash
   $ ./build_cmake.sh
   ```

   (**NOTE**: if you provide the flag `--run-make` (=== `-m`) in step 2 then
   you do not need this step.)

4. As a check (when testing was not disabled), execute the built unit-test
   programs via **run_all_unit_tests.sh**, as in:

   ```bash
   $ ./run_all_unit_tests.sh
   ```

5. Install (optional), for example:

   ```bash
   $ cmake --install _build --prefix ~/sis
   ```

   Consumers then use `find_package(collect-c REQUIRED)` and link
   `collect-c::core`.


## Bundled

As with other Synesis C libraries, **collect-c** may be vendored by copying
**include/collect-c** and the **src** implementation files into a dependent
tree and compiling them as part of that project. Prefer the **CMake** package
when integrating multiple Synesis libraries.


<!-- ########################### end of file ########################### -->
