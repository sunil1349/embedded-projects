#ifndef LOGGER_H
#define LOGGER_H

int logger_init(void);

void logger_info(const char *msg);
void logger_warn(const char *msg);
void logger_error(const char *msg);

#endif