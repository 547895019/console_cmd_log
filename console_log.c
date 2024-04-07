/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "esp_console.h"
#include "esp_log.h"
#include "argtable3/argtable3.h"
#include "console_log.h"

static const char *TAG = "console_log";


/**
 * Static registration of this plugin is achieved by defining the plugin description
 * structure and placing it into .console_cmd_desc section.
 * The name of the section and its placement is determined by linker.lf file in 'plugins' component.
 */
static const console_cmd_plugin_desc_t __attribute__((section(".console_cmd_desc"), used)) PLUGIN = {
    .name = "console_cmd_log",
    .plugin_regd_fn = &console_cmd_log_register
};


static struct {
    struct arg_str *tag;
    struct arg_str *level;
    struct arg_end *end;
} log_args;

/**
 * @brief  A function which implements log command.
 * examples:
 * log -t * -l INFO -e uart
 * log -d uart
 */
static int log_func(int argc, char **argv)
{
    const char *level_str[6] = {"NONE", "ERR", "WARN", "INFO", "DEBUG", "VER"};


    if (arg_parse(argc, argv, (void **)&log_args) != ESP_OK) {
        arg_print_errors(stderr, log_args.end, argv[0]);
        return -1;
    }

    for (int i = 0; log_args.level->count && i < sizeof(level_str) / sizeof(char *); ++i) {
        if (!strcasecmp(level_str[i], log_args.level->sval[0])) {
            const char *tag = log_args.tag->count ? log_args.tag->sval[0] : "*";
            printf("esp_log_level_set %s %d\r\n",tag,i);
            esp_log_level_set(tag, i);
        }
    }

    return ESP_OK;
}


/**
 * @brief Registers the log command.
 *
 * @return
 *          - esp_err_t
 */
esp_err_t console_cmd_log_register(void)
{
    log_args.tag         = arg_str0("t", "tag", "<tag>", "Tag of the log entries to enable, '*' resets log level for all tags to the given value");
    log_args.level       = arg_str0("l", "level", "<level>", "Selects log level to enable (NONE, ERR, WARN, INFO, DEBUG, VER)");
    log_args.end         = arg_end(3);

    const esp_console_cmd_t cmd = {
        .command = "log",
        .help = "Set log level for given tag",
        .hint = NULL,
        .func = &log_func,
        .argtable = &log_args,
    };

    return esp_console_cmd_register(&cmd);
}
