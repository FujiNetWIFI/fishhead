/**
 * @brief   fishhead - Display Fish-eye server health
 * @author  Thom Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 */

#include "main.h"

unsigned char running=1;

char status[16];
unsigned long uptime;
char timestamp[32];
char version[16];
char database_status[16];
unsigned long database_stats_files;
int database_stats_clients;
int database_stats_servers;

void main(void)
{
    while (running)
    {
        if (!fetch(status,
                   &uptime,
                   timestamp,
                   version,
                   database_status,
                   &database_stats_files,
                   &database_stats_clients,
                   &database_stats_servers))
        {
            cant_fetch();
            return;
        }

        display(status,
                uptime,
                timestamp,
                version,
                database_status,
                database_stats_files,
                database_stats_clients,
                database_stats_servers);
    }
}
