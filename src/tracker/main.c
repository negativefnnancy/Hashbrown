#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <hashbrown/music/module.h>

#include "interface.h"

void print_usage (char *command) {

    printf ("Usage: %s [options]\n", command);
    printf ("Options:\n");
    printf ("\t-h                show this help\n");
    printf ("\t-l [module.hbm]  load a module from a file at startup\n");
    printf ("\t-s [module.hbm]  save the module to a file at startup\n");
}

int main (int argc, char **argv) {

    int code; /* return code */
    int opt;

    /* cli options */
    char *load_path = NULL;
    char *save_path = NULL;

    /* the tracker module to startup with */
    module_t module;

    puts ("Mmmm hashbrowns!! @v@");

    while ((opt = getopt (argc, argv, "hl:s:")) != -1) {

        switch (opt) {

            case 'l':
                if (load_path == NULL)
                    load_path = optarg;
                else {

                    fprintf (stderr, "You can only load one module at startup!\n");
                    return EXIT_FAILURE;
                }
                break;

            case 's':
                if (save_path == NULL)
                    save_path = optarg;
                else {

                    fprintf (stderr, "You can only save one module at startup!\n");
                    return EXIT_FAILURE;
                }
                break;

            case 'h':
                print_usage (*argv);
                return EXIT_SUCCESS;

            case '?':
                print_usage (*argv);
                return EXIT_FAILURE;
        }
    }

    /* process options */
    if (load_path) {

        printf ("Loading module from \"%s\"...\n", load_path);
        module_load (&module, load_path);

    } else {

        puts ("Creating new empty module...");
        module_init_empty (&module);
    }

    if (save_path) {

        printf ("Saving module to \"%s\"...\n", save_path);
        module_save (&module, save_path);
    }

    /* start the user interface */
    puts ("Starting UI!");
    code = interface_launch (&module);

    /* cleanup */
    puts ("Bye bye");
    module_deinit (&module);
    return code;
}
