# It is planned that this program will be converted into javascript and user can input the game grid easier with a webpage layout.

# Tic tac toe is a game where the first player has the chance to not lose 100% if he knows all winning combinations.
import time
import psutil
import csv
import sys
import curses

            
def backtrack(game: list, players: dict, rounds: int, user: str) -> list:
    result = []
    if rounds >= 4 and win_check(game, user) and not win_check(game, players[1]):
        for row in game:
            result.append(row[:]) # this is to avoid elements in result have the same reference to elements in game
        return [result]
    for row_num, row in enumerate(game):
        for col_num, element in enumerate(row):
            if element == "_" and (rounds == 0 or valid_move(row_num, col_num, game[:][:], players, rounds)):
                game[row_num][col_num] = players[rounds % 2]
                result = backtrack(game, players, rounds + 1, user)
                game[row_num][col_num] = "_"
                if result:
                    temp = []
                    for row in game:
                        temp.append(row[:])
                    return [temp] + result
    return result
            
def win_check(game: list, player: str):
    for row in game: # check horizontal win
        if row[0] == row[1] == row[2] == player:
            return True
        
    if (game[0][0] == game[1][0] == game[2][0] == player # check vertical win
    or game[0][1] == game[1][1] == game[2][1] == player
    or game[0][2] == game[1][2] == game[2][2] == player):
        return True
    
    if (game[0][0] == game[1][1] == game[2][2] == player # check diagonal win
    or game[0][2] == game[1][1] == game[2][0] == player):
        return True
    
    return False

def check_checkmated(row_num, col_num, game: list, player): # this return the pos the player has to block the opponent from winning
    pos = []
    for i, row in enumerate(game):
        for j, element in enumerate(row):
            if game[i][j] == "_":
                game[i][j] = player
                if win_check(game, player):
                    pos = (i, j)
                game[i][j] = "_"
            if pos:
                return pos
    return pos # return False because don't need to block the opponent

def attack(target_row, target_col, game, curr_player, opponent): #
    for i, row in enumerate(game):
        for j, element in enumerate(row):
            if game[i][j] == "_":
                rowset = set([game[i][j + 1 - len(row)], game[i][j + 2 - len(row)], element])
                colset = set([game[i + 1 - len(game)][j], game[i + 2 - len(game)][j], element])
                descending_cross = set([game[i + 1 - len(game)][j + 1 - len(row)], game[i + 2 - len(game)][j + 2 - len(row)], element])
                ascending_cross = set([game[len(game) - i - 1][j + 1 - len(row)], game[len(game) - i - 2][j + 2 - len(row)], element])
                if (
                    # approach of checking set len == 2 works because cases like XX_ and OOX are eliminated beforehand
                    (opponent not in rowset and len(rowset) == 2) # horizontal
                    or (opponent not in colset and len(colset) == 2) # vertical
                    or (i == j and opponent not in descending_cross and len(descending_cross) == 2)
                    or (abs(i-j) == 2 and opponent not in ascending_cross and len(ascending_cross) == 2)
                    ):
                    if (i, j) == (target_row, target_col):
                        return True
    return False

def valid_move(row_num, col_num, game, players, rounds): # this function returns true if the move is valid(block or attack)
    # priority of moves:
    # 1. win
    # 2. block opponent who is going to win
    # 3. make constructive moves(attack) where 3 consecutive grid contain curr_player and does not contain opponent
    winable_pos = check_checkmated(row_num, col_num, game[:][:], players[rounds % 2])
    if winable_pos and winable_pos == (row_num, col_num):
        return True
    elif winable_pos:
        return False
    
    opponent_winnable_pos = check_checkmated(row_num, col_num, game[:][:], players[(rounds + 1) % 2]) # check if the opponent is checkmating you
    if opponent_winnable_pos and opponent_winnable_pos == (row_num, col_num):
        return True
    elif opponent_winnable_pos:
        return False
    else:
        return attack(row_num, col_num, game[:][:], players[rounds % 2], players[(rounds + 1) % 2])
   
