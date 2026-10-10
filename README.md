# Midterm Project - Implement ls(1)

## 1. Project Overview

This project implements a simplified version of the UNIX `ls(1)` command using the C programming language.

The program lists files and directories, displays file metadata, supports sorting and formatting options, and provides recursive directory traversal. The implementation is based on the provided `ls(1)` manual page.

The project is organized into multiple source files and header files to improve readability, maintainability, and code reuse.

## 2. Options Reference and Explanations

The program implements the following command-line options based on the provided manual page:

| Option | Description |
| :--- | :--- |
| `-A` | List all entries except `.` and `..`. |
| `-a` | Include hidden files and directories. |
| `-c` | Use file status change time instead of modification time. |
| `-d` | List directories as ordinary entries instead of listing their contents. |
| `-F` | Append indicators for file types, such as `/` for directories and `*` for executable files. |
| `-f` | Disable sorting and include hidden entries. |
| `-h` | Display sizes in a human-readable format when supported. |
| `-i` | Display the inode number of each entry. |
| `-k` | Display block counts in kilobytes when used with `-s`. |
| `-l` | Display detailed file information, including permissions, links, owner, group, size, date, and name. |
| `-n` | Use numeric user and group IDs in long format. |
| `-q` | Replace non-printable filename characters with `?`. |
| `-R` | Recursively list subdirectories. |
| `-r` | Reverse the sorting order. |
| `-S` | Sort entries by file size, largest first. |
| `-s` | Display the number of allocated filesystem blocks. |
| `-t` | Sort entries by modification time, newest first. |
| `-u` | Use the last access time instead of modification time. |
| `-w` | Display raw non-printable filename characters. |

Options that affect the same behavior are handled according to the program's option-parsing logic.

## 3. Technical Architecture and System Calls

### Filesystem Operations

The project uses UNIX filesystem interfaces to read directory contents and retrieve file metadata:

- `opendir()` opens a directory.
- `readdir()` reads directory entries.
- `closedir()` closes an opened directory.
- `lstat()` retrieves metadata without following symbolic links.
- `stat()` retrieves file metadata while following symbolic links.
- `getpwuid()` retrieves user information.
- `getgrgid()` retrieves group information.
- `getopt()` processes command-line options.

### Memory Management

Dynamic memory allocation is used to store file names, paths, and directory entries. The program provides wrapper functions such as `xmalloc()`, `xrealloc()`, and `xstrdup()` to simplify memory allocation and handle allocation failures.

### Sorting and Formatting

The program supports sorting by filename, file size, and timestamps, as well as reverse ordering and unsorted output.

Formatting functions handle file permissions, timestamps, file sizes, block counts, inode numbers, and file-type indicators.

## 4. Project Structure

```text
.
├── Makefile
├── README.md
├── .gitignore
├── include/
│   ├── entry.h
│   ├── format.h
│   ├── list.h
│   ├── options.h
│   ├── print.h
│   ├── sort.h
│   └── utils.h
└── src/
    ├── entry.c
    ├── options.c
    ├── list.c
    ├── utils.c
    ├── sort.c
    ├── format.c
    ├── print.c
    └── main.c
```

### Main Components

- `src/main.c`: controls program execution, classifies operands, and traverses directories (including `-R`).
- `src/options.c`: initializes and parses command-line options using `getopt()`.
- `src/entry.c`: creates file entries and retrieves file metadata with `lstat()`.
- `src/list.c`: reads directory contents and filters entries according to the options.
- `src/sort.c`: sorts by name, file size, or time, and supports reverse and unsorted output.
- `src/format.c`: formats permissions, file sizes, block counts, and timestamps.
- `src/print.c`: prints entries, inode numbers, and the long format.
- `src/utils.c`: provides shared memory allocation and string duplication helpers.
- `include/`: header files defining shared data structures and function interfaces.
- `Makefile`: automates compilation, dependency tracking, and cleanup.
- `.gitignore`: excludes binaries, object files, dependency files, and editor temporary files from Git.

## 5. Requirements

- A UNIX-compatible operating system. The project was developed and tested on NetBSD 10.1.
- A C compiler such as `cc`.
- The `make` build tool.

## 6. Compilation

Run the following commands from the project root directory:

```bash
make clean
make
```

After a successful build, the `my_ls` executable is created in the project root directory.

## 7. Usage and Examples

General syntax:

```bash
./my_ls [-AacdFfhiklnqRrSstuw] [file ...]
```

Examples:

```bash
./my_ls
./my_ls -a
./my_ls -A
./my_ls -l
./my_ls -i
./my_ls -s
./my_ls -sh
./my_ls -lh
./my_ls -S
./my_ls -tr
./my_ls -R
./my_ls -d .
./my_ls -l /tmp
```
