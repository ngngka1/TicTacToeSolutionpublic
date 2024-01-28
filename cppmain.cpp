#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>
#include <string>
#include <limits>

using namespace std;

class Game
{
    private:
        static Game* instance;
        Game()
        {
            grid = {
                {'_', '_', '_'},
                {'X', '_', '_'},
                {'O', '_', '_'}
            };
        }

    public:
        vector<vector<char>> grid;
        static Game* getInstance()
        {
            
            if (instance == nullptr)
            {
                instance = new Game();
            }
            return instance;
        }
};

Game* Game::instance = nullptr;

class Player
{
    public:
        char symbol;
        int num; // indicates player1 or player2
        Player(char playerSymbol, int playerNum)
        {
            symbol = playerSymbol;
            num = playerNum;
        }
};

class ListNode
{
    public:
        int rowNum;
        int colNum;
        char placedChess;
        ListNode* next;
        ListNode(int rowNumArg, int colNumArg, char placedChessArg, ListNode* nextarg)
        {
            rowNum = rowNumArg;
            colNum = colNumArg;
            placedChess = placedChessArg;
            next = nextarg;
        }
};

void inputPlayer(Player* &user, Player* &opponent, map<int, Player*>* players)
{
    char playerSymbol1, playerSymbol2;
    int option;
    bool inputting = true;
    while (inputting)
    {
        cout << "Please input the symbol representing Player 1: (The player who makes the first move)\n";
        cin >> playerSymbol1; // only the first char of the line is read
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!cin.fail())
        {
            break;
        }
    }
    
    while (inputting)
    {
        cout << "Please input the symbol representing Player 2:\n";
        cin >> playerSymbol2; // only the first char of the line is read
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!cin.fail())
        {
            break;
        }
    }

    while (inputting)
    {
        cout << "Are you player 1 or 2? (1/2)\n";
        cin >> option;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (option == 1 || option == 2)
        {
            inputting = false;
        }
    }

    if (option == 1)
    {
        user = new Player(playerSymbol1, 1);
        opponent = new Player(playerSymbol2, 2);
        (*players)[1] = user;
        (*players)[2] = opponent;

    }
    else
    {
        user = new Player(playerSymbol2, 2);
        opponent = new Player(playerSymbol1, 1);
        (*players)[2] = user;
        (*players)[1] = opponent;
    }
}

void inputGrid()
{
    return; // wip
}

bool winCheck(const Game* game, const char checkedPlayerChess)
{
    for (int i = 0; i < 3; i++)
    {
        if // checking if there are horizontal wins
        (
            game->grid[i][0] == game->grid[i][1] && 
            game->grid[i][1] == game->grid[i][2] && 
            game->grid[i][2] == checkedPlayerChess
        )
            {
                return true;
            }
        if // checking if there are vertical wins
        (
            game->grid[0][i] == game->grid[1][i] && 
            game->grid[1][i] == game->grid[2][i] && 
            game->grid[2][i] == checkedPlayerChess
        )
            {
                return true;
            }
    }

    if (game->grid[0][0] == game->grid[1][1] && game->grid[1][1] == game->grid[2][2] && game->grid[2][2] == checkedPlayerChess)
        return true;
    if (game->grid[2][0] == game->grid[1][1] && game->grid[1][1] == game->grid[0][2] && game->grid[0][2] == checkedPlayerChess)
        return true;

    return false;
}

vector<int> getWinablePosition(Game* game, const char currChess)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (game->grid[i][j] == '_')
            {
                game->grid[i][j] = currChess;
                if (winCheck(game, currChess))
                {
                    game->grid[i][j] = '_';
                    return {i, j};
                }
                game->grid[i][j] = '_';
            }
        }
    }
    return {};
}

