# Required packages for image processing
from PIL import Image

# Function to classificate an image upon its rgb values

def rgbClassification(imagePath, savingPath):

    image = Image.open(imagePath)
    imageName = getImageName(imagePath)
    imageSize = image.size
    imageWidth = imageSize[0]
    imageHeight = imageSize[1]
    colorAverage = getRGBaverage(image, imageWidth, imageHeight)
    highestValue = max(colorAverage)
    indexCA = colorAverage.index(highestValue)

    if indexCA == 0:

        image.save(savingPath + "rojas/" + imageName)

    elif indexCA == 1:

        image.save(savingPath + "verdes/" + imageName)

    else:

        image.save(savingPath + "azules/" + imageName)



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
    print([redValue,greenValue,blueValue])
    colorAverage = [redValue / totalPixels, greenValue / totalPixels, blueValue / totalPixels]
    print(colorAverage)
    return colorAverage

# Calling the main function for tests

# rgbClassification("blue.png", "")