#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *header = "Su Mo Tu We Th Fr Sa";

const char *months[] = {"January",   "February", "March",    "April",
                        "May",       "June",     "July",     "August",
                        "September", "October",  "November", "December"};

char *get_header(int month, int year) {
  int spacing_amt = (strlen(header) - sizeof(months[month - 1]) - 4) / 2;

  char *line = malloc(strlen(header));
  memset(line, ' ', strlen(header));

  char date[100];
  sprintf(date, "%s %d", months[month - 1], year);

  strncpy(line + spacing_amt, date, strlen(date));

  return line;
}

void write_row(const char *s) {
  fwrite(s, 1, strlen(header), stdout);
  printf("\n");
}

char *print_week(time_t *day, time_t now, time_t start_of_month,
                 time_t end_of_month) {
  char *line = malloc(strlen(header) + 1);
  memset(line, ' ', strlen(header));

  int day_displayed = 0;

  for (int i = 0; i < 7; i++) {
    struct tm *sunday_before_local = localtime(day);

    if (*day < start_of_month || *day > end_of_month) {
    } else {
      char day_str[3];
      sprintf(day_str, "%2d", sunday_before_local->tm_mday);
      memcpy(line + i * 3, day_str, 2);
      day_displayed = 1;
    }

    (*day) += 24 * 60 * 60;
  }

  if (day_displayed) {
    return line;
  } else {
    return NULL;
  }
}

int days_in_month(int month, int year) {
  if (month == 2) {
    if (year % 4 == 0) {
      return 29;
    } else {
      return 28;
    }
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  } else {
    return 31;
  }
}

int main() {
}
