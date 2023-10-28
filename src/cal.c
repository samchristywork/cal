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

time_t now;

char *get_header(int year, int month) {
  int spacing_amt = (strlen(header) - sizeof(months[month - 1]) - 4) / 2;

  char *line = malloc(strlen(header) + 1);
  memset(line, ' ', strlen(header));
  line[strlen(header)] = '\0';

  char date[100];
  sprintf(date, "%s %d", months[month - 1], year);

  strncpy(line + spacing_amt, date, strlen(date));

  return line;
}

int print_week(time_t *day, int month) {
  int day_displayed = 0;

  for (int i = 0; i < 7; i++) {
    struct tm *sunday_before_local = localtime(day);

    int is_current_day = now >= *day && now < *day + 24 * 60 * 60;

    if (is_current_day) {
      printf("%s", invert);
    }

    int is_in_month = (sunday_before_local->tm_mon + 1) == month;
    if (is_in_month) {
      printf("%2d", sunday_before_local->tm_mday);
      day_displayed = 1;
    } else {
      printf("  ");
    }

    if (is_current_day) {
      printf("%s", reset);
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

time_t get_time(int year, int month) {
  struct tm t;

  t.tm_mon = month - 1;
  t.tm_year = year - 1900;
  t.tm_mday = 1;
  t.tm_hour = 0;
  t.tm_min = 0;
  t.tm_sec = 0;

  return mktime(&t);
}

void print_calendar_row(int year, int month, int row) {
  time_t t = get_time(year, month);

  struct tm *local = localtime(&t);
  int day = local->tm_mday;

  time_t first_day = t - (day - 1) * 24 * 60 * 60;
  struct tm *first_day_local = localtime(&first_day);

  time_t sunday_before = first_day - first_day_local->tm_wday * 24 * 60 * 60;
  time_t start = sunday_before + row * 7 * 24 * 60 * 60;

  print_week(&start, month);
}

void print_month(int year, int month) {
  printf("%s\n", get_header(year, month));
  printf("%s\n", header);

  for (;;) {
    print_week(&start, &num_days, month);
    if (num_days <= 0) {
      break;
    }

    printf("\n");
  }

  printf("\n");
}

int main(void) {
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

  int num_days = (last_day - sunday_before) / (24 * 60 * 60) + 1;

  print_calendar(month, year, sunday_before, num_days);
}
