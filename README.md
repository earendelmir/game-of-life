# GAME OF LIFE

A command line implementation of the Game of Life.

## HOW IT WORKS:
A **cell** is a grid element that can either be dead or alive.<br>
A **neighbour** is an adiacent cell.<br>
Each cell has 8 neighbours.

Given a grid of cells - called *population* - the algorithm calculates
the life span of each individual cell according to the number of alive neighbours.

Each new iteration forms a new *generation*.

## THE RULES:
+ Any *live cell* with less than 2 alive neighbours dies by *isolation*.
+ Any *live cell* with more than 3 alive neighbours dies by *overpopulation*.
+ Any *dead cell* with three live neighbours becomes alive.
+ Any other cell remains as it is.
