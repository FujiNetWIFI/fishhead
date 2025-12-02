/**
 * @brief   fishhead - Display Fish-eye server health
 * @author  Thom Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details
 */

#include <atari.h>
#include <string.h> // memset/memcpy
#include <stdio.h>
#include <unistd.h>
#include "uptime.h"

#define DLIST_ADDR 0x0600      // Display list address
#define CHSET_ADDR 0x7C00      // Character set address
#define PF_ADDR    0x8000      // Playfield address

#define CHARS_PER_LINE        20
#define COLOR3_CHARSET_OFFSET 0x80

extern unsigned char font[1024];

/**
 * @brief Display list
 */
static const void _dlist =
    {
        DL_BLK8,
        DL_BLK8,
        DL_BLK8,
        DL_LMS(DL_CHR20x16x2),
        PF_ADDR,
        DL_CHR20x16x2,
        DL_CHR20x16x2,
        DL_CHR20x16x2,
        DL_CHR20x16x2,
        DL_CHR20x16x2,
        DL_CHR20x16x2,
        DL_CHR20x16x2,
        DL_CHR20x16x2,
        DL_CHR20x16x2,
        DL_BLK4,
        DL_CHR20x8x2,
        DL_BLK4,
        DL_CHR20x16x2,
        DL_JVB,
        DLIST_ADDR
    };

/**
 * @brief set up display list
 */
static void display_setup(void)
{
    // Set up display list
    memset((void *)DLIST_ADDR,0x00,sizeof(_dlist));
    memcpy((void *)DLIST_ADDR, &_dlist,sizeof(_dlist));
    OS.sdlst = (void *)DLIST_ADDR;

    // Set up font.
    memcpy((void *)CHSET_ADDR,&font,sizeof(font));
    OS.chbas = 0x7C;

    // Set up colors.
    OS.color0 = 0x0F;
    OS.color2 = 0xFA;
    OS.color4 = 0xD2;
}

/**
 * @brief convert ASCII to SCREEN CODE
 * @param c ASCII code to convert
 * @return resulting screen code
 */
static unsigned char display_ascii_to_screen_code(unsigned char c)
{
    char offset=0;

    if (c < 32)      offset = 64;
    else if (c < 96) offset = -32;

    return c + offset;
}

/**
 * @brief put character on screen at x,y
 * @param x Horizontal position (0-19)
 * @param y Vertical position (0-11)
 * @param c character to place (will be converted to screen code)
 */
static void display_putc(unsigned char x, unsigned char y, const char c)
{
    unsigned char *p = (unsigned char *)PF_ADDR;
    p[y*CHARS_PER_LINE+x] = display_ascii_to_screen_code(c);
}

/**
 * @brief put string s on screen at x,y
 * @param x Horizontal position (0-19)
 * @param y Vertical position (0-11)
 * @param s string to place
 */
static void display_puts(unsigned char x, unsigned char y, const char *s)
{
    char c=0;

    while (c = *s++)
    {
        display_putc(x++,y,c);
    }
}

/**
 * @brief put string s on screen at x,y using color1
 * @param x Horizontal position (0-19)
 * @param y Vertical position (0-11)
 * @param s string to place
 */
static void display_puts_color1(unsigned char x, unsigned char y, const char *s)
{
    char c=0;

    while (c = *s++)
    {
        c += 0x40;

        if (c == 'n')
            c = 0x0e; // .
        else if (c == 0x60)
            c = 0x00; // space

        display_putc(x++,y,c);
    }
}

/**
 * @brief put string s on screen at x,y using color3
 * @param x Horizontal position (0-19)
 * @param y Vertical position (0-11)
 * @param s string to place
 */
static void display_puts_color3(unsigned char x, unsigned char y, const char *s)
{
    char c=0;

    while (c = *s++)
    {
        c += 0x80;

        if (c == 0xA0)
            c = 0x00; // correct space

        display_putc(x++,y,c);
    }
}

/**
 * @brief Pause for a bit
 */
void display_pause(void)
{
    sleep(240);
}

/**
 * @brief Change field value to color3 (RED)
 * @param s pointer to string to change
 */
void set_color3(char *s)
{
    while (*s)
        *s++ += COLOR3_CHARSET_OFFSET;

    // Also set background color to red, to indicate something wrong.
    OS.color4 = 0x22;
}

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
             int database_stats_servers)
{
    char tmp[80]; // Line buffer for sprintf
    unsigned char y=0;

    display_setup();

    display_puts_color3(0,y++,"   server  health");
    display_puts_color1(0,y++," fisheye.diller.org");

    y++;

    if (strcmp(status,"ok"))
        set_color3(status);

    sprintf(tmp," STATUS: %s",status);
    display_puts(0,y++,tmp);

    sprintf(tmp,"VERSION: %s",version);
    display_puts(0,y++,tmp);

    if (strcmp(database_status,"connected"))
        set_color3(database_status);

    sprintf(tmp," DBSTAT: %s",database_status);
    display_puts(0,y++,tmp);

    sprintf(tmp,"  FILES: %lu",database_stats_files);
    display_puts(0,y++,tmp);

    sprintf(tmp,"CLIENTS: %u",database_stats_clients);
    display_puts(0,y++,tmp);

    sprintf(tmp,"SERVERS: %u",database_stats_servers);
    display_puts(0,y++,tmp);

    display_puts(0,10,"SERVER UPTIME:");
    format_uptime(uptime,tmp,sizeof(tmp));
    display_puts(0,11,tmp);

    display_pause();
}

/**
 * @brief message indicating we can't fetch!
 */
void cant_fetch(void)
{
    display_puts(0,6,"CAN'T FETCH!");
    display_pause();
}
