#define NOB_IMPLEMENTATION
#include "include/nob.h"


#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"


int main(int argc, char **argv){
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    nob_cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"main", SRC_FOLDER"main.c");
    nob_cmd_append(&cmd, "-L./lib/linux", "-lraylib", "-lm", "-I./include");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-fsanitize=undefined", "-ggdb");

    if (!nob_cmd_run_sync(cmd)) return 1;

    return 0;
}
