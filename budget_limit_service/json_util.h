#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple JSON string value extraction
// Format: "key": "value" or "key": value
char* json_get_string_value(const char* json, const char* key);
float json_get_float_value(const char* json, const char* key);

// Simple JSON creation
char* json_create_response(const char* status, float percentage, const char* message);

#endif
