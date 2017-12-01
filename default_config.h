#ifndef DEFAULT_CONFIG_H
#define DEFAULT_CONFIG_H

// TreeParser config
#define DEFAULT_VALUE_OPENING_CHAR '['
#define DEFAULT_VALUE_CLOSING_CHAR ']'
#define DEFAULT_SONS_OPENING_CHAR '{'
#define DEFAULT_SONS_CLOSING_CHAR '}'
#define DEFAULT_ALLOWED_CHAR_BETWEEN_NODES " \t\n"

// misc defines
#ifdef __unix__
# define ENDL '\n'
#else
# define ENDL '\r\n'
#endif

#endif // DEFAULT_CONFIG_H
