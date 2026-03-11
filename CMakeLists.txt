idf_component_register(SRCS "console_log.c"
                    INCLUDE_DIRS "."
					REQUIRES console console_simple_init
                    WHOLE_ARCHIVE)
