#include <string.h>
#include <stdbool.h>

#include "command_parser.h"
#include "uart_driver.h"

#define CMD_BUF_SIZE 64

static char cmd_buf[CMD_BUF_SIZE];
static uint8_t cmd_pos;
static bool echo_enabled = true;

static void process_command(const char *cmd)
{
    if (strcmp(cmd, "help") == 0) {
        uart_driver_send_string("\r\nCommands:\r\n");
        uart_driver_send_string("  help\r\n");
        uart_driver_send_string("  status\r\n");
        uart_driver_send_string("  echo on\r\n");
        uart_driver_send_string("  echo off\r\n");
    } else if (strcmp(cmd, "status") == 0) {
        uart_driver_send_string("\r\nSystem OK\r\n");
    } else if (strcmp(cmd, "echo on") == 0) {
        echo_enabled = true;
        uart_driver_send_string("\r\nEcho enabled\r\n");
    } else if (strcmp(cmd, "echo off") == 0) {
        echo_enabled = false;
        uart_driver_send_string("\r\nEcho disabled\r\n");
    } else if (cmd[0] != '\0') {
        uart_driver_send_string("\r\nUnknown command\r\n");
    }

    uart_driver_send_string("> ");
}

void command_parser_init(void)
{
    cmd_pos = 0;
    memset(cmd_buf, 0, sizeof(cmd_buf));

    uart_driver_send_string("\r\nCommand parser started\r\n");
    uart_driver_send_string("Type 'help'\r\n");
    uart_driver_send_string("> ");
}

void command_parser_process_char(char ch)
{
    if (echo_enabled) {
        uart_driver_send_char((uint8_t)ch);
    }

    if (ch == '\r' || ch == '\n') {
        cmd_buf[cmd_pos] = '\0';
        process_command(cmd_buf);
        cmd_pos = 0;
        memset(cmd_buf, 0, sizeof(cmd_buf));
        return;
    }

    if (ch == '\b' || ch == 127) {
        if (cmd_pos > 0) {
            cmd_pos--;
            cmd_buf[cmd_pos] = '\0';
        }
        return;
    }

    if (cmd_pos < CMD_BUF_SIZE - 1) {
        cmd_buf[cmd_pos++] = ch;
    } else {
        uart_driver_send_string("\r\nCommand too long\r\n");
        cmd_pos = 0;
        memset(cmd_buf, 0, sizeof(cmd_buf));
        uart_driver_send_string("> ");
    }
} 