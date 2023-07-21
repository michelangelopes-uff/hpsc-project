__kernel void mean_filter(__global float* inputPixels, __global float* outputPixels, int image_width, int height, int window_size)
{
    const int gid_x = get_global_id(0); //column
    const int gid_y = get_global_id(1); //row

    // Calculate the starting and ending x, y coordinates for the 3x3 neighborhood
    int startX = max(0, gid_x - window_size);
    int startY = max(0, gid_y - window_size);
    int endX = min(image_width - window_size, gid_x + window_size);
    int endY = min(height - window_size, gid_y + window_size);

    float sum = 0.0f;

    // Compute the sum of the neighborhood
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            sum += inputPixels[y * image_width + x];
        }
    }

    // Calculate the mean and write it to the output
    float mean = sum / 9.0f;
    outputPixels[gid_y * image_width + gid_x] = mean;
}