def input_gamegrid(stdscr, game: list, players: dict):
    stdscr = curses.initscr()
    stdscr.nodelay(0)
    row_num = 0
    col_num = 0
    rounds = 0
    game[row_num][col_num] = "{" + game[row_num][col_num] +"}" # selecting cursor
    inputting = True
    while inputting:
        stdscr.clear()
        stdscr.addstr(0, 0, "Current Game Grid: ")
        for i, row in enumerate(game, 0):
            stdscr.addstr(1 + i, 0, " ".join(row))
        stdscr.addstr(2 + i, 0, f"Chess to be placed next: {players[rounds % 2]}")
        stdscr.refresh()
        key = stdscr.getch()
        if key == ord("q"):
            game[row_num][col_num] = game[row_num][col_num][1]
            break
        elif key == curses.KEY_RIGHT and col_num + 1 < 3:
            game[row_num][col_num] = game[row_num][col_num][1]
            col_num += 1
            game[row_num][col_num] = "{" + game[row_num][col_num] +"}"
        elif key == curses.KEY_LEFT and col_num > 0:
            game[row_num][col_num] = game[row_num][col_num][1]
            col_num -= 1
            game[row_num][col_num] = "{" + game[row_num][col_num] +"}"
        elif key == curses.KEY_DOWN and row_num + 1 < 3:
            game[row_num][col_num] = game[row_num][col_num][1]
            row_num += 1
            game[row_num][col_num] = "{" + game[row_num][col_num] +"}"
        elif key == curses.KEY_UP and row_num > 0:
            game[row_num][col_num] = game[row_num][col_num][1]
            row_num -= 1
            game[row_num][col_num] = "{" + game[row_num][col_num] +"}"
        if key == ord("\n"):
            game[row_num][col_num] = players[rounds % 2]
            rounds += 1
            if col_num < 2:
                col_num += 1
            elif row_num < 2:
                row_num += 1
            else:
                break
            game[row_num][col_num] = "{" + game[row_num][col_num] +"}"
            
    curses.endwin()
    return rounds
        
def prompt(game: list, players: dict):
    print("The Current Game Grid:")
    for row in game:
        print(" ".join(row))
    inputting = True
    while inputting:
        print("Player 1: " + players[0])
        print("Player 2: " + players[1])
        selection = (input("Are you Player 1 or 2? (1/2/s)"))
        print("(input s if you want to switch the symbols for players)")
        if selection == "1":
            return players[0]
        elif selection == "2":
            return players[1]
        elif selection == "s":
            players[0], players[1] = players[1], players[0]
        else:
            print("Invalid input! Please input again.")
    
def print_result(result, rounds):
    if rounds < len(result):
        print(f"Round {rounds}:")
        for row in result[rounds]:
            print(" ".join(row))
    else:
        sys.exit()
    
          
def init_file():
    stepfile = open("step.csv", "w")
    stepfile.close()
    permufile = open("move.csv", "w")
    permufile.close()
    
def main():
    #time_start = round(time.time(), 8)
    #process = psutil.Process()
    init_file()
    game = [
        ["_", "_", "_"],
        ["_", "_", "_"],
        ["_", "_", "_"]
    ]
    
    players = {
        0: "O", # player 1
        1: "X" # player 2
    }
    
    starting_rounds = curses.wrapper(input_gamegrid, game, players)
    user = prompt(game, players)
    result = backtrack(game[0:], players, starting_rounds, user)
    index = 0 # this is to keep track of the index of result to be outputted
    #Note: result stores steps from the ongoing round to end
    
    if result:   
        with open("step.csv", "a", newline='') as file:
            writer = csv.writer(file, delimiter=" ")
            writer.writerow(f"You are Player 1 ({user})".split())
            for i, gamegrid in enumerate(result, starting_rounds):
                writer.writerow(f"Round {i}:".split())
                for row in gamegrid:
                    writer.writerow(row)
                    
        with open("move.csv", "a", newline='') as file:
            writer = csv.writer(file, delimiter=" ")
            index += 1
            for row in result[index]:
                writer.writerow(row)
            file.write("\n")
        print_result(result, index)
    else:
        print("No 100% win rate move in this situation!")
        
    checking = True
    while checking:
        cont = input("Continue to check solutions? (y/n)")
        if cont == "y":
            index += 1
            print_result(result, index)
        elif cont == "n":
            checking = False
            sys.exit()
        else:
            print("Invalid input! Please input again.")

    #time_elapsed = (round(time.time(), 8) - time_start) * 1000
    #print(f"%.4f ms" % time_elapsed)
    #memory_usage = process.memory_info().rss / 1024 / 1024 # Convert to kilobytes
    #print("Memory usage:", memory_usage, "MB")
    
if __name__ == "__main__":
    main()