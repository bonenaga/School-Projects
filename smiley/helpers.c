#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Check if pixel is black
            if (image[i][j].rgbtBlue == 0x00 && image[i][j].rgbtGreen == 0x00 && image[i][j].rgbtRed == 0x00)
            {
                // Change black pixels to blue
                image[i][j].rgbtGreen = 0x00; // turned off
                image[i][j].rgbtRed = 0x00;   // turned off
                image[i][j].rgbtBlue = 0xFF;  // turned to max
            }
        }
    }
}
