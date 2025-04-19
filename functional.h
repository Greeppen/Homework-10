#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_FILES 1024
#define MAX_FILENAME 256
#define MAX_PATH 4096

typedef struct {
    char name[MAX_FILENAME];
    bool is_dir;
} FileEntry;

typedef struct {
    char current_path_left[MAX_PATH];
    char current_path_right[MAX_PATH];
    FileEntry files_left[MAX_FILES];
    FileEntry files_right[MAX_FILES];
    int file_count_left;
    int file_count_right;
    int selected_left;
    int selected_right;
    bool active_panel;
} FileManager;

void list_directory(FileManager *fm, bool is_left);
void change_directory(FileManager *fm, const char *new_dir, bool is_left);
void go_parent_directory(FileManager *fm, bool is_left);
bool is_executable(const char *path);
bool is_regular_file(const char *path);
void execute_file(FileManager *fm, bool is_left);

#endif
