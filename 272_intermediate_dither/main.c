#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int width, height, max_value;
  int *image_buffer;

  scanf("P2 %i %i %i", &width, &height, &max_value);

  image_buffer = malloc(width * height * sizeof(int));
  for (int i = 0; i < width * height; i++) scanf("%i", image_buffer + i);

  printf("P1\n%i %i\n", width, height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int new_value = (image_buffer[x + y * width] > (max_value / 2)) ? 0 : 1;
      int error     = image_buffer[x + y * width] - max_value * (1 - new_value);
      printf("%i ", new_value);
      if (x + 1 < width) image_buffer[(x + 1) + y * width] += error * 7 / 16;
      if (x > 0 && y + 1 < height)
        image_buffer[(x - 1) + (y + 1) * width] += error * 3 / 16;
      if (y + 1 < height) image_buffer[x + (y + 1) * width] += error * 5 / 16;
      if (x + 1 < width && y + 1 < height)
        image_buffer[(x + 1) + (y + 1) * width] += error * 1 / 16;
    }
    putchar('\n');
  }

  free(image_buffer);
  return 0;
}
