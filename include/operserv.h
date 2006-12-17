#ifndef OPERSERV_H
#define OPERSERV_H

struct AKill
{
  unsigned int id;
  char *mask;
  char *reason;
  unsigned int setter;
  unsigned int time_set;
  unsigned int duration;
};

#define OS_HELP_SHORT 1
#define OS_HELP_LONG 2
#define OS_MOD_LIST_HELP_SHORT 3
#define OS_MOD_LIST_HELP_LONG 4
#define OS_MOD_LOAD_HELP_SHORT 5
#define OS_MOD_LOAD_HELP_LONG 6
#define OS_MOD_UNLOAD_HELP_SHORT 7
#define OS_MOD_UNLOAD_HELP_LONG 8
#define OS_MOD_RELOAD_HELP_SHORT 9
#define OS_MOD_RELOAD_HELP_LONG 10
#define OS_MOD_HELP_SHORT 11
#define OS_MOD_HELP_LONG 12
#define OS_RAW_HELP_SHORT 13
#define OS_RAW_HELP_LONG 14
#define OS_GLOBAL_HELP_SHORT 15
#define OS_GLOBAL_HELP_LONG 16
#define OS_ADMIN_HELP_SHORT 17
#define OS_ADMIN_HELP_LONG 18
#define OS_ADMIN_ADD_HELP_SHORT 19
#define OS_ADMIN_ADD_HELP_LONG 20
#define OS_ADMIN_LIST_HELP_SHORT 21
#define OS_ADMIN_LIST_HELP_LONG 22
#define OS_ADMIN_DEL_HELP_SHORT 23
#define OS_ADMIN_DEL_HELP_LONG 24
#define OS_AKILL_ADD_HELP_SHORT 25
#define OS_AKILL_ADD_HELP_LONG 26
#define OS_AKILL_LIST_HELP_SHORT 27
#define OS_AKILL_LIST_HELP_LONG 28
#define OS_AKILL_DEL_HELP_SHORT 29
#define OS_AKILL_DEL_HELP_LONG 30
#define OS_SESSION_HELP_SHORT 31
#define OS_SESSION_HELP_LONG 32
#define OS_AKILL_HELP_SHORT 33
#define OS_AKILL_HELP_LONG 34
#define OS_EXCEPTIONS_HELP_SHORT 35
#define OS_EXCEPTIONS_HELP_LONG 36
#define OS_JUPE_HELP_SHORT 37
#define OS_JUPE_HELP_LONG 38
#define OS_SET_HELP_SHORT 39
#define OS_SET_HELP_LONG 40
#define OS_SHUTDOWN_HELP_SHORT 41
#define OS_SHUTDOWN_HELP_LONG 42
#define OS_QUARENTINE_HELP_SHORT 43
#define OS_QUARENTINE_HELP_LONG 44
#define OS_ADMIN_LIST 45
#define OS_ADMIN_ADDED 46
#define OS_NICK_NOTREG 47
#define OS_ADMIN_NOTADMIN 48
#define OS_ADMIN_DEL 49
#define OS_MOD_ALREADYLOADED 50
#define OS_MOD_LOADED 51
#define OS_MOD_LOADFAIL 52
#define OS_MOD_NOTLOADED 53
#define OS_MOD_RELOADING 54
#define OS_MOD_UNLOAD 55
#define OS_AKILL_LIST 56

#endif
