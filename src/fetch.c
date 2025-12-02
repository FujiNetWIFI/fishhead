/**
 * @brief   fishhead - Display Fish-eye server health
 * @author  Thom Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-network.h"

/**
 * @brief URL to the fish eye server
 */
const char *devicespec = "N:HTTP://fisheye.diller.org/health";

/**
 * @brief JSON path for status
 */
const char *query_status = "/status";

/**
 * @brief JSON path for uptime
 */
const char *query_uptime = "/uptime";

/**
 * @brief JSON path for timestamp
 */
const char *query_timestamp = "/timestamp";

/**
 * @brief JSON path for version
 */
const char *query_version = "/version";

/**
 * @brief JSON path for database status
 */
const char *query_database_status = "/database/status";

/**
 * @brief JSON path for file stats
 */
const char *query_database_stats_files = "/database/stats/files";

/**
 * @brief JSON path for client stats
 */
const char *query_database_stats_clients = "/database/stats/clients";

/**
 * @brief JSON path for server stats
 */
const char *query_database_stats_servers = "/database/stats/servers";

/**
 * @brief fetch the stats from server
 * @param status ptr to status string
 * @param uptime ptr to uptime int
 * @param timestamp ptr to ISO-8601 timestamp string
 * @param database_status ptr to database_status string
 * @param database_stats_files ptr to database_stats_files ulong
 * @param database_stats_clients ptr to database_stats_clients int
 * @param database_stats_servers ptr to database_stats_serevrs int
 * @return true if fetch successful, otherwise false.
 */
bool fetch(char *status,
           unsigned long *uptime,
           char *timestamp,
           char *version,
           char *database_status,
           unsigned long *database_stats_files,
           int *database_stats_clients,
           int *database_stats_servers)
{
    unsigned char r=false; // Error until proven otherwise.
    char tmp[80];

    memset(tmp,0,sizeof(tmp));

    network_init();

    if (network_open(devicespec, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE) != FN_ERR_OK)
        goto bye;

    if (network_json_parse(devicespec) != FN_ERR_OK)
        goto bye;

    if (network_json_query(devicespec, query_status, status) < 0)
        goto bye;

    if (network_json_query(devicespec, query_uptime, tmp) < 0)
        goto bye;
    else
        *uptime = atol(tmp);

    if (network_json_query(devicespec, query_timestamp, timestamp) < 0)
        goto bye;

    if (network_json_query(devicespec, query_version, version) < 0)
        goto bye;

    if (network_json_query(devicespec, query_database_status, database_status) < 0)
        goto bye;

    if (network_json_query(devicespec, query_database_stats_files, tmp) < 0)
        goto bye;
    else
        *database_stats_files = atol(tmp);

    if (network_json_query(devicespec, query_database_stats_clients, tmp) < 0)
        goto bye;
    else
        *database_stats_clients = atoi(tmp);

    if (network_json_query(devicespec, query_database_stats_servers, tmp) < 0)
        goto bye;
    else
        *database_stats_servers = atoi(tmp);

    r = true;

 bye:
    network_close(devicespec);

    return r;
}
