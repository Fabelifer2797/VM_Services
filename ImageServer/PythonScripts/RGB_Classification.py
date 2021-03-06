# Required packages for image processing
import sys
from PIL import Image
from pathlib import Path

# Function to classificate an image upon its rgb values

def rgbClassification(imagePath, savingPath):

    home = str(Path.home())
    imagePathFinal = imagePath.replace("~", home)
    savingPathFinal = savingPath.replace("~", home)
    image = Image.open(imagePathFinal)
    imageName = getImageName(imagePath)
    imageSize = image.size
    imageWidth = imageSize[0]
    imageHeight = imageSize[1]
    colorAverage = getRGBaverage(image, imageWidth, imageHeight)
    highestValue = max(colorAverage)
    indexCA = colorAverage.index(highestValue)

    if indexCA == 0:

        image.save(savingPathFinal + "/rojas/" + imageName)

    elif indexCA == 1:

        image.save(savingPathFinal + "/verdes/" + imageName)

    else:

        image.save(savingPathFinal + "/azules/" + imageName)



# Function to get the image name from the path given in the rgbClassification function

def getImageName(imagePath):

    pathList = imagePath.split("/")
    nameAndFormat = pathList[-1]
    return nameAndFormat

# Function to get the average of the RGB components

def getRGBaverage(image, width, height):

    redValue = 0
    greenValue = 0
    blueValue = 0

    for x in range(width):

        for y in range(height):

            redValue += image.getpixel((x, y))[0]
            greenValue += image.getpixel((x, y))[1]
            blueValue += image.getpixel((x, y))[2]

    totalPixels = width * height
    colorAverage = [redValue / totalPixels, greenValue / totalPixels, blueValue / totalPixels]
    # print(colorAverage)
    return colorAverage

# Calling the main function for tests

rgbClassification(sys.argv[1], sys.argv[2])