vector<vector<int>> validAttack(Game* game, const char currChess, const char nextChess)
{
    vector<vector<int>> validAttackPositions = {};
    vector<vector<int>> blankGrid = {};
    
    for (int rowNum = 0; rowNum < 3; rowNum++) // horizontal wins
    {
        if (game->grid[rowNum][0] != nextChess && game->grid[rowNum][1] != nextChess && game->grid[rowNum][2] != nextChess)
        {
            blankGrid = {};
            for (int colNum = 0; colNum < 3; colNum++)
            {
                if (game->grid[rowNum][colNum] == '_')
                {
                    blankGrid.push_back({rowNum, colNum});
                }
            }
            if (blankGrid.size() == 2)
            {
                validAttackPositions.insert(validAttackPositions.end(), blankGrid.begin(), blankGrid.end());
            }
        }
    }

    for (int colNum = 0; colNum < 3; colNum++) // horizontal wins
    {
        if (game->grid[0][colNum] != nextChess && game->grid[1][colNum] != nextChess && game->grid[2][colNum] != nextChess)
        {
            blankGrid = {};
            for (int rowNum = 0; rowNum < 3; rowNum++)
            {
                if (game->grid[rowNum][colNum] == '_')
                {
                    blankGrid.push_back({rowNum, colNum});
                }
            }
            if (blankGrid.size() == 2)
            {
                validAttackPositions.insert(validAttackPositions.end(), blankGrid.begin(), blankGrid.end());
            }
        }
    }

    if (game->grid[1][1] == '_') // note: if all cases of 2 connected chess are eliminated and middle grid is occupied, the game is forced to end in a draw.
    {

        blankGrid = {{1, 1}};
        if (game->grid[0][0] != nextChess && game->grid[2][2] != nextChess)
        {
            if (game->grid[0][0] == '_')
            {
                blankGrid.push_back({0, 0});
            }
            if (game->grid[2][2] == '_')
            {
                blankGrid.push_back({2, 2});
            }
            if (blankGrid.size() == 2)
            {
                validAttackPositions.insert(validAttackPositions.end(), blankGrid.begin(), blankGrid.end());
            }

            blankGrid = {{1, 1}};
            if (game->grid[2][0] == '_')
            {
                blankGrid.push_back({2, 0});
            }
            if (game->grid[0][2] == '_')
            {
                blankGrid.push_back({0, 2});
            }
            if (blankGrid.size() == 2)
            {
                validAttackPositions.insert(validAttackPositions.end(), blankGrid.begin(), blankGrid.end());
            }
        } 
    }

    cout << validAttackPositions.size() << endl;
    return validAttackPositions;


}

vector<vector<int>> getValidMovePosition(Game* game, char currChess, const char nextChess) // nextChess is the chess to be placed NEXT round
{
    vector<int> winablePosition = getWinablePosition(game, currChess);
    if (winablePosition.size() > 0)
    {
        cout << "winable position: " << winablePosition[0] << " " << winablePosition[1] << endl;
        return {winablePosition};
    }

    vector<int> opponentWinablePosition = getWinablePosition(game, nextChess);
    if (opponentWinablePosition.size() > 0)
    {
        cout << "opponent winable position: " << opponentWinablePosition[0] << " " << opponentWinablePosition[1] << endl;
        return {opponentWinablePosition};
    }

    vector<vector<int>> validAttackPositions = validAttack(game, currChess, nextChess);
    return validAttackPositions;
}

ListNode* backtrack(ListNode* end, Game* game, const Player* user, const Player* opponent, const map<int, Player*>* players, int rounds)
{
    if (rounds >= 4)
    {
        if (winCheck(game, opponent->symbol) || rounds > 9)
        {
            return nullptr;
        }
        else if (winCheck(game, user->symbol))
        {
            return end;
        }
    }

    char currChess = (*players).at((rounds & 1) + 1)->symbol;
    char nextChess = (*players).at(((rounds + 1) & 1) + 1)->symbol;
    vector<vector<int>> validMovePositions;
    validMovePositions = getValidMovePosition(game, currChess, nextChess);
    if (validMovePositions.size() == 0)
    {
        cout << "couldn't get anymore valid move position at round: " << rounds << endl;
        return nullptr;
    }

    cout << "Number of possible moves in round " << rounds << ": " << validMovePositions.size() << ", which are:" << endl;
    for (auto i : validMovePositions)
    {
        cout << i[0] << " " << i[1] << endl;
    }

    for (auto pos: validMovePositions)
        {
            ListNode* currhead = new ListNode(pos[0], pos[1], currChess, end);
            game->grid[pos[0]][pos[1]] = currChess;
            cout << "current grid: " << endl;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    cout << game->grid[i][j] << ' ';
                }
                cout << '\n';
            }
            currhead->next = backtrack(end, game, user, opponent, players, rounds + 1);
            if (currhead->next != nullptr)
            {
                return currhead;
            }
            else
            {
                delete currhead;
                game->grid[pos[0]][pos[1]] = '_';
            }

        }
    return nullptr;
}

int main()
{
    Game* game = Game::getInstance();
    Player* user;
    Player* opponent;
    map<int, Player*>* players = new map<int, Player*>();

    inputPlayer(user, opponent, players);
    inputGrid();

    ListNode* end;
    end = (ListNode*)malloc(sizeof(ListNode));
    if (end == nullptr)
    {
        cout << "malloc failed";
        return 1;
    }
    // Linked list / stack is viable data structure but I just chose the former one
    ListNode* head = backtrack(end, game, user, opponent, players, 0); // round 0 refers to no chess placed
    if (head == nullptr)
    {
        cout << "no solution found\n";
    }

    cout << "backtracking done\n";

    ofstream outputFile;
    outputFile.open("move.csv");
    while (head != end)
    {
        outputFile << head->rowNum << ' ' << head->colNum << ' ' << head->placedChess << endl;
        head = head->next;
    }
    outputFile.close();




    return 0;

}