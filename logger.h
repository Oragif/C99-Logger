#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H
#include <stdio.h>
typedef struct _iobuf *STREAM;

/* ====================================================================
 *                        Settings & file utility
 * ====================================================================
 */

int log_enable(int enabled);
int log_to_file(int enabled);
int set_log_file_name(char* name);

void clear_log_file();
void update_log_path();
void log_start(int to_console, int to_file, int date_file);

/* ====================================================================
 *                               Logger
 * ====================================================================
 */

//Log functions
int logger(STREAM stream, char* file_name, int line, char* type, const char *format, ...);

/**
 * Logs info to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of logger
 */
#define log_info(format, ...) logger(stdout, __FILE__, __LINE__, "INFO", format, ##__VA_ARGS__)

/**
 * Logs error to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of logger
 */
#define log_error(format, ...) logger(stdout, __FILE__, __LINE__, "ERROR", format, ##__VA_ARGS__)

/**
 * Logs warn to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of logger
 */
#define log_warn(format, ...) logger(stdout, __FILE__, __LINE__, "WARN", format, ##__VA_ARGS__)

/**
 * Logs with custom tag to console if log_enable(bool) is set to true
 * @param format String to print with formatting
 * @param ... Argument pointers for replacing the formatted string
 * @return output of logger
 */
#define log_custom(name, format, ...) logger(stdout, __FILE__, __LINE__, name, format, ##__VA_ARGS__)

/* ====================================================================
 *                              Utility
 * ====================================================================
 */
int catch_null_pointer(char* path, int line, void* val);

/**
 * Returns bool depending on val being null, error logs if it is
 * @param val Input value of any type to check
 */
#define catch_null(val) catch_null_pointer(__FILE__, __LINE__, val)

void print_format_int(STREAM stream, int val, int min_length);
void print_time(STREAM stream);
#endif //LOGGER_LOGGER_H