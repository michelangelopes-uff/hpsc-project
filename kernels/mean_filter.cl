__kernel void mean_filter(__global float* inputPixels, __global float* outputPixels, int image_width, int height, int window_size)
{
    const int gid_x = get_global_id(0); //column
    const int gid_y = get_global_id(1); //row

    int startColumn = max(0, gid_x - window_size);
    int startRow = max(0, gid_y - window_size);
    int endColumn = min(image_width - window_size, gid_x + window_size);
    int endRow = min(height - window_size, gid_y + window_size);

    float sum = 0.0f;

    for (int y = startRow; y <= endRow; y++)
    {
        for (int x = startColumn; x <= endColumn; x++)
        {
            sum += inputPixels[y * image_width + x];
        }
    }

    float mean = sum / (window_size * window_size);
    outputPixels[gid_y * image_width + gid_x] = mean;
}
