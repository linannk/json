#ifndef JSONDEF_H
#define JSONDEF_H

#define JSON_NAMESPACE json
#define BEGIN_JSON_NAMESPACE namespace JSON_NAMESPACE {
#define END_JSON_NAMESPACE }

#define JSON_TAB_OFFSET 2

#define JSON_COMMA ','
#define JSON_QUOTE '\"'
#define JSON_OPENBRACE '{'
#define JSON_CLOSEBRACE '}'
#define JSON_OPENBRACKET '['
#define JSON_CLOSEBRACKET ']'
#define JSON_POSITIVE_OPERATOR '+'
#define JSON_NEGATIVE_OPERATOR '-'
#define JSON_SCIENTIFIC_E_UPPER 'E'
#define JSON_SCIENTIFIC_E_LOWER 'e'
#define JSON_FLOAT_F_UPPER 'F'
#define JSON_FLOAT_F_LOWER 'f'

#if defined(_DEBUG) || defined(DEBUG)
#define JSON_DEBUG
#endif

#endif // JSONDEF_H
