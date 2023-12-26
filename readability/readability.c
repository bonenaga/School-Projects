#include <ctype.h>
#include <stdio.h>

int count_letters(const char *text);
int count_words(const char *text);
int count_sentences(const char *text);

int main(void)
{
    char text[1000];

    // Prompt user for text
    printf("Text: ");
    fgets(text, sizeof(text), stdin);

    // Count letters, words, and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculate L and S
    float L = (float) letters / words * 100;
    float S = (float) sentences / words * 100;

    // Apply Coleman-Liau index formula
    float index = 0.0588 * L - 0.296 * S - 15.8;

    // Print grade level
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        // Round to nearest whole number
        printf("Grade %d\n", (int) (index + 0.5));
    }
}

int count_letters(const char *text)
{
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }
    }
    return count;
}

int count_words(const char *text)
{
    // Start at 1 to count the last word
    int count = 1;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            count++;
        }
    }
    return count;
}

int count_sentences(const char *text)
{
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            count++;
        }
    }
    return count;
}
