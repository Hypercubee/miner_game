#define NOB_IMPLEMENTATION
#include "include/nob.h"


#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

// #include <string.h>

void print_usage(FILE* stream, const char* program_name) {
    fprintf(stream, "usage: %s <platform> [run]\n", program_name);
}

#define TRY(fun, stream, msg) if(!fun()) {fprintf(stream, msg);}

Nob_Cmd cmd = {0};

bool build_linux() {
    nob_cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"main", SRC_FOLDER"main.c");
    nob_cmd_append(&cmd, "-L./lib/linux", "-lraylib", "-lm", "-I./include");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-fsanitize=undefined", "-ggdb");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return false;
    return true;
}

bool build_windows() {
    nob_cmd_append(&cmd, "x86_64-w64-mingw32-gcc", "-o", BUILD_FOLDER"main.exe", SRC_FOLDER"main.c");
    nob_cmd_append(&cmd, "-L./lib/win", "-lraylib", "-lm", "-I./include");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-lgdi32", "-lwinmm", "-ggdb");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return false;
    return true;
}

int main(int argc, char **argv){
    NOB_GO_REBUILD_URSELF(argc, argv);
    int argument_count = argc;
    char **arguments = argv;

    const char *program_name = nob_shift(arguments, argument_count); // ignore program name
    if(argument_count <= 0) {
        print_usage(stderr, program_name);
        return 0;
    }

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    
    const char *arg1 = nob_shift(arguments, argument_count);
    if(strcmp("windows", arg1) == 0) {
        TRY(build_windows, stderr, "could not build windows executable\n");
    } else if(strcmp("linux", arg1) == 0) {
        TRY(build_linux, stderr, "could not build linux executable\n");
    } else {
        fprintf(stderr, "unknown platform");
        print_usage(stderr, program_name);
        return 1;
    }

    if(argument_count <= 0) return 0;
    const char *arg2 = nob_shift(arguments, argument_count);
    
    if(strcmp("run", arg2) == 0){
        if(strcmp("linux", arg1) == 0){
            nob_cmd_append(&cmd, "./build/main");
        } else {
            nob_cmd_append(&cmd, "./build/main.exe");
        }
        if (!nob_cmd_run_sync_and_reset(&cmd)) return false;
    }

    return 0;
}
