# TicTacToeSolution

### Launch "TicTacToeSolution.bat" to run the program in python;
### "TicTacToeSolutioncpp.bat" to run in c++ (not fully finished)

This program aims to provide steps that can lead to a win in a TicTacToe game, according
to the inputted game grids by the user.


(C++ version is still WIP, game grid has to be modified manually in the Game class for now, will be finished after I improve user interface with curses; I will also include solutions for games that are forced to draw, instead of prompting no result)


# User Guide

1. Input which player you are (1/2)  (player 1 is the one that starts first)
2. Input "s" to switch symbols that represent players
2. Input your current game by clicking arrow keys to select grid
3. Press "Enter" to place the current chess on the selected grid.
4. Click "q" when you finish inputting your current game.
5. After that, You can see the game grid of next round, which shows the move that you should take.
6. You can input "y" to continue checking the solution and "n" to quit the program.
7. Please note that the continued solution may not necessarily be the same as your game. If your opponent makes a move that is different than the program solution, you should quit the program and input the current game grid again to find winning moves.

    Note: the program MAY result in inaccurate result if you input the game grid if you are
    the one who made the last move. Input the game grid after your opponent made a move would
    eliminate such inaccuracies. <br/>
    E.g. (You are Player "O") <br/>
    Inputted game grid: <br/>
        O _ X <br/>
        _ _ O <br/>
        _ _ _ <br/>
    Output:<br/>
        No 100% win rate move in this situation!<br/>
        (However, this output is in fact correct, because if Player "X" places a chess in the middle, it must result in
        in a draw)

