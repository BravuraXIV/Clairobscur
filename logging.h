#ifndef BRAVURA_EMU_LOGGING_H
#define BRAVURA_EMU_LOGGING_H

#define bravura_log(format, ...) printf("[info] %s.%d: "format"\n", __FILE__, __LINE__, __VA_ARGS__);
#define bravura_log_err(format, ...) fprintf(stderr, "[error] %s.%d: "format"\n", __FILE__, __LINE__, __VA_ARGS__);

#endif // BRAVURA_EMU_LOGGING_H
