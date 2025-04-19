#include "graphics.h"
#include "functional.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FileManager fm;
    WINDOW *win_left, *win_right;
 
    getcwd(fm.current_path_left, sizeof(fm.current_path_left));
    strcpy(fm.current_path_right, fm.current_path_left);
    fm.active_panel = true;
    fm.selected_left = 0;
    fm.selected_right = 0;
 
    win_init(&win_left, &win_right);
    if (COLS < 80 || LINES < 24) {
        endwin();
        printf("Terminal too small! Minimum size: 80x24\n");
        return 1;
    }
 
    list_directory(&fm, true);
    list_directory(&fm, false);
 
    int ch;
    while ((ch = getch()) != 27) {
        switch(ch) {
            case 9:
                fm.active_panel = !fm.active_panel;
                break;
            case KEY_UP:
                if (fm.active_panel && fm.selected_left > 0) fm.selected_left--;
                else if (!fm.active_panel && fm.selected_right > 0) fm.selected_right--;
                break;
            case KEY_DOWN:
                if (fm.active_panel && fm.selected_left < fm.file_count_left - 1) fm.selected_left++;
                else if (!fm.active_panel && fm.selected_right < fm.file_count_right - 1) fm.selected_right++;
                break;
            case 10:
                if (fm.active_panel) {
                    if (fm.files_left[fm.selected_left].is_dir) {
                        change_directory(&fm, fm.files_left[fm.selected_left].name, true);
                    } else {
                        execute_file(&fm, true);
		    }
                } else {
                    if (fm.files_right[fm.selected_right].is_dir) {
                        change_directory(&fm, fm.files_right[fm.selected_right].name, false);
                    } else {
                        execute_file(&fm, false);
                    }
                }
                win_refresh(&fm, win_left, win_right);
                break;
            case KEY_BACKSPACE:
            case 127:
                go_parent_directory(&fm, fm.active_panel);
                win_refresh(&fm, win_left, win_right);
                break;
            case 'e': 
                execute_file(&fm, fm.active_panel);
                win_refresh(&fm, win_left, win_right);
                break;
        }
        win_edit(&fm, win_left, win_right);
    }

    endwin();
    return 0;
}
