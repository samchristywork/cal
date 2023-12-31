#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char *center(char *str, int width) {
  int len = strlen(str);
  int padding = (width - len + 1) / 2;
  char *result = malloc(width + 1);
  memset(result, ' ', width);
  memcpy(result + padding, str, len);
  result[width] = '\0';
  return result;
}

const char *header = "Su Mo Tu We Th Fr Sa";

const char *months[] = {"January",   "February", "March",    "April",
                        "May",       "June",     "July",     "August",
                        "September", "October",  "November", "December"};

char *invert = "\033[7m";

char *reset = "\033[0m";

time_t now;

char *get_header(int year, int month, bool year_in_header) {
  if (year_in_header) {
    char *header = malloc(100);
    sprintf(header, "%s %d", months[month - 1], year);
    return center(header, 20);
  } else {
    char *header = malloc(100);
    sprintf(header, "%s", months[month - 1]);
    return center(header, 20);
  }
}

int print_week(time_t *day, int month) {
  int day_displayed = 0;

  for (int i = 0; i < 7; i++) {
    struct tm *sunday_before_local = localtime(day);

    int is_current_day = now >= *day && now < *day + 24 * 60 * 60;

    int is_in_month = (sunday_before_local->tm_mon + 1) == month;

    int is_a_tty = isatty(fileno(stdout));

    if (is_current_day && is_in_month && is_a_tty) {
      printf("%s", invert);
    }

    if (is_in_month) {
      printf("%2d", sunday_before_local->tm_mday);
      day_displayed = 1;
    } else {
      printf("  ");
    }

    if (is_current_day && is_in_month && is_a_tty) {
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

  memset(&t, 0, sizeof(t));

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
  printf("%s\n", get_header(year, month, true));
  printf("%s\n", header);
  for (int i = 0; i < 6; i++) {
    print_calendar_row(year, month, i);
    printf("\n");
  }
}

void print_three_months(int year, int month, bool year_in_header) {

  char *normal_spacing = "  ";
  char *wide_spacing = "   ";
  char *spacing = normal_spacing;

  if (!year_in_header) {
    spacing = wide_spacing;
  }

  // Print header
  for (int i = -1; i < 2; i++) {
    int m = month + i;
    int y = year;
    if (m < 1) {
      m = 12 + m;
      y--;
    } else if (m > 12) {
      m = m - 12;
      y++;
    }

    printf("%s", get_header(y, m, year_in_header));

    if (i == -1 || i == 0) {
      printf("%s", spacing);
    }
  }
  printf("\n");

  // Print days
  printf("%s%s", header, spacing);
  printf("%s%s", header, spacing);
  printf("%s", header);
  printf("\n");

  // Print weeks
  for (int i = 0; i < 6; i++) {
    for (int j = -1; j < 2; j++) {
      int m = month + j;
      int y = year;
      if (m < 1) {
        m = 12 + m;
        y--;
      } else if (m > 12) {
        m = m - 12;
        y++;
      }
      print_calendar_row(y, m, i);
      if (j == -1 || j == 0) {
        printf("%s", spacing);
      }
    }
    printf("\n");
  }
}

void print_year(int year) {
  char *header = malloc(100);
  sprintf(header, "%d", year);
  printf("%s\n", center(header, 66));
  printf("\n");

  for (int i = 1; i <= 12; i += 3) {
    print_three_months(year, i + 1, false);
  }
}

int main(int argc, char *argv[]) {
  int month = 0;
  int year = 0;
  int show_three_months = 0;

  time(&now);

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-y") == 0) {
      // struct tm *local = localtime(&now);
      // year = local->tm_year + 1900;
    } else if (strcmp(argv[i], "-3") == 0) {
      show_three_months = 1;
    } else {
      if (year == 0) {
        year = atoi(argv[i]);
        if (year == 0) {
          fprintf(stderr, "Invalid year: %s\n", argv[i]);
          exit(EXIT_FAILURE);
        }
      } else {
        month = year;
        year = atoi(argv[i]);
        if (year == 0) {
          fprintf(stderr, "Invalid year: %s\n", argv[i]);
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  if (month == 0 && year == 0) {
    struct tm *local = localtime(&now);
    year = local->tm_year + 1900;
    month = local->tm_mon + 1;
  }

  if (show_three_months) {
    print_three_months(year, month, true);
  } else if (month == 0) {
    print_year(year);
  } else {
    print_month(year, month);
  }

  fflush(stdout);
}
