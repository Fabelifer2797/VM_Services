# Required packages for image processing
import sys
from PIL import Image, ImageOps
from pathlib import  Path

# Function to process the image
# imagePath: Is the path where the image is stored in the system
# savingPath: Is the path where the equalized image it'll be saved

def imageProcessing(imagePath, savingPath):

    home = str(Path.home())
    imagePathFinal = imagePath.replace("~", home)
    savingPathFinal = savingPath.replace("~", home)
    image = Image.open(imagePathFinal)
    imageName = getImageName(imagePath)
    imageFormat = "." + str(image.format).lower()
    imageFinalName = "/" + imageName + "_HE" + imageFormat


    if str(image.mode) == "RGB":

        image = imageRGBtoGS(image)


    histogramEqualization(image,imageFinalName,savingPathFinal)

# Function to get the image name from the path given in the imageProcessing function

def getImageName(imagePath):

    pathList = imagePath.split("/")
    nameAndFormat = pathList[-1]
    finalList = nameAndFormat.split(".")
    name = finalList[0]
    return name

# Function to convert a colour image to a grayscale one

def imageRGBtoGS(imageRGB):

    imageL = imageRGB.convert('L')
    return imageL

# Function to perform the histogram equalization algorithm
# The processed image will be saved with the original name plus the _HE acronym (Histogram Equalized)

def histogramEqualization(imageGS, imageFinalName, savingPath):

    # imageGS.show()
    imageHE = ImageOps.equalize(imageGS)
    # imageHE.show()
    imageHE.save(savingPath + imageFinalName)


# Calling the main function for tests


imageProcessing(sys.argv[1], sys.argv[2])
