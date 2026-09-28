# Linux Memory Mapped File Management System
An Efficient Approach to Virtual Memory-Based File Access and I/O Optimization.

## 👥 Section & Team Details
* **Course:** Operating Systems (25CS2104E) — 2026–27, Term-I
* **Institution:** Koneru Lakshmaiah Education Foundation (KLH University), Hyderabad
* **Section:** 01 | **Team:** 16
* **Team Members:**
  * Siri Manasa (Roll No: 2520030563) — *Module design, mmap/munmap, address handling.*
  * Kavya Sree (Roll No: 2520030564) — *Memory mutations, msync synchronization, mapping logic.*

---

## 📑 Project Abstract
This project implements a command-line tool in C that maps a target file directly onto a process's virtual memory landscape using POSIX/Linux system APIs (`mmap`, `msync`, `munmap`). By bypassing standard `read()` and `write()` boundaries, the system achieves zero-copy memory pointer mutation, eliminates continuous context switches, and demonstrates key core OS mechanics like demand paging, the page cache, and Copy-On-Write (COW) behaviors.

---

## 🚀 How to Run the Implementation on Ubuntu

### 1. Create a Dummy Target File
```bash
echo "Operating Systems Project: Memory Mapped Architecture." > test.txt
```

### 2. Compile the Source Code
```bash
gcc main.c -o mmap_manager
```

### 3. Execute (Choose Shared or Private Mode)
* **To test Shared Mode (Changes sync back to disk):**
  ```bash
  ./mmap_manager test.txt shared
  ```
* **To test Private Mode (Copy-on-write, changes remain volatile in memory):**
  ```bash
  ./mmap_manager test.txt private
  ```

---

## 🖥️ Linux APIs & System Calls Demonstrated
* `mmap()`: Map physical storage blocks into virtual memory space.
* `munmap()`: Cleanly release mapped memory allocations.
* `msync()`: Flush dirty memory cache fields directly to the disk.
* `fstat()` / `open()`: Target file handling and attribute profiling.
