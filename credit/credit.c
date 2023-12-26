#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int length = 0;
int sum = 0;
int digit = 0;
char result[20] = "INVALID";

int main(void)
{
    // gets input from user
    long credit = get_long("Enter your credit card number ");
    char str[50];
    sprintf(str, "%ld", credit);
    length = strlen(str);

    // checks for the type of card used
    char first[3];
    first[0] = str[0];
    first[1] = str[1];
    first[2] = '\0';
    int num = atoi(first);
    if (length == 13 && str[0] == '4')
    {
        strcpy(result, "VISA");
    }
    if (length == 16 && str[0] == '4')
    {
        strcpy(result, "VISA");
    }
    if (length == 15 && (num == 34 || num == 37))
    {
        strcpy(result, "AMEX");
    }
    if (length == 16 && num > 50 && num < 56)
    {
        strcpy(result, "MASTERCARD");
    }

    // run checksum
    bool double_digit = false;

    for (int i = length - 1; i >= 0; i--)
    {
        digit = str[i] - '0';

        if (double_digit)
        {
            digit *= 2;

            if (digit > 9)
            {
                digit -= 9;
            }
        }

        sum += digit;
        double_digit = !double_digit;
    }

    if (sum % 10 != 0)
    {
        strcpy(result, "INVALID");
    }

    printf("%s\n", result);
}
