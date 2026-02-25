# af_memory
This is af_memory, a custom, thread-safe, POSIX-compliant memory allocator library written in C.

I built this to understand exactly how the operating system manages RAM under the hood. It provides a complete, drop-in replacement for the standard malloc, free, calloc, and realloc functions. It is packaged as a shared library that you can install and link against any of your C projects.

## Core features
- **Direct Kernel Mapping & Unmapping:** The library bypasses the standard heap entirely. It talks 
  directly to the kernel using mmap to request large chunks of memory (4MB heaps). More importantly, if a 4MB heap becomes completely empty, the allocator acts as a good citizen and uses munmap to return that memory straight back to the OS.
- **Segregated Free Lists:** Free blocks are sorted into size-specific buckets. Finding a suitable 
  block is essentially an O(1) operation.
- **Boundary Tags & Coalescing:** Every block has metadata headers and footers. When you free a 
  block, the allocator instantly checks its neighbors and merges them in O(1) time, crushing external fragmentation.
- **Block Splitting:** Large blocks are automatically split to satisfy small memory requests, 
  minimizing internal waste.
- **Thread-Safety:** The entire API is protected by POSIX mutexes, making it safe to use in 
  multi-threaded environments.
- **Zero-Overhead Payload:** Using C unions, the free-list pointers share the exact same memory 
  space as the user's data payload. When a block is in use, the metadata pointers disappear, giving you maximum space.

## Technical limitations
While this library is robust and handles extreme multi-threaded stress tests without crashing, here is how it compares to massive industrial allocators like jemalloc or ptmalloc

- **Global Lock Contention:** The library relies on a single global mutex. While this makes it 
  thread-safe, it can become a bottleneck in highly concurrent applications (like a massive async web server) where hundreds of threads are fighting for the same lock. Industrial allocators use Thread-Local Caching (Arenas) to avoid this.
- **Aggressive Unmapping:** Because it instantly munmaps a heap as soon as it is completely free, 
  it could technically suffer from "thrashing" (a CPU-heavy ping-pong effect of calling mmap/munmap repeatedly) if an application constantly allocates and frees exactly 4MB on loop. There is currently no "reserve heap" logic.
- **Security:** It does not implement modern exploit mitigations like guard pages or encrypted 
  metadata pointers. Use it for your personal projects, emulators, and tools, but avoid using it for highly sensitive security software.

## Installation

he project uses Modern CMake.
```bash
mkdir build && cd build

# Configure CMake to install into your home folder
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local ..

# Compile and install
make
make install
```

This places the header in ~/.local/include and the shared library (.dylib or .so) in ~/.local/lib.

## Usage
```
#include <af_memory.h>
#include <stdio.h>

int main(void) {
    char *data = af_malloc(128);
    
    if (data) {
        printf("Memory allocated using custom af_memory\n");
        af_free(data); 
    }
    
    return 0;
}
```