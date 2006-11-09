#ifndef NICKSERV_H
#define NICKSERV_H

struct Nick
{
  dlink_node node;
  
  unsigned int id;
  char nick[NICKLEN+1];
  char pass[PASSLEN+1];
  char *email;
  char *url;
  char cloak[HOSTLEN+1];
  unsigned int status;
  unsigned int flags;
  unsigned int language;
  time_t reg_time;
  time_t last_seen;
  time_t last_used;
  time_t last_quit_time;
};

/* Nick flags */
#define NS_FLAG_ADMIN     0x1

#define IsServAdmin(x)    (x)->nickname->flags & NS_FLAG_ADMIN

/* Language defines */

#define NS_ALREADY_REG      1
#define NS_REG_COMPLETE     2
#define NS_REG_FAIL         3
#define NS_REG_FIRST        4
#define NS_IDENTIFIED       5
#define NS_IDENT_FAIL       6
#define NS_HELP_SHORT       7
#define NS_HELP_LONG        8
#define NS_HELP_REG_SHORT   9
#define NS_HELP_REG_LONG    10
#define NS_HELP_ID_SHORT    11
#define NS_HELP_ID_LONG     12
#define NS_CURR_LANGUAGE    13
#define NS_AVAIL_LANGUAGE   14
#define NS_LANGUAGE_SET     15
#define NS_SET_SUCCESS      16
#define NS_SET_FAILED       17
#define NS_SET_VALUE        18
#define NS_HELP_DROP_SHORT  19
#define NS_HELP_DROP_LONG   20
#define NS_NEED_IDENTIFY    21
#define NS_NICK_DROPPED     22
#define NS_NICK_DROPFAIL    23
#define NS_ACCESS_HELP_S    24
#define NS_ACCESS_HELP_LONG 25
#define NS_ACCESS_ADD       26 
#define NS_ACCESS_ADDFAIL   27
#define NS_ACCESS_INVALID   28
#define NS_ACCESS_START     29
#define NS_ACCESS_ENTRY     30
#define NS_ACCESS_DEL       31
#define NS_HELP_GHOST_SHORT 32
#define NS_HELP_GHOST_LONG  33
#define NS_GHOST_SUCCESS    34
#define NS_GHOST_FAILED     35
#define NS_GHOST_NOSELF     36
#define NS_HELP_LINK_SHORT  37
#define NS_HELP_LINK_LONG   38
#define NS_LINK_NOMASTER    39
#define NS_LINK_BADPASS     40
#define NS_LINK_FAIL        41
#define NS_LINK_OK          42

#endif
