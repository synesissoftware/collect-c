# collect-c - Changes <!-- omit in toc -->


## 0.1.0-alpha3 - 10th September 2026

* Added tree-map container (**tmap** / **collect-c/tmap.h**, terse API in **collect-c/terse/tmap.h**) with insert, find, entry/node walk, and free-storage;
* Added custom memory API (**collect-c/common/mem_api.h**) and applied it across **circq**, **dlist**, **vec**, and **tmap**;
* Added stock comparison functions (sized and built-in integers; C-strings, including case-insensitive) and **collect_c_stricmp**;
* Added type-detecting limits macros (**COLLECT_C_LIMITS_maximum** / **COLLECT_C_LIMITS_minimum** in **collect-c/util/limits.h**); `_Generic` associations use distinct built-in types only so stdint typedefs do not create duplicate matches across ABIs;
* Circular queue: fixed index wrap when `b`/`e` approach the end of their integral value space; custom **mem_api** support;
* Doubly-linked list: spare-node reuse (cap 64), push-back/front by-ref macros, **COLLECT_C_DLIST_free_storage**, element-free `el_index` always `-1`, shared **collect_c_common_node_data_t**, custom **mem_api**;
* Vector: custom **mem_api** for allocate/realloc/free paths;
* Unit tests: **test.unit.tmap**, **test.unit.stock_cmp_fns**, **test.unit.util.limits**; expanded **cq** / **dlist** / **vec** / **version** coverage (including custom **mem_api** cases); GCC/MSVC warning fixes in **stock_cmp_fns.c**, **string_fns.c**, **tmap.c**;
* Performance: **test.performance.dlist** and **test.performance.std_list**; scratch: **test.scratch.tmap**;
* Helper scripts: **run_all_unit_tests.sh** `--unit-only` / `--component-only`; **prepare_cmake.sh** `--msvc-mt` and clearer testing-deps help;
* Required **xTests** version raised to **0.26.4**;


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
