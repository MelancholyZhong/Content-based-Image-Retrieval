# Content-based-Image-Retrieval

The objective of the project is to gain experience in manipulating and analyzing images at a pixel level, as well as performing image matching and pattern recognition using generic image characteristics such as color, texture, and spatial layout. The task involves finding similar images in a database using color spaces, histograms, spatial features, and texture features, without using neural networks or object recognition methods.

If encounter any problems, please feel free to contact us via email hu.hui1@northeastern.edu or zhong.yao@northeastern.edu

## Environment

MacOS M1 chip

IDE: VS Code

Build with Makefile

## Links

- link to report: https://cerulean-novel-54b.notion.site/Report-2-Content-based-Image-Retrieval-b09a291ee55f4d4195472a594744dd0b

- link to github: https://github.com/MelancholyZhong/content-based-image-retrieval

## How to run the code

In the treminal

1. Enter command `make match` to compile cpp files

2. Enter enter command `./match [target] [database] [method] [N]` to find the top N matches

   `target`: target image name, e.g. “pic.0535.jpg”

   `database`: the name of the database, e.g. “olympus”

   `method`: the matching method, the method set is {”baseline”, "color”, "magnitude”, "objectSpacial”, "spacialVariance”, "laws”, "multiHistogram", "coMatrix”, "gradient”}.

   `N`: the number of results

3. An example input would be `./match pic.1016.jpg olympus baseline 3`

To view the results

4. When matching result is ready, the first matching result will be shown in a window, you can press `n` to view next image(if any), `p` to view previous image(if any) and press `q` to close the window.

5. The list of matching result will also be displayed in the terminal.

## List of methods

`baseline` Task1: baseline matching
`color` Task2: single color histogram matching
`multiHistogram` Task3: multi histogram matching
`magnitude` Task4: color and texture matching
`spacialVariance` Task5: custom design
`laws`,`coMatrix`, or `gradient` Extensions: different texture matchings

## notes

Generating feature files for task 4 and task 5 for the first time may take 2-3 minutes.
