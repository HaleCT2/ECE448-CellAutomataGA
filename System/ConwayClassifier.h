/* 
 * File:   ConwayClassifier.h
 * Author: Eric Schonauer
 *
 */

#ifndef CONWAY_CLASSIFIER_H
#define CONWAY_CLASSIFIER_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>

class ConwayClassifier {
public:
    // constructor
    // takes path to data directory and number of gens run as well as the max
    // number of threads allowed
    // endCalcPercent is the end percentage of generations for which stats
    // should be calculated, so endCalcPercent == 25 means that the last 
    // 25% of generations will have stats calculated for them
    ConwayClassifier(const std::string& dataDirPath, const int genNum,
            const int maxThrNum, const int endCalcPercent);

    // destructor to deallocate
    ~ConwayClassifier();

    /*
     * return classification num which is initialized to 3 and has the 
     * chance to be set to either 1 or 2 in constructor as necessary
     */
    unsigned short int classification();

    // returns x and y coordinates of top left corner as a pair
    std::pair<int, int> getCoords() const;

    // returns width and height as a pair
    std::pair<int, int> getDimensions() const;

    // returns number of generations, which means the max generation number
    // plus one due to the starting condition, generation 0
    int getGenNum() const;

    // returns min and max x-coords as pair for given gen if giveXCoords is true
    // else returns min and max y-coords as pair for given gen
    std::pair<int, int> getMinMax(const int gen, const bool giveXCoords) const;

    // returns value of given cell
    bool getCellVal(const int gen, const int xCoord, const int yCoord) const;

    // returns the rule of the given data as a string ex:b234_s67
    std::string getRule() const;

    // returns the alive cell ratio for a given generation
    // if no generation is given (hence genNum = -1) then this getter will
    // return the average of all calculated alive cell ratios
    double getAliveCellRatio(const int genNum = -1) const;

    // returns the percent change between a given gen and the previous, so if
    // supplied generation is 750, the percent change returned is the difference
    // between generations 749 and 750
    // if no generation is given (hence genNum = -1) then this getter will
    // return the average of all calculated percent changes between gens
    double getPercentChange(const int genNum = -1) const;
    
    // returns the active cell ratio for a given generation that is within the
    // set of generations that have stats calculated for them. If no generation
    // is supplied as a parameter, the average of the activeCellRatio vector 
    // will be given
    double getActiveCellRatio(const int genNum = -1) const;
    
    // returns factor of expansion expansion along x-axis for any generation 
    // given if no parameter is given, it returns the average over the last set
    // of gens on which stats are calculated
    // Example: if the board from one gen to the next increases in width by 50%,
    // this method would return 1.5. Conversely, if the board shrinks by 50%,
    // this method would return 0.5.
    // NOTE: this method calculates everything as its called and thus repeat
    // calls without a parameter (calculating the average) will reduce 
    // performance
    double getExpansionRateX(const int genNum = -1) const;
    
    // same as getExpansionPercentageX but in the y-direction
    // NOTE: this method calculates everything as its called and thus repeat
    // calls without a parameter (calculating the average) will reduce 
    // performance
    double getExpansionRateY(const int genNum = -1) const;
    
    // same as getExpansionPercentageX but compares total area, not just in
    // one direction
    // NOTE: this method calculates everything as its called and thus repeat
    // calls without a parameter (calculating the average) will reduce 
    // performance
    double getExpansionRateArea(const int genNum = -1) const;

    // prints a given generation of the gameBoard to the given output stream
    void printGameBoard(const int genNum, std::ostream& os = std::cout,
            const char onChar = '1', const char offChar = '0') const;

private:
    // the rule of the given data as a string ex:b234_s67
    // in golly x is positive to the right and y is positive going DOWN NOT UP
    std::string rule;
    // number of generations specified plus one for the
    // starting conditions
    int generationCount;
    int x; // x-coordinate of top left corner
    int y; // y-coordinate of top left corner
    int width; // width measures length of board from x to right end of board
    int height; // height measures length of board from y to bottom end
    // represents classification num of this rule. Anything greater than class
    // 2 will be assigned a value of 3.
    unsigned short int classNum;
    // the gameboard will be resized such that it covers the max range of the
    // game
    /*
     * IMPORTANT GAMEBOARD NOTE: when "pressing" 3d game board into 1d
     * it is done by generation, then y-coord, then x-coord.
     * This means that when you go one element over in the array (assuming that
     * you don't cross a boundary between gen's or differing rows) you are 
     * increasing the x-coord value by one. Another way to think of this is
     * that the data was "pressed" row-wise, reading down the 0th row
     * then increasing the y-coord by one and reading down the 1st row and
     * so on.
     */
    bool* gameBoard; // 1d array to represent 3d board for speed
    long long int boardSize; // size of the gameBoard array
    const int posQualifierLen = 4; // "pos=" length in rle header
    // saves the min and the max x-coord for every gen
    std::vector<std::pair<int, int>> minMaxX;
    // saves the min and the max y-coord for every gen
    std::vector<std::pair<int, int>> minMaxY;
    // from this gen on, stats will be calculated for things like aliveCellRatio
    int statStartGen;
    // vector describing alive cell ratio for gens specified by endCalcPercent
    std::vector<double> aliveCellRatio;
    // vector where each element describes
    // percentage of cells that change their value between generations so if
    // you start calculating stats with generation 750, the 0th element of this
    // vector is the % change between generations 749 and 750
    std::vector<double> percentChange;
    // a vector where each element describes the percentage of cells in a given
    // generation that are considered "active" active cells have to have been
    // alive for the last n generations, and also have been dead within the
    // last k generations
    std::vector<double> activeCellRatio;
    // describes how many immediately previous consecutive generations a cell
    // must be alive to fulfill that part of the "active cell" requirement
    // Used by the cellAliveRecently method
    // For example, if this constant is set to 5, the cellAliveRecently method
    // checks the 5 previous gens and the current gen, so 6 in total
    const int consecutiveAliveLen = 5;
    // describes the range of immediately previous generations within which
    // a cell must be dead for at least one of those generations to fulfill this
    // part of the "active cell" requirement
    // Used by the cellDeadRecently method
    const int deadWithinLen = 25;


