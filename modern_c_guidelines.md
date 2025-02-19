
# **Modern Safe C Guide**  

## **1. General Code Conventions**  
### **Naming Conventions**  
- **Functions**: `snake_case()`, use descriptive names.  
  ```c
  void compute_crc32(const uint8_t *data, size_t length);
  ```
- **Variables**: `snake_case`, prefer meaningful names.  
  ```c
  uint32_t checksum_value;
  ```
- **Constants**: `UPPER_CASE_WITH_UNDERSCORES`  
  ```c
  #define MAX_BUFFER_SIZE 1024
  ```
- **Structs & Enums**: Prefix with type  
  ```c
  typedef struct person_t {
      char name[50];
      uint8_t age;
  } person_t;
  ```

---

## **2. Safety & Memory Management**  
### **Use `stdint.h` for Fixed-Size Types**  
✅ Safer than `int`, `long`, etc.  
```c
#include <stdint.h>
uint32_t value;
int64_t large_number;
```

### **Use `size_t` for Sizes & Indexing**  
✅ Prevents signed/unsigned mismatches.  
```c
for (size_t i = 0; i < length; i++) { ... }
```

### **Avoid Magic Numbers, Use Constants**  
❌ **Bad:**  
```c
if (buffer_size > 1024) { ... }
```
✅ **Good:**  
```c
#define MAX_BUFFER_SIZE 1024
if (buffer_size > MAX_BUFFER_SIZE) { ... }
```

### **Always Initialize Variables**  
✅ Avoids **undefined behavior (UB)**.  
```c
int x = 0;
uint8_t buffer[256] = {0};
```

### **Use `const` for Read-Only Data**  
✅ Prevents unintended modifications.  
```c
void print_data(const char *data);
```

### **Use `restrict` for Performance (C99+)**  
✅ Compiler optimizations  
```c
void process_array(int *restrict arr);
```

### **Avoid `malloc()` If Not Necessary**  
✅ Prefer **stack** when possible.  
```c
char local_buffer[1024]; // Stack (fast)
char *heap_buffer = malloc(1024); // Heap (slower, needs free)
```

✅ **If malloc is required, always check & free**  
```c
char *ptr = malloc(256);
if (!ptr) {
    perror("malloc failed");
    return -1;
}
free(ptr);
```

---

## **3. Functions & Modularity**  
### **Keep Functions Small & Focused**  
✅ **One function, one responsibility.**  
```c
void compute_crc32(const uint8_t *data, size_t length);
void read_file(const char *filename);
```

### **Use `static` for Internal Functions**  
✅ Avoids name collisions in large projects.  
```c
static void helper_function(void);
```

### **Avoid Global Variables**  
✅ If needed, make them `static` and `const`.  
```c
static const double PI = 3.14159;
```

---

## **4. Pointers & Arrays**  
### **Always Check for `NULL` Before Using a Pointer**  
❌ **Bad:**  
```c
printf("%s", ptr);
```
✅ **Good:**  
```c
if (ptr) {
    printf("%s", ptr);
}
```

### **Use `const` When Passing Strings or Arrays**  
```c
void print_message(const char *msg);
```

### **Pass Structs by Pointer for Efficiency**  
✅ Avoids unnecessary copies.  
```c
void process_data(struct data_t *data);
```

---

## **5. Strings & Buffer Safety**  
### **Use `snprintf()` Instead of `sprintf()`**  
❌ **Bad (buffer overflow risk)**  
```c
char buf[32];
sprintf(buf, "User: %s", username);
```
✅ **Good (safe, prevents overflow)**  
```c
snprintf(buf, sizeof(buf), "User: %s", username);
```

### **Use `strncpy()` Instead of `strcpy()`**  
✅ Ensure null termination.  
```c
strncpy(dest, src, sizeof(dest) - 1);
dest[sizeof(dest) - 1] = '\0';  // Ensure null termination
```

---

## **6. File Handling**  
### **Always Check `fopen()` Return Value**  
```c
FILE *file = fopen("data.txt", "r");
if (!file) {
    perror("Failed to open file");
    return -1;
}
fclose(file);
```

### **Use `fgets()` Instead of `gets()`**  
✅ Prevents buffer overflow.  
```c
char buffer[128];
if (fgets(buffer, sizeof(buffer), stdin)) {
    printf("Input: %s", buffer);
}
```

