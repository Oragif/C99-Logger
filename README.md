# Logger - C99
## Enable Logger
Sets where logger should output
```
log_start(int to_console, int to_file, int date_file);
```
## Logging functions
### Info logging to console and file
Output: '[12:30:00] [TAG/file:line] Text'
```
TAG: INFO - log_info(const char *format, ...)
TAG: ERROR - log_error(char *format, ...)
TAG: WARN - log_warn(const char *format, ...)
TAG: Custom - log_custom(TAG, const char *format, ...)
```

## Utility functions
### Catch NULL
Checks if any val is NULL and logs error if NULL
```
catch_null(void* val)
```
### Print int with min length
Input ex.: val - 1, min_length - 4\
Output: '0001'
```
print_format_int(STREAM stream, int val, int min_length);
```
### Print current time
Output: '12:30:00' - H:M:S
```
print_time(STREAM stream);
```