    // this is used to set relevant instance variables to 0/null if
    // class is determined to be 1/2 before classification method is called
    void voidInstanceVars();

    // return the string containing the rule that has been extracted from a
    // larger path
    std::string extractRule(const std::string& rleDataDirPath) const;

    // checks to see if number of files in directory specified by dataDirPath
    // is less than the genNum given. If so, it sets the classNum instance
    // variable to 1.
    void checkForClass1(const std::string& dataDirPath, const int genNum);

    // adds encoded pattern in each rle file to hash map to check for repeat
    // patterns. If it is class 2, the classNum variable will be set to 2
    void checkForClass2(std::vector<std::ifstream*>& dataFiles);

    // takes vector of ifstream objects to figure out coords and dimensions
    // when finished resets ifstreams
    // sets the x, y, width and height vars and dynamically resizes gameBoard
    void calcBoardSpecs(std::vector<std::ifstream*>& dataFiles);

    // with the board specs calculated fill gameBoard array with data from files
    // by calling fillGen for every ifstream
    void fillBoard(std::vector<std::ifstream*>& dataFiles, const int maxThrNum);

    // reads files corresponding to genStartNum through (and including) 
    // genEndNum and fills the gameBoard accordingly. Once done with a given
    // ifstream object it closes it.
    void fillGen(std::vector<std::ifstream*>& dataStreams,
            const int genStartNum, const int genEndNum);

    // takes path to the data directory and creates ifstream object for every
    // file and adds its address (pointer) to a vector; then returns that vector
    std::vector<std::ifstream*> populateIStreamVec(const std::string& dataPath,
            const int genNum) const;

    // deallocate all the ifstreams that are allocated in the populateIstreamVec
    // method
    void deallocateIfstreams(std::vector<std::ifstream*>& streamsToClose) const;

    // splits a string based on the given delimiter, returns the before the
    // delimiter if firstStr parameter is true, returns after the delimiter
    // if firstStr is false
    // returns empty string if no split can be made
    // used to help parse .rle headers 
    std::string split(const std::string& str, const std::string& delimiter,
            const bool firstStr) const;

    // takes what would be the 3 values needed to get a value of a cell in 
    // Conway's game and calculates at what 1D index that cell data is stored
    // in the gameBoard instance variable
    long long int get1DIndex(const int gen, const int xCoord,
            const int yCoord) const;

    // sets a given cell to a given value
    void setCellVal(const int gen, const int xCoord, const int yCoord,
            const bool val);

    // allocates memory for the gameBoard instance var and sets every elt to 0
    // also sets aliveCellRatio vector to correct length
    void initializeGameBoard(const int genNum);

    // takes the first line of an rle file and extracts the x and y values
    // for the position of the upper right corner of the data
    // return the x and y values as pair in that order
    std::pair<int, int> readPos(const std::string& firstLine) const;

    // takes the second line of an rle file as a string and extracts the 
    // width and height data ("x =" and "y =" in the rle) and returns
    // the data as a pair
    std::pair<int, int> readWidthHeight(const std::string& secLine) const;

    // adds 1 to correct vector element of aliveCellRatio
    void setAliveCount(const int genNum);

    // finishes calculating stats like the aliveCellRatio by dividing each
    // generation's alive count by the area of the generation
    void finishStats();
    
    // calculates the alive cell ratio from each element in the vector by 
    // dividing each vector element by the area of the gameBoard
    void calculateAliveCellRatio();

    // goes through generations specified by endCalcPercent and calculates
    // the percent change of cells between generations
    void calculatePercentChange();
    
    // calculates the active cell ratio for all necessary generations and
    // populates the activeCellRatio vector as it does so.
    void calculateActiveCellRatio();
    
    // takes a vector of doubles and returns the average of those values
    double averageVector(const std::vector<double>& statVec) const;
    
    // says whether or not a cell has been alive for n immediately previous
    // generations (n = this->consecutiveAliveLen)
    bool cellAliveRecently(const int gen, const int x, const int y) const;
    
    // says whether or not a cell has been dead for at least one of the
    // immediately previous n generations (n = this->deadWithinLen)
    bool cellDeadRecently(const int gen, const int x, const int y) const;
};



#endif /* CONWAY_CLASSIFIER_H */

