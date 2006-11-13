/*
 *  oftc-ircservices: an exstensible and flexible IRC Services package
 *  oftc.c: A protocol handler for the OFTC IRC Network
 *
 *  Copyright (C) 2006 Stuart Walsh and the OFTC Coding department
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *  USA
 *
 *  $Id$
 */

#include "stdinc.h"

static void *oftc_server_connected(va_list);
static void *oftc_sendmsg_gnotice(va_list);
static void *oftc_sendmsg_svsmode(va_list);
static void *oftc_sendmsg_svscloak(va_list);
static void *oftc_sendmsg_svsnick(va_list);
//static void *oftc_sendmsg_svsjoin(va_list);
static void *oftc_identify(va_list);

static dlink_node *oftc_gnotice_hook;
static dlink_node *oftc_umode_hook;
static dlink_node *oftc_svscloak_hook;
static dlink_node *oftc_svsnick_hook;
//static dlink_node *oftc_svsjoin_hook;
static dlink_node *oftc_identify_hook;
static dlink_node *oftc_connected_hook;

static void m_pass(struct Client *, struct Client *, int, char *[]);
static void m_server(struct Client *, struct Client *, int, char *[]);
static void m_sid(struct Client *, struct Client *, int, char *[]);
static void m_uid(struct Client *, struct Client *, int, char *[]);

struct Message gnotice_msgtab = {
  "GNOTICE", 0, 0, 3, 0, 0, 0,
  { m_ignore, m_ignore }
};

struct Message pass_msgtab = {
  "PASS", 0, 0, 4, 0, 0, 0,
  { m_pass, m_pass }
};

struct Message server_msgtab = {
  "SERVER", 0, 0, 3, 0, 0, 0,
  { m_server, m_server }
};

struct Message sid_msgtab = {
  "SID", 0, 0, 5, 0, 0, 0,
  { m_sid, m_sid }
};

struct Message uid_msgtab = {
  "UID", 0, 0, 10, 0, 0, 0,
  { m_uid, m_uid }
};

INIT_MODULE(oftc, "$Revision$")
{
  oftc_connected_hook  = install_hook(connected_cb, oftc_server_connected);
  oftc_gnotice_hook   = install_hook(send_gnotice_cb, oftc_sendmsg_gnotice);
  oftc_umode_hook     = install_hook(send_umode_cb, oftc_sendmsg_svsmode);
  oftc_svscloak_hook  = install_hook(send_cloak_cb, oftc_sendmsg_svscloak);
//  oftc_svsjoin_hook   = install_hook(send_nick_cb, oftc_sendmsg_svsjoin);
  oftc_svsnick_hook   = install_hook(send_nick_cb, oftc_sendmsg_svsnick);
  oftc_identify_hook  = install_hook(on_identify_cb, oftc_identify); 
  mod_add_cmd(&gnotice_msgtab);
  mod_add_cmd(&pass_msgtab);
  mod_add_cmd(&server_msgtab);
  mod_add_cmd(&sid_msgtab);
  mod_add_cmd(&uid_msgtab);
}

CLEANUP_MODULE
{
  mod_del_cmd(&gnotice_msgtab);
  mod_del_cmd(&pass_msgtab);
  mod_del_cmd(&server_msgtab);
  mod_del_cmd(&sid_msgtab);
  mod_del_cmd(&uid_msgtab);

  uninstall_hook(send_gnotice_cb, oftc_sendmsg_gnotice);
  uninstall_hook(send_umode_cb, oftc_sendmsg_svsmode);
  uninstall_hook(send_cloak_cb, oftc_sendmsg_svscloak);
  uninstall_hook(on_identify_cb, oftc_identify);
  uninstall_hook(connected_cb, oftc_server_connected);
}

/*
 * client_from_server()
 */
static void
client_from_server(struct Client *client_p, struct Client *source_p, int parc,
                   char *parv[], time_t newts, char *nick, char *ugecos)
{
  const char *m = NULL;
  const char *servername = source_p->name;
  unsigned int flag = 0;

  source_p = make_client(client_p);
  dlinkAdd(source_p, &source_p->node, &global_client_list);

  source_p->hopcount = atoi(parv[2]);
  source_p->tsinfo = newts;

  /* copy the nick in place */
  strcpy(source_p->name, nick);
  strlcpy(source_p->id, parv[8], sizeof(source_p->id));
  strlcpy(source_p->sockhost, parv[7], sizeof(source_p->sockhost));

  hash_add_client(source_p);
  hash_add_id(source_p);

  /* parse usermodes */
  for (m = &parv[4][1]; *m; ++m)
  {
    flag = user_modes[(unsigned char)*m];
    source_p->umodes |= flag;
  }

  register_remote_user(client_p, source_p, parv[5], parv[6],
                       servername, ugecos);
}

static void 
m_pass(struct Client *client, struct Client *source, int parc, char *parv[])
{
  strlcpy(client->id, parv[4], sizeof(client->id));
}

