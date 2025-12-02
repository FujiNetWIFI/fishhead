#include <stdio.h>
#include <string.h>

void format_uptime(unsigned long seconds, char *out, size_t out_size)
{
    unsigned long days, hours, minutes, secs;
    char buf[64];

    days = seconds / 86400;
    seconds %= 86400;

    hours = seconds / 3600;
    seconds %= 3600;

    minutes = seconds / 60;
    secs = seconds % 60;

    out[0] = '\0';  // start empty

    if (days > 0) {
        snprintf(buf, sizeof(buf), "%lud", days);
        strncat(out, buf, out_size - strlen(out) - 1);
    }

    if (hours > 0) {
        snprintf(buf, sizeof(buf), "%luh", hours);
        strncat(out, buf, out_size - strlen(out) - 1);
    }

    if (minutes > 0) {
        snprintf(buf, sizeof(buf), "%lum", minutes);
        strncat(out, buf, out_size - strlen(out) - 1);
    }

    // Always show seconds, even if zero
    snprintf(buf, sizeof(buf), "%lus", secs);
    strncat(out, buf, out_size - strlen(out) - 1);
}