---

## **7. Error Handling & Debugging**  
### **Use `errno` and `perror()` for System Calls**  
```c
#include <errno.h>
#include <stdio.h>

if (remove("file.txt") != 0) {
    perror("Error deleting file");
}
```

### **Use Assertions for Debugging (`assert.h`)**  
```c
#include <assert.h>
assert(pointer != NULL);
```

---

## **8. Thread Safety (C11 `stdatomic.h`)**  
### **Use `atomic` for Shared Data**  
```c
#include <stdatomic.h>
atomic_int counter = 0;
```

### **Use Mutexes When Needed (`pthread.h`)**  
```c
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
pthread_mutex_lock(&lock);
// critical section
pthread_mutex_unlock(&lock);
pthread_mutex_destroy(&lock);
```

---

# **Modern C Cheatsheet**
| Concept            | Example |
|--------------------|---------|
| **Fixed-size integers** | `uint32_t val = 123;` |
| **Safe array size** | `size_t length = sizeof(arr) / sizeof(arr[0]);` |
| **Memory allocation** | `ptr = malloc(size); if (!ptr) return -1;` |
| **Safe string copy** | `strncpy(dest, src, sizeof(dest)-1); dest[sizeof(dest)-1] = '\0';` |
| **File handling** | `FILE *f = fopen("file.txt", "r"); if (!f) { perror("Error"); }` |
| **Safer printing** | `snprintf(buf, sizeof(buf), "%s", str);` |
| **Pointer checks** | `if (ptr == NULL) { return -1; }` |
| **Atomic variables** | `atomic_int counter = 0;` |
| **Thread-safe lock** | `pthread_mutex_lock(&lock); ... pthread_mutex_unlock(&lock);` |
| **Error handling** | `if (f == NULL) { perror("File error"); return -1; }` |

---

## **Final Thoughts**  
- Prefer `const`, `static`, `size_t`, `stdint.h`.  
- Avoid raw pointers when possible.  
- Always check return values (`malloc()`, `fopen()`, etc.).  
- Use **modular functions** and **safe standard functions** (`snprintf()`, `strncpy()`).  
- Leverage **C11 features** (`stdatomic.h`, `_Generic`).  
- Use **CMake** and **unit tests** (`cmocka`, `Unity`).  


## **1. Modern C Features & Best Practices (C11 & Later)**  
### ✅ **Use `_Static_assert()` for Compile-Time Checks (C11)**  
```c
#include <assert.h>
_Static_assert(sizeof(int) == 4, "int must be 4 bytes on this platform");
```
**Why?** Prevents surprises with architecture-dependent sizes.  

---

### ✅ **Use `_Generic` for Type-Safe Macros (C11)**  
This is useful for writing **type-generic** functions (like `max()`, `min()`).  
```c
#define max(a, b) _Generic((a), \
    int: max_int, \
    float: max_float, \
    double: max_double \
)(a, b)

int max_int(int a, int b) { return a > b ? a : b; }
float max_float(float a, float b) { return a > b ? a : b; }
double max_double(double a, double b) { return a > b ? a : b; }
```
**Why?** Avoids unsafe macros like `#define max(a, b) ((a) > (b) ? (a) : (b))` which can have unintended behavior with expressions.  

---

### ✅ **Use `alignas` & `alignof` for Performance (C11)**  
If you need **specific memory alignment** (e.g., for SIMD optimizations or hardware constraints).  
```c
#include <stdalign.h>

struct alignas(16) Vector4 { float x, y, z, w; };
printf("Alignment of Vector4: %zu\n", alignof(struct Vector4));
```
**Why?** Ensures proper memory alignment for better performance in low-level applications.  

---

### ✅ **Safer Threading with `stdatomic.h` (C11)**  
Instead of using mutexes for simple atomic operations:  
```c
#include <stdatomic.h>

atomic_int counter = 0;
void increment() { atomic_fetch_add(&counter, 1); }
```
**Why?** **Lock-free**, **safe**, and faster than `pthread_mutex_t` in many cases.  

---

## **2. Modern C Code Organization & Build Systems**  
### ✅ **Use `CMake` for Modern Project Structure**  
Instead of `Makefiles`, use `CMakeLists.txt`:  
```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject C)
set(CMAKE_C_STANDARD 11)

add_executable(my_app main.c utils.c)
target_include_directories(my_app PRIVATE include)
```
**Why?** Portable, modular, and integrates well with IDEs and cross-platform builds.  

