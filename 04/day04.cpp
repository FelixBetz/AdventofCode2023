#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>

#define INPUT_FILE "../../input.txt"

#define NUM_SLICE_BEGINNING 10 // 8
#define NUM_WINNING_NUMBERS 10 // 5
#define NUM_MY_NUMBERS 25      // 8

//------------------------------------------------------------------------------
//   getWinningNumbers
//------------------------------------------------------------------------------
static int getWinningNumbers(std::set<int> *pWinningNumbers, std::set<int> *pMyNumbers)
{
    int winningNumbers = 0;
    for (auto num : *pMyNumbers)
    {
        if (pWinningNumbers->find(num) != pWinningNumbers->end())
        {
            winningNumbers++;
        }
    }
    return winningNumbers;
}

//------------------------------------------------------------------------------
//   main
//------------------------------------------------------------------------------
int main(int, char **)
{
    // Open the file
    std::ifstream inputFile(INPUT_FILE);

    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open the file." << std::endl;
        return 1;
    }

    std::vector<std::set<int>> winningNumbers;
    std::vector<std::set<int>> myNumbers;

    std::string line;
    int part1Sum = 0;
    while (std::getline(inputFile, line))
    {

        std::set<int> winningNumbersSet;
        std::set<int> myNumbersSet;
        // erase 'Card X:'
        line.erase(0, NUM_SLICE_BEGINNING);

        // parse winning numbers
        for (int i = 0; i < NUM_WINNING_NUMBERS; i++)
        {
            int num = std::stoi(line.substr(0, 3));
            winningNumbersSet.insert(num);
            line.erase(0, 3);
        }

        // earse '|'
        line.erase(0, 1);

        // parse my numbers
        for (int i = 0; i < NUM_MY_NUMBERS; i++)
        {
            int num = std::stoi(line.substr(0, 3));
            myNumbersSet.insert(num);
            line.erase(0, 3);
        }

        winningNumbers.push_back(winningNumbersSet);
        myNumbers.push_back(myNumbersSet);
    }

    inputFile.close();

    // solve part1
    for (int i = 0; i < winningNumbers.size(); i++)
    {
        int cntWinningNumbers = getWinningNumbers(&(winningNumbers.at(i)), &(myNumbers.at(i)));
        int cardPoints = 1 << (cntWinningNumbers);
        cardPoints >>= 1; // :2
        part1Sum += cardPoints;
    }

    // solve part2

    std::vector<int> cards;
    std::vector<int> winnings;

    // precalculate winning numbers per card
    for (int cardNum = 0; cardNum < winningNumbers.size(); cardNum++)
    {
        cards.push_back(cardNum);
        int cardWinning = getWinningNumbers(&(winningNumbers.at(cardNum)), &(myNumbers.at(cardNum)));
        winnings.push_back(cardWinning);
    }

    // process cards
    int cntWinningCards = 0;
    while (!cards.empty())
    {
        int cardNum = cards.back();
        cards.pop_back();
        int cardWinning = winnings.at(cardNum);

        cntWinningCards++;
        if (cardWinning > 0)
        {
            for (int i = 0; i < cardWinning; i++)
            {
                int subCard = i + 1 + cardNum;
                if (subCard < winningNumbers.size())
                {
                    cards.push_back(subCard);
                }
            }
        }
    }

    std::cout << "PART1: points worth in total: " << part1Sum << std::endl;
    std::cout << "PART2: sum of all of the gear ratios in your engine schematic: " << cntWinningCards << std::endl;
    return 0;
}
