#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
  int start;
  do
  {
      start = get_int("Enter Starting Population, Must be 9 or More: ");
  }
  while (start < 9);
    // TODO: Prompt for end size
  int end;
  do
  {
      end = get_int("Enter Ending Population: ");
  }
  while (end < start);
    // TODO: Calculate number of years until we reach threshold
  int n;
  n = 0;
  grow:
  if (start < end)
  {
      start = start + (start/3) - (start/4);
      n = n + 1;
      goto grow;
  }

    // TODO: Print number of years
    printf("Years: %d\n", n);
}