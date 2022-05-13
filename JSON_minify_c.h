
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

// json will be processed until len or a null-char is found (-1 as len can be provided for null-terminated jsons)
// returns new_str size
// new_str will be null-terminated, if json is null-terminated
// json and new_str may be the same
// new_str can be null. if not null, it must be at least as big as the result of JSON_minify_c(json), which is always at most as large as json
size_t JSON_minify_c(char* json, size_t len, char* new_str);

#ifdef __cplusplus
}
#endif
