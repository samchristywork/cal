#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *header = "Su Mo Tu We Th Fr Sa";

const char *months[] = {"January",   "February", "March",    "April",
                        "May",       "June",     "July",     "August",
                        "September", "October",  "November", "December"};

char *invert = "\033[7m";

char *reset = "\033[0m";

char *get_header(int month, int year) {
  int spacing_amt = (strlen(header) - sizeof(months[month - 1]) - 4) / 2;

  char *line = malloc(strlen(header));
  memset(line, ' ', strlen(header));

  char date[100];
  sprintf(date, "%s %d", months[month - 1], year);

  strncpy(line + spacing_amt, date, strlen(date));

  return line;
}

int print_week(time_t *day, time_t now, time_t start_of_month,
               time_t end_of_month) {

  int day_displayed = 0;

  for (int i = 0; i < 7; i++) {
    struct tm *sunday_before_local = localtime(day);

    if (*day < start_of_month || *day > end_of_month) {
    } else {
      printf("%2d", sunday_before_local->tm_mday);
      day_displayed = 1;
    }

    if (i != 6) {
      printf(" ");
    }

    (*day) += 24 * 60 * 60;
  }

  return day_displayed;
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

void print_calendar(int month, int year, time_t sunday_before, time_t now,
                    time_t first_day, time_t last_day) {
  printf("%s\n", get_header(month, year));
  printf("%s\n", header);

  for (;;) {
    if (!print_week(&sunday_before, now, first_day, last_day)) {
      break;
    }
    printf("\n");
  }

  printf("\n");
}

int main(void) {
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  int year = local->tm_year + 1900;
  int month = local->tm_mon + 1;
  int day = local->tm_mday;

  time_t first_day;
  struct tm *first_day_local;

  first_day = now - (day - 1) * 24 * 60 * 60;
  first_day_local = localtime(&first_day);

  int first_day_weekday = first_day_local->tm_wday;

  time_t sunday_before;
  struct tm *sunday_before_local;

  sunday_before = first_day - first_day_weekday * 24 * 60 * 60;
  sunday_before_local = localtime(&sunday_before);

  time_t last_day = first_day + (days_in_month(month, year) - 1) * 24 * 60 * 60;

  print_calendar(month, year, sunday_before, now, first_day, last_day);
}
