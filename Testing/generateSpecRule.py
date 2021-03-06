# File:         generateSpecRule.py
# Author:       Carter Hale
# Date Created: September 9, 2020
# Last Updated: March 24, 2021

# Golly doesn't offer standalone Library so the Module is instantiated
# when the Executable is launched. Script can only be ran from within Golly.
import golly as g
# Random Library used for Rule Generation
import random
# OS Library used for File Management
import os


# -----------------------------------------------------------------------------
# Function to create Random Rules
def make_rule():
    # Determine Number of Rules
    numBorn = random.randrange(0, 9)
    numSurvive = random.randrange(0, 9)

    # Randomly Generate Birth Conditions
    bornRule = ''
    for i in range(numBorn):
        num = random.randrange(1, 9)
        if(str(num) not in bornRule):
            bornRule += str(num)

    # Sort Numbers for Rule Notation
    bornRule = ''.join(sorted(bornRule))

    # Randomly Generate Survival Conditions
    surviveRule = ''
    for j in range(numSurvive):
        num = random.randrange(1, 9)
        if(str(num) not in surviveRule):
            surviveRule += str(num)

    surviveRule = ''.join(sorted(surviveRule))

    # Return Rule after concatenating Strings
    rule = "b" + bornRule + "/s" + surviveRule
    return rule
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
# Function to compare RLE Files for Homogeneity
def compare_rle(fileName1, fileName2):
    with open(fileName1, 'r') as file1:
        with open(fileName2, 'r') as file2:
            difference = set(file1).difference(file2)

    return (len(difference) == 1)
# -----------------------------------------------------------------------------


# Ask User Configuration Questions
gridSize = g.getstring("Enter Side Length of Grid:", "100", "Grid Size")
fillPerc = g.getstring("Enter Grid Fill Percentage:", "50", "Fill Percentage")

customRule = g.getstring("If you wish to use a Specific Rule,\n" +
                         "enter it below with 'B0...8/S0...8' Notation.\n" +
                         "Otherwise, press 'OK' for Random Generation:",
                         "Random", "Specific Rule Set")

numGenerations = g.getstring("Enter Number of CA Generations to Explore\n" +
                             "for Each Individual Rule in the Population:",
                             "100", "Number of Generations")

# Loop Process for Each Individual Rule in the Population

# Create New Window and Fill X% of YxY Square Grid with Random Noise
g.new("test-pattern")
g.select([0, 0, int(gridSize), int(gridSize)])
g.randfill(int(fillPerc))

# Declare Algorithm and Rule
g.setalgo("QuickLife")

if (customRule == "Random"):
    rule = make_rule()
else:
    rule = customRule.lower()

g.setrule(rule)

# Creates "Test Patterns" Folder within Directory if it does not Exist
fileLoc = g.getdir("rules") + "Test Patterns/"
if (os.path.isdir(fileLoc) is not True):
    os.mkdir(fileLoc)

# Creates Subfolder specific to Rule Set to hold Generation Patterns
fileLoc += rule.replace("/", "_") + "/"
if (os.path.isdir(fileLoc) is not True):
    os.mkdir(fileLoc)

# Prepare File Names for each Genereration's Pattern File
fileNamePrefix = fileLoc + rule.replace("/", "_") + "_"

# Loop and Save Patterns
for i in range(int(numGenerations) + 1):
    # Stop Loop if Universe is Empty
    if (g.empty()):
        break

    # Determine File Names
    fileNameRLE = fileNamePrefix + str(i) + ".rle"
    # Determine Previous File Names
    fileNamePrevRLE = fileNamePrefix + str(i-1) + ".rle"

    g.save(fileNameRLE, "rle")
    # Compare Previous Generation to Determine Class I Systems
    if (i > 0 and compare_rle(fileNameRLE, fileNamePrevRLE)):
        break

    g.run(1)

# Prepare for Viewing
g.fit()
g.setcursor("Select")
