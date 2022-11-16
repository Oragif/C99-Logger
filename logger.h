#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H
//Setting functions

int log_enable(int enabled);

//Log functions
int logger(char* type, const char *format, ...);

#define log_info(format, ...) logger("[INFO]  ", format, __VA_ARGS__);
#define log_error(format, ...) logger("[ERROR] ", format, __VA_ARGS__);
#define log_warn(format, ...) logger("[WARN]  ", format, __VA_ARGS__);

//null catcher

int catch_null_pointer(char* file, int line, void* val);
/**
 * Returns bool depending on val being null, error logs if it is
 * @param val Input value of any type to check
 */
#define catch_null(val) catch_null_pointer(__FILE__, __LINE__, val);
#endif //LOGGER_LOGGER_H
