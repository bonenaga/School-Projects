def main():
    # Get input from user
    credit = input("Enter your credit card number: ")
    length = len(credit)

    # Check for the type of card used
    first_two_digits = int(credit[:2])
    result = "INVALID"

    if (length == 13 or length == 16) and credit[0] == '4':
        result = "VISA"
    elif length == 15 and (first_two_digits == 34 or first_two_digits == 37):
        result = "AMEX"
    elif length == 16 and 50 < first_two_digits < 56:
        result = "MASTERCARD"

    # Run checksum
    sum_digits = 0
    double_digit = False

    for i in range(length - 1, -1, -1):
        digit = int(credit[i])

        if double_digit:
            digit *= 2
            if digit > 9:
                digit -= 9

        sum_digits += digit
        double_digit = not double_digit

    if sum_digits % 10 != 0:
        result = "INVALID"

    print(result)


if __name__ == "__main__":
    main()
