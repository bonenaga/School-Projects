import re

def main():
    # Prompt user for text
    text = input("Text: ")

    # Count letters, words, and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Calculate L and S
    L = (letters / words) * 100
    S = (sentences / words) * 100

    # Apply Coleman-Liau index formula
    index = 0.0588 * L - 0.296 * S - 15.8

    # Print grade level
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        # Round to nearest whole number
        print(f"Grade {round(index)}")

def count_letters(text):
    return len([char for char in text if char.isalpha()])

def count_words(text):
    # Count words by splitting text into a list of words
    return len(text.split())

def count_sentences(text):
    # Count sentences using regular expressions
    return len(re.findall(r'[.!?]', text))

if __name__ == "__main__":
    main()
