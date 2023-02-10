# Content-based-Image-Retrieval

The objective of the project is to gain experience in manipulating and analyzing images at a pixel level, as well as performing image matching and pattern recognition using generic image characteristics such as color, texture, and spatial layout. The task involves finding similar images in a database using color spaces, histograms, spatial features, and texture features, without using neural networks or object recognition methods.

If encounter any problems, please feel free to contact us via email hu.hui1@northeastern.edu or 
# Links

- link to report: https://cerulean-novel-54b.notion.site/Report-2-Content-based-Image-Retrieval-b09a291ee55f4d4195472a594744dd0b

- link to github: https://github.com/MelancholyZhong/content-based-image-retrieval

# How to run the code

In the treminal 

1. Enter command `make match` to compile cpp files
2. Enter enter command `/match target dirname method N` to find the N most close images
    
    `target`: the path to the target image, e.g. “olympus/pic.0535.jpg”
    
    `dirname`: the name of directory, e.g. “olympus”
    
    `method`: the matching method you wanna use, the  method set is {”baseline”, "color”, "magnitude”, "objectSpacial”, "spacialVariance”, "laws”, "multiHistogram, "coMatrix”, "gradient”}. You can choose only one method, the default method is “baseline”
    
    `N`: the number of results you wanna return

# Details 
## color histogram

### calculation

We choosed the RGB histogram and used 8 bins for each channel. Therefore, this is a 3-D histogram stored in cv::Mat

### normalization

The normalization is to transform the count into the propability of distribution, therefore we used the value count/total_pixel as the value of each index.

### distance

Use histogram intersection as the distance metric.

## magnitude histogram

### calculation

We choosed to calculate a Sobel magnitude image of the input image and use a histogram of gradient magnitudes as the texture feature. Therefore, it is a 3-D histogram stored in cv::Mat

### normalization

The normalization is to transform the count into the propability of distribution, therefore we used the value count/total_pixel as the value of each index.

### distance

We currently still uses the intersection as the distance metric, we may try other ways later.
