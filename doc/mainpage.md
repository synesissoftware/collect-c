# collect-c {#mainpage}

**collect-c** provides special-purpose collection and container types for
standard **C** (C99 and later). The library ships as a small, standalone C
API with no non-standard dependencies for application use.


## Components

| Module | Header | Summary |
| ------ | ------ | ------- |
| Circular queue | `<collect-c/circq.h>` | Fixed-capacity circular queue (`collect_c_cq_t`) |
| Doubly-linked list | `<collect-c/dlist.h>` | Intrusive-style doubly-linked list (`collect_c_dlist_t`) |
| Tree-map | `<collect-c/tmap.h>` | Binary tree-map of key/value pairs (`collect_c_tmap_t`) |
| Vector | `<collect-c/vec.h>` | Contiguous growable vector (`collect_c_vec_t`) |
| Common / version | `<collect-c/common.h>`, `<collect-c/common/version.h>` | Shared types and `COLLECT_C_VER_*` / `collect_c_api_version()` |
| Memory API | `<collect-c/common/mem_api.h>` | Pluggable alloc/realloc/free |
| Key comparison | `<collect-c/compare.h>` | Stock key comparison for hashtable keys |
| Hashing | `<collect-c/hash.h>` | Stock 64-bit hashes of counted multibyte buffers |
| Limits | `<collect-c/util/limits.h>` | Type-detecting min/max macros |


## Quick start

Include the container header you need and link against the **collect-c**
`core` library:

```c
#include <collect-c/vec.h>

collect_c_vec_t v;
/* … */
```

Consumer **CMake** projects may use `find_package(collect-c)` and link
`collect-c::core`. See
[INSTALL.md](https://github.com/synesissoftware/collect-c/blob/master/INSTALL.md)
for build and install instructions.


## Related projects

* [**collect-cxx**](https://github.com/synesissoftware/collect-cxx/) — same
  family for **C++**;
* [**collect-rs**](https://github.com/synesissoftware/collect-rs/) — same
  family for **Rust**;


<!-- ########################### end of file ########################### -->
