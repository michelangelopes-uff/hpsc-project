__kernel void mean_filter(__global float* inputPixels, __global float* outputPixels, int width, int height, int window_size)
{
    const int gid_x = get_global_id(0); //column
    const int gid_y = get_global_id(1); //row

    // if(gid_y == 0)
    //     printf("[%d][%d]\n", gid_y, gid_x);

    int startRow = max(0, gid_y - window_size/2);
    int startColumn = max(0, gid_x - window_size/2);
    int endRow = min(height - window_size, gid_y + window_size/2);
    int endColumn = min(width - window_size, gid_x + window_size/2);

    float sum = 0.0f;
    int count = 0;

    for (int y = startRow; y <= endRow; y++)
    {
        for (int x = startColumn; x <= endColumn; x++)
        {
            sum += inputPixels[y * width + x];
            count += 1;
        }
    }

    float mean = sum / (count * count);
    // printf("%f\n", mean);
    outputPixels[gid_y * width + gid_x] = mean;
}