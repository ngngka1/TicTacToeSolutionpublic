# Tic tac toe is a game where the first player has the chance to not lose 100% if he knows all winning combinations.

import psutil
import csv
import sys
import curses
import os

def backtrack(game: list, players: dict, rounds: int, user: str, opponent: str) -> list: # return a list of steps to win
    result = []
    if rounds >= 4 and win_check(game, user) and not win_check(game, opponent):
        for row in game:
            result.append(row[:]) # this is to avoid elements in result have the same reference to elements in game
        return [result]
    for row_num, row in enumerate(game):
        for col_num, element in enumerate(row):
            if element == "_" and (rounds <= 1 or valid_move_check(row_num, col_num, game[:][:], players, rounds)):
                game[row_num][col_num] = players[rounds % 2]
                result = backtrack(game, players, rounds + 1, user, opponent)
                game[row_num][col_num] = "_"
                if result:
                    temp = []
                    for row in game:
                        temp.append(row[:])
                    return [temp] + result
    return result
            
def win_check(game: list, player: str) -> bool: # return true/false if the player has won
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

def checkmated_position(game: list, player: str) -> list: # return the position of winable position
    pos = []
    # this loops through each grid in game, and check if placing a chess on that grid
    # can result in a win, if yes, return the position of the grid
    for i, row in enumerate(game):
        for j, element in enumerate(row):
            if element == "_":
                game[i][j] = player
                if win_check(game, player):
                    pos = (i, j)
                game[i][j] = "_"
            if pos:
                return pos
    return pos 

def valid_attack_check(target_row: int, target_col: int, game: list, opponent: str) -> bool: # return true if the target_row, target_col is a valid attack position
    for i, row in enumerate(game):
        for j, element in enumerate(row):
            if element == "_":
                # The sets below are used to check whether 3 grids in horizontal, vertical or diagonal
                # contain only "_" and the chess of the attacking player "O/X"
                rowset = set([game[i][j + 1 - len(row)], game[i][j + 2 - len(row)], element]) # "-" horizontal
                colset = set([game[i + 1 - len(game)][j], game[i + 2 - len(game)][j], element]) # "|" vertical
                descending_cross = set([game[i + 1 - len(game)][j + 1 - len(row)], game[i + 2 - len(game)][j + 2 - len(row)], element]) # "\" diagonal
                ascending_cross = set([game[len(game) - i - 1][j + 1 - len(row)], game[len(game) - i - 2][j + 2 - len(row)], element]) # "/" diagonal
                if (
                    # have to add an addition check "opponenet not in" to avoid invalid move examples 
                    # like placing a chess "O" in _ _ X
                    (opponent not in rowset and len(rowset) == 2)
                    or (opponent not in colset and len(colset) == 2) 
                    or (i == j and opponent not in descending_cross and len(descending_cross) == 2)
                    or (abs(i-j) == 2 and opponent not in ascending_cross and len(ascending_cross) == 2)
                    ):
                    if (i, j) == (target_row, target_col):
                        return True
    return False

def valid_move_check(row_num: int, col_num: int, game: list, players: dict, rounds: int) -> bool: # this function returns true if the move is valid(block or attack)
    # priority of moves:
    # 1. win
    # 2. block opponent who is going to win
    # 3. make constructive moves(attack) where 3 consecutive grid contain curr_player and does not contain opponent
    winable_pos = checkmated_position(game[:][:], players[rounds % 2])
    if winable_pos and winable_pos == (row_num, col_num):
        return True
    elif winable_pos:
        return False
    
    opponent_winnable_pos = checkmated_position(game[:][:], players[(rounds + 1) % 2]) # check if the opponent is checkmating you
    if opponent_winnable_pos and opponent_winnable_pos == (row_num, col_num):
        return True
    elif opponent_winnable_pos:
        return False
    else:
        return valid_attack_check(row_num, col_num, game[:][:], players[rounds % 2], players[(rounds + 1) % 2])
   
