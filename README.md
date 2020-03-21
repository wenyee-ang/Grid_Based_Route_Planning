# Grid_Based_Route_Planning
A route planning program which takes the dimensions of a grid as its input and plans a route from a starting point to an end point while avoiding obstacles. Written in C by Wen Yee ANG as part of an assignment for COMP10002 Foundations of Algorithms at the University of Melbourne in Semester 2, 2019.

Overall mark of 14.5/15.

The program functions in the following ways:
- Reads input from stdin. The first line of the input encodes the dimensions of the grid, the second line of the input encodes the
initial cell of the agent in the grid, while the third line encodes the goal cell. A cell is encoded using the format
[r,c], where r and c are numbers that stand for, respectively, the row and column of the cell. Subsequent input lines specify positions of blocks in the grid, one block cell per line. An input line with a single character $ denotes the end of block lines. The input lines that follow encode a route of the agent in the grid. A route is encoded by alternating cells and ->, for example [0,0]->[0,1]->[0,2].
- A status check is performed on the given route and outputted to the user (valid, invalid, blocked).
- Visualize the initial input grid and route using ASCII characters. This is printed out.
- If the route is broken, a repair is attempted, and the repaired route visualized in the same way as above.
- If the route could not be repaired, the message: "The route cannot be repaired!" is printed.
- This program can also handle multiple changes of block positions in the grid.
