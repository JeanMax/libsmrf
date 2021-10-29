#include <smrf.h>

/*
// here is an example on how you could update the GameState in a background thread:

static void *update_thread(void *game)
{
    while (TRUE) {
        update_game_state((GameState *)game);
        sleep(1);
    }
    return NULL;
}

static void background_update()
{
    GameState game;
    init_game_state(&game);

    //refresh the game in another thread:
    pthread_t t;
    pthread_create(&t, NULL, update_thread, &game);

    while (TRUE) {
        //make sure to lock the GameState's mutex
        pthread_mutex_lock(&game.mutex);
        if (game.level) {
            //do your magic!
        }
        pthread_mutex_unlock(&game.mutex);
    }

    pthread_exit(NULL);
}
*/

static bool main_loop(bool loop)
{
    GameState game;
    init_game_state(&game);

    bool successful_update;
    do {
        successful_update = update_game_state(&game);
        if (successful_update) {
            LOG_INFO("{"
                     "\"seed\": %d, "
                     "\"x\": %d, "
                     "\"y\": %d, "
                     "\"area_name\": \"%s\", "
                     "\"area\": (%d)"
                     "}",
                     game.player.pAct->dwMapSeed,
                     game.player.pPath->xPos,
                     game.player.pPath->yPos,
                     AREAS[game.level->dwLevelNo],
                     game.level->dwLevelNo);
        }
        if (loop) {
            sleep(1);
        }
    } while (loop);

    destroy_game_state(&game);
    return successful_update;
}

static void usage(const char *exe)
{
    LOG_INFO("Usage: %s [OPTION]...\n"
             "List information about the D2R game state.\n"
             "\n"
             "  -l, --loop    refresh info in an endless loop\n"
             "      --help    display this help and exit",
             exe);
}

int main(int argc, const char **argv)
{
    bool loop = FALSE;

    for (const char *exe = *argv++; argc > 1 && *argv; argv++) {
        if (!strcmp(*argv, "--loop") || !strcmp(*argv, "-l")) {
            loop = TRUE;
        } else if (!strcmp(*argv, "--help")) {
            usage(exe);
            return EXIT_SUCCESS;
        } else {
            usage(exe);
            return EXIT_FAILURE;
        }
    }

    if (!main_loop(loop)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