def input_gamegrid(stdscr, game: list, players: dict) -> int: # this function modifies game and return the current round number
    stdscr = curses.initscr()
    stdscr.nodelay(0)
    curses.curs_set(0)
    row_num = 0
    col_num = 0
    rounds = 0
    game[row_num][col_num] = "{" + game[row_num][col_num] +"}" # selecting cursor
    inputting = True
    while inputting:
        stdscr.clear()
        stdscr.addstr(0, 0, "Current Game Grid:     (Input \"q\" to exit)")
        stdscr.addstr(1, 0, "")
        stdscr.addstr(2, 0, f"Chess to be placed: {players[rounds % 2]}")
        for i, row in enumerate(game, 0):
            stdscr.addstr(3 + i, 0, " ".join(list(element.center(3) for element in row)))
        stdscr.refresh()
        key = stdscr.getch()
        if key == ord("q"):
            game[row_num][col_num] = game[row_num][col_num][1]
            break
        
        if key == curses.KEY_RIGHT and col_num + 1 < 3:
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

        if key == ord("\n") and "_" in game[row_num][col_num] :
            game[row_num][col_num] = players[rounds % 2]
            rounds += 1
            if col_num < 2:
                col_num += 1
            elif row_num < 2:
                row_num += 1
            else:
                if rounds >= 8:
                    break
                row_num = 0
                col_num = 0
            game[row_num][col_num] = "{" + game[row_num][col_num] +"}"
            
    curses.endwin()
    return rounds
        
def input_user(players: dict) -> tuple:
    inputting = True
    while inputting:
        os.system('cls||clear')
        print("Symbols for:        (Input \"s\" if you want to switch the symbols for players)")
        print("Player 1: " + players[0])
        print("Player 2: " + players[1])
        print()
        selection = (input("Are you Player 1 or 2? (1/2/s)"))
        if selection == "1":
            return players[0], players[1]
        elif selection == "2":
            return players[1], players[0]
        elif selection == "s":
            players[0], players[1] = players[1], players[0]
        else:
            print("Invalid input! Please input again.")
    
def print_result(result, rounds, print_index) -> None:
    print(f"Round {rounds + print_index}:")
    for row in result[print_index]:
        print(" ".join(row))
        
def init_file() -> None:
    stepfile = open("step.csv", "w")
    stepfile.close()
    permufile = open("move.csv", "w")
    permufile.close()
    
def main():
    init_file() # the files were purely for my documentation purpose
    game = [
        ["_", "_", "_"],
        ["_", "_", "_"],
        ["_", "_", "_"]
    ]
    
    players = {
        0: "O", # player 1
        1: "X" # player 2
    }
    
    user, opponent = input_user(players)
    starting_rounds = curses.wrapper(input_gamegrid, game, players)
    result = backtrack(game[0:], players, starting_rounds, user, opponent)
    print_index = 0 # this is to keep track of the index of result to be outputted
    #Note: result only stores steps from the ongoing round to end

    if result:   
        with open("step.csv", "a", newline='') as file: # simply writing to file for documentation, can be removed
            writer = csv.writer(file, delimiter=" ")
            writer.writerow(f"You are Player 1 ({user})".split())
            for i, gamegrid in enumerate(result, starting_rounds):
                writer.writerow(f"Round {i}:".split())
                for row in gamegrid:
                    writer.writerow(row)
                    
        with open("move.csv", "a", newline='') as file: # simply writing to file for documentation, can be removed
            writer = csv.writer(file, delimiter=" ")
            index += 1
            for row in result[index + 1]:
                writer.writerow(row)
            file.write("\n")
            
        os.system('cls||clear')
        print_result(result, starting_rounds, print_index)
        continuecheck = True
        while continuecheck:
            if print_index + 1 < len(result):
                cont = input("Continue to check solutions for this game grid? (y/n)")
            else:
                input()
                sys.exit()
                
            if cont == "y":
                print_index += 1
                os.system('cls||clear')
                print_result(result, starting_rounds, index)
            elif cont == "n":
                sys.exit()
            else:
                print("Invalid input! Please input again.")
    else:
        print("No 100% win rate move in this situation!")
        input()
        sys.exit()
    
if __name__ == "__main__":
    #process = psutil.Process()
    main()
    #memory_usage = process.memory_info().rss / 1024 / 1024 # Convert to kilobytes
    #print("Memory usage:", memory_usage, "MB")