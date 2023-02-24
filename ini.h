#ifndef INI_H
#define INI_H

void parse_ini(char** out);
void parse_ini_section(char** out, const char* section_name);
void parse_ini_key(char* out, const char* key);

#endif // INI_H
