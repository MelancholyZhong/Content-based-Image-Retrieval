# Content-based-Image-Retrieval

## color histogram

### calculation

We choosed the RGB histogram and used 8 bins for each channel. Therefore, this is a 3-D histogram stored in cv::Mat

### normalization

The normalization is to transform the count into the propability of distribution, therefore we used the value count/total_pixel as the value of each index.

### distance

Use histogram intersection as the distance metric.
