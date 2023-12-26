#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];
            uint8_t average = (uint8_t) ((pixel->rgbtRed + pixel->rgbtGreen + pixel->rgbtBlue) / 3.0 + 0.5);
            pixel->rgbtRed = pixel->rgbtGreen = pixel->rgbtBlue = average;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];
            int originalRed = pixel->rgbtRed;
            int originalGreen = pixel->rgbtGreen;
            int originalBlue = pixel->rgbtBlue;

            // Calculate sepia values with proper rounding
            int sepiaRed = (int) (.393 * originalRed + .769 * originalGreen + .189 * originalBlue + 0.5);
            int sepiaGreen = (int) (.349 * originalRed + .686 * originalGreen + .168 * originalBlue + 0.5);
            int sepiaBlue = (int) (.272 * originalRed + .534 * originalGreen + .131 * originalBlue + 0.5);

            // Clamp values to max 255
            pixel->rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
            pixel->rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            pixel->rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    // Calculate blurred values for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalRed = 0, totalGreen = 0, totalBlue = 0;
            int count = 0;

            // Loop over neighboring pixels
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int newI = i + di;
                    int newJ = j + dj;

                    // Check if neighboring pixel is within the image boundaries
                    if (newI >= 0 && newI < height && newJ >= 0 && newJ < width)
                    {
                        RGBTRIPLE pixel = image[newI][newJ];
                        totalRed += pixel.rgbtRed;
                        totalGreen += pixel.rgbtGreen;
                        totalBlue += pixel.rgbtBlue;
                        count++;
                    }
                }
            }

            // Assign averaged (blurred) values to the temporary image
            temp[i][j].rgbtRed = (uint8_t) (totalRed / (float) count + 0.5);
            temp[i][j].rgbtGreen = (uint8_t) (totalGreen / (float) count + 0.5);
            temp[i][j].rgbtBlue = (uint8_t) (totalBlue / (float) count + 0.5);
        }
    }

    // Copy the blurred values back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}
