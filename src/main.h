#ifndef MAIN_H
#define MAIN_H

/**
 * @brief called when we can't fetch
 */
void cant_fetch(void);

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
unsigned char fetch(char *status,
           unsigned long *uptime,
           char *timestamp,
           char *version,
           char *database_status,
           unsigned long *database_stats_files,
           int *database_stats_clients,
           int *database_stats_servers);

/**
 * @brief display the stats from server
 * @param status ptr to status string
 * @param uptime ptr to uptime int
 * @param timestamp ptr to ISO-8601 timestamp string
 * @param database_status ptr to database_status string
 * @param database_stats_files ptr to database_stats_files ulong
 * @param database_stats_clients ptr to database_stats_clients int
 * @param database_stats_servers ptr to database_stats_serevrs int
 */
void display(char *status,
             unsigned long uptime,
             char *timestamp,
             char *version,
             char *database_status,
             unsigned long database_stats_files,
             int database_stats_clients,
             int database_stats_servers);

#endif // MAIN_H