---

### ✅ **Use Unit Testing (`cmocka`, `Unity`, `Catch2`)**  
```c
#include <cmocka.h>

static void test_addition(void **state) {
    (void) state; // unused
    assert_int_equal(2 + 2, 4);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_addition),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
```
**Why?** Proper **unit testing** ensures correctness and reliability.  

---

### ✅ **Use `do { ... } while (0)` for Multi-Statement Macros**  
```c
#define LOG_ERROR(msg) do { \
    fprintf(stderr, "Error: %s\n", msg); \
    exit(EXIT_FAILURE); \
} while (0)
```
**Why?** Ensures macro works safely inside `if` conditions.  

---

## **3. Memory Safety & Bounds Checking**  
### ✅ **Use `__builtin_expect()` for Likely/Unlikely Branching (GCC/Clang)**  
Hinting the compiler for better **branch prediction** in performance-critical code.  
```c
if (__builtin_expect(ptr == NULL, 0)) {
    return -1;
}
```
**Why?** Helps CPUs optimize execution paths.  

---

### ✅ **Use `valgrind` & `AddressSanitizer` to Catch Bugs**  
Compile with `-fsanitize=address -g` for **memory leak detection**.  
```sh
gcc -fsanitize=address -g myprogram.c -o myprogram
./myprogram
```
**Why?** Helps detect **buffer overflows, leaks, and use-after-free bugs**.  

---

## **4. Data-Oriented Programming (DOP) & Cache Efficiency**  
Instead of **object-oriented** patterns in C (which are unnatural), **use SoA (Structure of Arrays) instead of AoS (Array of Structures)** for better **cache performance**.  

**Bad (Array of Structures - AoS, bad for cache)**  
```c
typedef struct {
    float x, y, z;
} Point;

Point points[1000];
```

**Good (Structure of Arrays - SoA, cache-friendly)**  
```c
typedef struct {
    float x[1000], y[1000], z[1000];
} PointArray;

PointArray points;
```
**Why?** SoA helps CPU cache performance **(faster SIMD operations, fewer cache misses)**.  

---

## **5. Modern Alternative to `malloc()` - `talloc()` (Hierarchical Memory)**  
Instead of managing **raw `malloc()` calls**, `talloc()` helps **automatic cleanup** like a **garbage collector**.  
```c
#include <talloc.h>

int main() {
    void *ctx = talloc_new(NULL);
    char *msg = talloc_strdup(ctx, "Hello, World!");
    
    printf("%s\n", msg);
    talloc_free(ctx); // Frees all memory at once
}
```
**Why?** Avoids **memory leaks** by grouping allocations and freeing everything at once.  

---

## **6. Safer Error Handling: Use `struct` Instead of `errno`**  
Instead of relying on **global `errno`**, use a structured approach:  
```c
typedef struct {
    int success;
    int error_code;
} Result;

Result divide(int a, int b) {
    if (b == 0) return (Result){ .success = 0, .error_code = -1 };
    return (Result){ .success = 1, .error_code = a / b };
}
```
**Why?** Makes error handling **explicit** and **thread-safe**.  

---

# **Final Notes & Summary**
| Feature | Modern C Best Practice |
|---------|------------------------|
| **Fixed-size types** | `stdint.h` (`uint32_t`, `size_t`) |
| **Safe Macros** | Use `_Generic` and `do { } while(0)` |
| **Compile-time checks** | `_Static_assert(sizeof(int) == 4, "Size mismatch")` |
| **Safer Memory Allocation** | `talloc()` instead of `malloc()` |
| **Thread Safety** | `stdatomic.h` for atomic variables |
| **Error Handling** | Use `struct Result` instead of global `errno` |
| **Optimized Branching** | `__builtin_expect()` for likely/unlikely hints |
| **Cache Performance** | Prefer **SoA (Structure of Arrays)** over **AoS** |
| **Testing** | Use `cmocka`, `Unity`, `Catch2` |
| **Modern Build System** | Use `CMake` over raw `Makefiles` |
| **Memory Debugging** | `valgrind`, `AddressSanitizer` (`-fsanitize=address`) |
