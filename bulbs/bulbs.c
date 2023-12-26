#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // prompts user for input
    string input = get_string("Enter a word or phrase to convert: ");

    // iterate over each character in the input string
    for (int i = 0; i < strlen(input); i++)
    {
        char current_char = input[i];

        // iterate over each bit in the character
        for (int j = BITS_IN_BYTE - 1; j >= 0; j--)
        {
            // extract the j-th bit of the current character
            int bit = (current_char >> j) & 1;

            // print the corresponding bulb emoji
            print_bulb(bit);
        }

        // add a space between characters
        printf("\n");
    }

    // print a newline at the end
    printf("\n");

    return 0;
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}


