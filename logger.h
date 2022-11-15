#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H
//Setting functions

int log_enable(int enabled);

//Log functions

int log_info(const char *message, ...);
int log_error(char *format, ...);
int log_warn(char *format, ...);

//null catcher

int catch_null_pointer(char* file, int line, void* val);
/**
 * Returns bool depending on val being null, error logs if it is
 * @param val Input value of any type to check
 */
#define catch_null(val) catch_null_pointer(__FILE__, __LINE__, val);
#endif //LOGGER_LOGGER_H
