# DONE
def main():
    i = 0
    while i < 1 or i > 8:
        try:
            i = int(input("Choose Height: "))
        except ValueError:
            continue

    for n in range(1, i + 1):
        print(" " * (i - n) + "#" * n + "  " + "#" * n)


if __name__ == "__main__":
    main()
