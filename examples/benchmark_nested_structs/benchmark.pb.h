/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.2-dev */

#ifndef PB_BENCHMARK_PB_H_INCLUDED
#define PB_BENCHMARK_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _CppCommand {
    char command[128];
} CppCommand;

typedef struct _attribute_msg {
    bool a1;
    float a2;
} attribute_msg;

typedef struct _Command {
    int32_t version;
    char cmdkey[16];
    bool has_attributes;
    attribute_msg attributes;
} Command;


/* Initializer values for message structs */
#define attribute_msg_init_default               {0, 0}
#define Command_init_default                     {0, "", false, attribute_msg_init_default}
#define CppCommand_init_default                  {""}
#define attribute_msg_init_zero                  {0, 0}
#define Command_init_zero                        {0, "", false, attribute_msg_init_zero}
#define CppCommand_init_zero                     {""}

/* Field tags (for use in manual encoding/decoding) */
#define CppCommand_command_tag                   1
#define attribute_msg_a1_tag                     1
#define attribute_msg_a2_tag                     2
#define Command_version_tag                      1
#define Command_cmdkey_tag                       2
#define Command_attributes_tag                   3

/* Struct field encoding specification for nanopb */
#define attribute_msg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     a1,                1) \
X(a, STATIC,   SINGULAR, FLOAT,    a2,                2)
#define attribute_msg_CALLBACK NULL
#define attribute_msg_DEFAULT NULL

#define Command_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    version,           1) \
X(a, STATIC,   SINGULAR, STRING,   cmdkey,            2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  attributes,        3)
#define Command_CALLBACK NULL
#define Command_DEFAULT NULL
#define Command_attributes_MSGTYPE attribute_msg

#define CppCommand_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   command,           1)
#define CppCommand_CALLBACK NULL
#define CppCommand_DEFAULT NULL

extern const pb_msgdesc_t attribute_msg_msg;
extern const pb_msgdesc_t Command_msg;
extern const pb_msgdesc_t CppCommand_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define attribute_msg_fields &attribute_msg_msg
#define Command_fields &Command_msg
#define CppCommand_fields &CppCommand_msg

/* Maximum encoded size of messages (where known) */
#define attribute_msg_size                       7
#define Command_size                             37
#define CppCommand_size                          130

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
