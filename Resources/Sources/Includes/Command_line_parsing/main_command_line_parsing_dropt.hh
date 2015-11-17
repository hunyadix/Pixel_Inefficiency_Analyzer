#include "dropt-1.1.1/include/droptxx.hpp"

int parse_main_command_line_arguments( int argc, char** argv, std :: string& input_file, std :: string& output_file )
{
    static dropt_bool showHelp = 0;
    static dropt_bool showVersion = 0;

    int exitCode = EXIT_SUCCESS;

    /* Each option is defined by a row in a table, containing properties
     * such as the option's short name (e.g. -h), its long name (e.g.
     * --help), its help text, its handler callback, and its callback data
     * (for typical handlers, this data is usually the address of a variable
     * for the handler to modify).
     *
     * See the dropt_option documentation in dropt.h for a complete list
     * of option properties.
     */
    dropt_option options[] = {
        { 'h',  "help", "Shows help.", NULL, dropt_handle_bool, &showHelp, dropt_attr_halt },
        { '?', NULL, NULL, NULL, dropt_handle_bool, &showHelp, dropt_attr_halt | dropt_attr_hidden },
        { '\0', "version", "Shows version information.", NULL, dropt_handle_bool, &showVersion, dropt_attr_halt },
        { 'i',  "input-file",  "Set input path.",  "value", dropt_handle_string, &input_file },
        { 'o',  "output-file", "Set output path.", "value", dropt_handle_string, &output_file },
        { 0 } /* Required sentinel value. */
    };

    dropt_context* droptContext = dropt_new_context(options);
    if (droptContext == NULL)
    {
        /* We failed to create the dropt context, possibly due to memory
         * allocation failure.
         *
         * This also can happen due to logical errors (e.g. if the options
         * array is malformed).  Logical errors will trigger DROPT_MISUSE()
         * and will terminate the program in debug builds.
         */
        exitCode = EXIT_FAILURE;
    }
    else if (argc == 0)
    {
        /* This check is useless but is here for pedantic completeness.
         * Hosted C environments are not required to supply command-line
         * arguments, although obviously any environment that doesn't
         * supply arguments wouldn't have any use for dropt.
         */
    }
    else
    {
        /* Parse the arguments from argv.
         *
         * argv[1] is always safe to access since argv[argc] is guaranteed
         * to be NULL and since we've established that argc > 0.
         */
        char** rest = dropt_parse(droptContext, -1, &argv[1]);
        if(dropt_get_error(droptContext) != dropt_error_none)
        {
            fprintf(stderr, "dropt_example: %s\n", dropt_get_error_message(droptContext));
            exitCode = EXIT_FAILURE;
        }
        else if( showHelp )
        {
            printf( "Usage: %s [options] [--] [operands]\n\n"
                    "Options:\n", argv[0] );
            dropt_print_help( stdout, droptContext, NULL );
        }
        /*else if (showVersion)
        {
            printf("dropt_example 1.0\n");
        }*/
        else
        {
            printf("Input_file_name value: %s\n", input_file.c_str() );
            printf("Output_file_name value: %s\n", output_file.c_str() );

            printf("Operands: ");
            while (*rest != NULL)
            {
                printf("%s ", *rest);
                rest++;
            }
            printf("\n");
        }
    }

    dropt_free_context(droptContext);

    return exitCode;
}
