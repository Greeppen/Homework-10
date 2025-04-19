#include "functional.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void list_directory(FileManager *fm, bool is_left) {
    const char *path = is_left ? fm->current_path_left : fm->current_path_right;
    FileEntry *files = is_left ? fm->files_left : fm->files_right;
    int *count = is_left ? &fm->file_count_left : &fm->file_count_right;
    
    *count = 0;
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir failed");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && *count < MAX_FILES) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
            
        strncpy(files[*count].name, entry->d_name, MAX_FILENAME-1);
        files[*count].name[MAX_FILENAME-1] = '\0';
        files[*count].is_dir = (entry->d_type == DT_DIR);
        (*count)++;
    }
    closedir(dir);
}

void change_directory(FileManager *fm, const char *new_dir, bool is_left) {
    char *path = is_left ? fm->current_path_left : fm->current_path_right;
    char temp_path[MAX_PATH];
    
    snprintf(temp_path, sizeof(temp_path), "%s/%s", path, new_dir);
    if (chdir(temp_path) == 0) {
        getcwd(path, MAX_PATH);
        if (is_left) fm->selected_left = 0;
        else fm->selected_right = 0;
        list_directory(fm, is_left);
    } else {
        perror("chdir failed");
    }
}

void go_parent_directory(FileManager *fm, bool is_left) {
    char *path = is_left ? fm->current_path_left : fm->current_path_right;
    char parent_path[MAX_PATH];
    
    snprintf(parent_path, sizeof(parent_path), "%s/..", path);
    
    if (chdir(parent_path) == 0) {
        getcwd(path, MAX_PATH);
        if (is_left) {
            fm->selected_left = 0;
        } else {
            fm->selected_right = 0;
        }
        list_directory(fm, is_left);
    } else {
        perror("chdir to parent failed");
    }
}

bool is_executable(const char *path) {
    struct stat file_stat;
    if (stat(path, &file_stat) == -1) {
        perror("stat failed");
        return false;
    }
    return (file_stat.st_mode & S_IXUSR) || 
           (file_stat.st_mode & S_IXGRP) || 
           (file_stat.st_mode & S_IXOTH);
}

bool is_regular_file(const char *path) {
    struct stat file_stat;
    if (stat(path, &file_stat) == -1) {
        perror("stat failed");
        return false;
    }
    return S_ISREG(file_stat.st_mode);
}

void execute_file(FileManager *fm, bool is_left) {
    const char *path = is_left ? fm->current_path_left : fm->current_path_right;
    int selected = is_left ? fm->selected_left : fm->selected_right;
    FileEntry *files = is_left ? fm->files_left : fm->files_right;
    
    if (selected < 0 || selected >= (is_left ? fm->file_count_left : fm->file_count_right)) {
        printf("Invalid selection\n");
        return;
    }
    
    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, files[selected].name);
    
    if (!is_regular_file(full_path)) {
        printf("Not a regular file\n");
        return;
    }
    
    if (!is_executable(full_path)) {
        printf("File is not executable\n");
        return;
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        execl(full_path, full_path, NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("Process %d exited with status %d\n", pid, status);
    }
}
