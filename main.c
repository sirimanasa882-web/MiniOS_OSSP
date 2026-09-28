#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

void print_menu() {
    printf("\n=== Linux Memory Mapped File Manager ===\n");
    printf("1. View File Contents (Read via Pointer)\n");
    printf("2. Modify Data at Specific Offset (Write/Mutate)\n");
    printf("3. Synchronize Memory Changes to Disk (msync)\n");
    printf("4. Unmap File & Exit\n");
    printf("Enter choice: ");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <filename> <mode: shared/private>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char *mode_str = argv[2];
    int mapping_flags = 0;

    // Determine mapping mode (MAP_SHARED vs MAP_PRIVATE)
    if (strcmp(mode_str, "shared") == 0) {
        mapping_flags = MAP_SHARED;
    } else if (strcmp(mode_str, "private") == 0) {
        mapping_flags = MAP_PRIVATE;
    } else {
        printf("Invalid mode! Use 'shared' or 'private'.\n");
        return 1;
    }

    // Phase 1: Open Target Data File & Get Size Details
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0) {
        perror("Error getting file stats");
        close(fd);
        return 1;
    }
    size_t file_size = sb.st_size;

    if (file_size == 0) {
        printf("Error: File size is 0 bytes. Cannot map an empty file.\n");
        close(fd);
        return 1;
    }

    // Phase 2: Virtual Memory Mapping Allocation (mmap)
    char *map_ptr = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, mapping_flags, fd, 0);
    if (map_ptr == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    // File descriptor can be closed immediately after successful mapping
    close(fd); 

    printf("\n[SUCCESS] File '%s' mapped into virtual address space.\n", filename);
    printf("Mapping Address: %p | File Size: %zu bytes\n", (void*)map_ptr, file_size);

    int choice;
    char input_buf[256];
    size_t offset;

    // Runtime Management Loop
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) break;

        if (choice == 1) {
            // Phase 3: Zero-Copy Reading via Pointer Traversal
            printf("\n--- File Content Output ---\n");
            for (size_t i = 0; i < file_size; i++) {
                putchar(map_ptr[i]);
            }
            printf("\n---------------------------\n");
        } 
        else if (choice == 2) {
            // Phase 3: Zero-Copy In-Memory Mutation
            printf("Enter memory offset to overwrite (0 to %zu): ", file_size - 1);
            scanf("%zu", &offset);
            if (offset >= file_size) {
                printf("Error: Offset out of bounds.\n");
                continue;
            }
            printf("Enter text to write at offset: ");
            scanf(" %[^\n]", input_buf); // Reads string with spaces

            size_t input_len = strlen(input_buf);
            if (offset + input_len > file_size) {
                printf("Warning: String truncated to fit remaining file capacity.\n");
                input_len = file_size - offset;
            }

            // Direct in-memory modification using pointer logic
            memcpy(map_ptr + offset, input_buf, input_len);
            printf("Memory successfully mutated directly via address manipulation.\n");
        } 
        else if (choice == 3) {
            // Phase 4: Explicit Cache-to-Disk Sync (msync)
            if (msync(map_ptr, file_size, MS_SYNC) == 0) {
                printf("[SUCCESS] Memory modifications flushed to disk via msync().\n");
            } else {
                perror("msync failed");
            }
        } 
        else if (choice == 4) {
            break;
        } 
        else {
            printf("Invalid selection. Try again.\n");
        }
    }

    // Phase 5: Resource Virtual Unmapping (munmap)
    printf("\nReleasing runtime resources...\n");
    if (munmap(map_ptr, file_size) < 0) {
        perror("munmap failed");
    } else {
        printf("[SUCCESS] Virtual memory segment cleanly unmapped.\n");
    }

    return 0;
}
