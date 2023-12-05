#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <cctype>

using namespace std;

#define INPUT_FILE "../../input.txt"

const std::string DIGIT_STRINGS[] = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

//------------------------------------------------------------------------------
//   checkDigitTuples
//------------------------------------------------------------------------------
static void checkDigitTuples(std::tuple<int, int> *pFirstDigit, std::tuple<int, int> *pSecondDigit, int pPos, int pVal)
{
    if (pPos < get<0>(*pFirstDigit))
    {
        *pFirstDigit = std::make_tuple(pPos, pVal);
    }
    if (pPos > get<0>(*pSecondDigit))
    {
        *pSecondDigit = std::make_tuple(pPos, pVal);
    }
}

//------------------------------------------------------------------------------
//   solvePartOne
//------------------------------------------------------------------------------
static void solvePartOne()
{
    // Open the file
    std::ifstream inputFile(INPUT_FILE);

    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return;
    }

    std::string line;

    int calibrationValueSum = 0;
    while (std::getline(inputFile, line))
    {
        std::tuple<int, int> firstDigit = std::make_tuple(std::numeric_limits<int>::max(), -1);
        std::tuple<int, int> secondDigit = std::make_tuple(std::numeric_limits<int>::min(), -1);
        int pos = 0;
        for (char ch : line)
        {
            if (std::isdigit(ch))
            {
                int digit = ch - '0';
                checkDigitTuples(&firstDigit, &secondDigit, pos, digit);
            }
            pos++;
        }

        int calibrationValue = 10 * get<1>(firstDigit) + get<1>(secondDigit);
        calibrationValueSum += calibrationValue;
    }
    // Close the file
    inputFile.close();
    std::cout << "PART1: sum of all of the calibration values: " << calibrationValueSum << std::endl;
}

//------------------------------------------------------------------------------
//   solvePartTwo
//------------------------------------------------------------------------------
static void solvePartTwo()
{
    // Open the file
    std::ifstream inputFile(INPUT_FILE);

    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return;
    }

    std::string line;

    int calibrationValueSum = 0;
    while (std::getline(inputFile, line))
    {

        std::tuple<int, int> firstDigit = std::make_tuple(std::numeric_limits<int>::max(), -1);
        std::tuple<int, int> secondDigit = std::make_tuple(std::numeric_limits<int>::min(), -1);

        int digitIndex = 1;
        for (std::string digitString : DIGIT_STRINGS)
        {

            // Find the position of the first occurrence
            size_t pos = line.find(digitString);
            while (pos != std::string::npos)
            {
                checkDigitTuples(&firstDigit, &secondDigit, (int)pos, digitIndex);
                // Find the next occurrence
                pos = line.find(digitString, pos + digitString.length());
            }

            digitIndex++;
        }

        int pos = 0;
        for (char ch : line)
        {
            if (std::isdigit(ch))
            {
                int digit = ch - '0';
                checkDigitTuples(&firstDigit, &secondDigit, pos, digit);
            }
            pos++;
        }

        int calibrationValue = 10 * get<1>(firstDigit) + get<1>(secondDigit);

        calibrationValueSum += calibrationValue;
    }
    // Close the file
    inputFile.close();
    std::cout << "PART2: sum of all of the calibration values: " << calibrationValueSum << std::endl;
}

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{
    solvePartOne();
    solvePartTwo();
    return 0;
}
