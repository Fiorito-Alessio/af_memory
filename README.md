# af_memory

A custom, thread-safe memory allocator written in C.

I wanted to see what actually happens under the hood when you call `malloc`, `calloc`, `realloc` 
and 
`free`, 
so I 
built my own from scratch. It is a fully functional, POSIX-compliant drop-in replacement packaged as a shared library.

## How it works

Instead of relying on the standard library, `af_memory` talks directly to the OS kernel using `mmap` to grab 4MB chunks of memory.



To keep things fast and prevent fragmentation, it uses a few classic architecture tricks:
* **Segregated Free Lists & Boundary Tags:** Free blocks are sorted into size buckets. Finding a block and merging adjacent free blocks (coalescing) are both O(1) operations.
* **Zero-Overhead:** It uses C unions to store metadata pointers right inside the user payload area when the block is free. No wasted space.
* **Kernel Returns:** If a 4MB heap becomes completely empty after a `free`, the allocator doesn't hoard it. It uses `munmap` to give it right back to the OS.
* **Thread-Safe:** Everything is protected by POSIX mutexes, so it won't crash if multiple threads allocate memory at the same time.

## The Catch (Honest Talk)

It works great, but it has boundaries compared to industrial allocators like `jemalloc`:
* **Global Lock:** It uses a single global mutex. If you use this in a massive async web server, threads will fight for the lock and bottleneck. Industrial allocators use thread-local arenas to fix this.
* **Thrashing:** Because it aggressively unmaps empty heaps, an app that constantly allocates and frees exactly 4MB on a loop will force the CPU to ping-pong between `mmap` and `munmap`.
* **Security:** It lacks modern exploit mitigations (like guard pages). Use it for personal 
  projects.

## Installation

You can install it locally to your home directory. You just need CMake and a C compiler.

```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local ..
make
make install
```
## Usage

Include the header in your C project and use it like the standard library.
```c++
#include <af_memory.h>

int main(void) {
    char *data = af_malloc(128);
    
    // Do your thing
    
    af_free(data); 
    return 0;
}
```
