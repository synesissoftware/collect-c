# collect-c - Changes <!-- omit in toc -->


## 0.1.0-alpha2 - 10th September 2026

* Added modular GitHub Actions CI (**ci.yml** + **ci-cell.yml**) covering Linux/macOS/Windows with Clang, GCC, VC++, and MinGW, plus install-smoke;
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