static void
m_server(struct Client *client, struct Client *source, int parc, char *parv[])
{
  if(IsConnecting(client))
  {
    sendto_server(client, "SVINFO 5 5 0: %lu", CurrentTime);
    //irc_sendmsg_ping(client, me.name, me.name);
    SetServer(client);
    hash_add_client(client);
    hash_add_id(client);
    global_notice(NULL, "Completed server connection to %s %s", client->name,
        client->id);
    ClearConnecting(client);
    client->servptr = &me;
    dlinkAdd(client, &client->lnode, &me.server_list);
  }
  else
  {
    struct Client *newclient = make_client(client);

    strlcpy(newclient->name, parv[1], sizeof(newclient->name));
    strlcpy(newclient->info, parv[3], sizeof(newclient->info));
    newclient->hopcount = atoi(parv[2]);
    SetServer(newclient);
    dlinkAdd(newclient, &newclient->node, &global_client_list);
    hash_add_client(newclient);
    newclient->servptr = source;
    dlinkAdd(newclient, &newclient->lnode, &newclient->servptr->server_list);
    printf("Got TS5 server %s from TS6 hub %s\n", parv[1], source->name);
  }
}

static void
m_sid(struct Client *client, struct Client *source, int parc, char *parv[])
{
  struct Client *newclient = make_client(client);

  strlcpy(newclient->name, parv[1], sizeof(newclient->name));
  strlcpy(newclient->id, parv[3], sizeof(newclient->id));
  strlcpy(newclient->info, parv[4], sizeof(newclient->info));
  newclient->hopcount = atoi(parv[2]);
  SetServer(newclient);
  dlinkAdd(newclient, &newclient->node, &global_client_list);
  hash_add_client(newclient);
  hash_add_id(newclient);
  newclient->servptr = source;
  dlinkAdd(newclient, &newclient->lnode, &newclient->servptr->server_list);
  printf("Got server %s(%s) from hub %s(%s)\n", parv[1], parv[3], 
      source->name, source->id);
}

/*
 * m_uid()
 *
 * server introducing new nick
 *    parv[0] = sender prefix
 *    parv[1] = nickname
 *    parv[2] = hop count
 *    parv[3] = TS
 *    parv[4] = umode
 *    parv[5] = username
 *    parv[6] = hostname
 *    parv[7] = ip
 *    parv[8] = uid
 *    parv[9] = ircname
 */
static void
m_uid(struct Client *client_p, struct Client *source_p,
        int parc, char *parv[])
{
  struct Client *target_p;
  char nick[NICKLEN];
  char ugecos[REALLEN + 1];
  time_t newts = 0;
  char *unick = parv[1];
  char *uts = parv[3];
  char *uname = parv[5];
  char *uhost = parv[6];

  if (EmptyString(unick))
    return;

  /* Fix the lengths */
  strlcpy(nick, parv[1], sizeof(nick));
  strlcpy(ugecos, parv[9], sizeof(ugecos));

  if (check_clean_nick(client_p, source_p, nick, unick, source_p) ||
      check_clean_user(client_p, nick, uname, source_p) ||
      check_clean_host(client_p, nick, uhost, source_p))
    return;

  newts = atol(uts);

  if ((target_p = find_client(unick)) == NULL)
    client_from_server(client_p, source_p, parc, parv, newts, nick, ugecos);
}


static void *
oftc_server_connected(va_list args)
{
  struct Client *client = va_arg(args, struct Client *);
  dlink_node *ptr;
  
  sendto_server(client, "PASS %s TS 6 %s", client->server->pass, me.id);
  sendto_server(client, "CAPAB :KLN PARA EOB QS UNKLN GLN ENCAP TBURST CHW IE EX");
  sendto_server(client, "SERVER %s 1 : %s", me.name, me.info);
  send_queued_write(client);

  me.uplink = client;

  /* Send out our list of services loaded */
  DLINK_FOREACH(ptr, services_list.head)
  {
    struct Service *service = ptr->data;

    introduce_service(service);
  }

  return NULL;
}


static void *
oftc_identify(va_list args)
{
  struct Client *uplink = va_arg(args, struct Client *);
  struct Client *client = va_arg(args, struct Client *);

  /* XXX */
  uplink = uplink;
  
  send_umode(NULL, client, "+R");

  return pass_callback(oftc_identify_hook, uplink, client);
}

static void *
oftc_sendmsg_gnotice(va_list args)
{
  struct Client *client = va_arg(args, struct Client*);
  char          *source = va_arg(args, char *);
  char          *text   = va_arg(args, char *);
  
  // 1 is UMODE_ALL, aka UMODE_SERVERNOTICE
  sendto_server(client, ":%s GNOTICE %s 1 :%s", source, source, text);
  return pass_callback(oftc_gnotice_hook, client, source, text);
}

static void *
oftc_sendmsg_svscloak(va_list args)
{
  struct Client *client = va_arg(args, struct Client *);
  char *cloakstring = va_arg(args, char *);

  if (cloakstring != NULL) {
    sendto_server(client, ":%s SVSCLOAK %s :%s", 
      me.name, client->name, cloakstring);
  }
  
  return pass_callback(oftc_svscloak_hook, client, cloakstring);
}

static void *
oftc_sendmsg_svsmode(va_list args)
{
  struct Client *client = va_arg(args, struct Client*);
  char          *target = va_arg(args, char *);
  char          *mode   = va_arg(args, char *);

  sendto_server(client, ":%s SVSMODE %s :%s", me.name, target, mode);

  return pass_callback(oftc_umode_hook, client, target, mode);
}

static void *
oftc_sendmsg_svsnick(va_list args)
{
  struct Client *uplink = va_arg(args, struct Client *);
  struct Client *user   = va_arg(args, struct Client *);
  char          *newnick= va_arg(args, char *);

  sendto_server(uplink, ":%s SVSNICK %s :%s", me.name, user->name, newnick);
  
  return pass_callback(oftc_svsnick_hook, uplink, user, newnick);
}
