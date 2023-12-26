#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int i = 0;
    int n = 0;
    int p = 0;
    while (i < 1 || i > 8)
    {
        i = get_int("Choose Height:");
    }
    for (n = 1; n <= i; n++)
    {
        for (p = 0; p < (i - n); p++)
        {
            printf(" ");
        }
        for (p = 0; p < n; p++)
        {
            printf("#");
        }

        printf("  ");

        for (p = 0; p < n; p++)
        {
            printf("#");
        }
        printf("\n");
    }
}
