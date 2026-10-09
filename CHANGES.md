# collect-c - Changes <!-- omit in toc -->


## Unreleased

* Brought CMake helper scripts to Phase 4b gold: replaced the **make**-driven **prepare_cmake.sh** / **build_cmake.sh** / **clean_cmake.sh** / **run_all_*.sh** with the **SisClr** **cmake --build** set (MinGW only via `--mingw`; colour via `SIS_CMAKE_ALWAYS_USE_COLOURS`), preserving `--no-shwild` and `--stlsoft-root-dir` / `-s`;
* Added **ctest_cmake.sh**, **run_all_automated_tests.sh**, **run_all_component_tests.sh**, **run_all_performance_tests.sh**, and the native `cmd.exe` **run_all_{automated,component,examples,performance,scratch,unit}_tests.cmd** runners (no Bash wrappers);
* **run_all_unit_tests.sh** is now unit-only (aggregate behaviour is **run_all_automated_tests.sh**), and **run_all_scratch_tests.sh** no longer runs performance programs (see **run_all_performance_tests.sh**);
* Renamed the scratch reporter target / argv0 to `test.scratch.versions` (Phase 4c);
* Applied **misc-dev-scripts** **0.6.0** editor/Git/`.sis` drop-in templates on **boilerplate**;
* Restored historical **.gitignore** patterns as a sorted union with **misc-dev-scripts** gold section layout;


## 0.1.0-alpha2 - 10th September 2026

* Added modular GitHub Actions CI (**ci.yml** + **ci-cell.yml**) covering Linux/macOS/Windows with Clang, GCC, VC++, and MinGW, plus install-smoke (consumer sets `CMAKE_C_STANDARD` 17 for MSVC `__STDC_VERSION__`);
* Added **install-sis-deps** composite action for test dependency installation;
* Added/filled customary markdown: **AUTHORS.md**, **CHANGES.md**, **FAQ.md**, **INSTALL.md**, **NEWS.md**, **README.md**;
* CMake helper scripts: coloured status output and related chore improvements;
* VC++ compatibility;
* **CMakeLists.txt** — corrected project description (C, not C++); dependency table Examples column; **STLSoft** / **cstring** / **Diagnosticism** `find_package` gated on examples or tests;
* **src/CMakeLists.txt** — removed erroneous **STLSoft** `PRIVATE` link from exported **core** (library is standalone);


## 0.1.0-alpha1 - 11th February 2025

* Initial public alpha of **collect-c**;
* Circular queue (**circq**), doubly-linked list (**dlist**), and vector (**vec**) containers;
* CMake packaging and helper scripts;
* Unit tests for version, **cq**, **dlist**, and **vec**;


<!-- ########################### end of file ########################### -->
