
rows, cols = (8, 8)
 
board = [[0 for i in range(cols)] for j in range(rows)]
queue = []

# Brief implementation of a pop method for the list serving as our queue
# Better practice to define a separate object with a pop method for larger scopes
def pop(queue):
	item = queue[0]
	queue.remove(queue[0])
	return item

def knightMoves(Kx, Ky, jumpNum):
	if (jumpNum == 1):
		board[Kx][Ky] = 9
	for i in range (-2, 3):
		for j in range (-2, 3):
			if (i == 0 or j == 0 or abs(i) == abs(j)):
				continue
			if (0 <= Kx+i < rows and 0 <= Ky+j < cols):
				if (board[Kx+i][Ky+j] == 0):
					board[Kx+i][Ky+j] = jumpNum
					queue.append((Kx+i, Ky+j, jumpNum+1))

def knightJumpCount():
	x=0
	y=1
	jumps=2
	while (queue):
		location = pop(queue)
		#location = queue[0]
		#print("(" + str(location[x]) + ", " + str(location[y]) + ") =>" + str(location[jumps]))
		#queue.remove(queue[0])
		
		knightMoves(location[x], location[y], location[jumps])
	for row in board:
		print(row)

queue.append((7, 1, 1))
#queue.append((7, 6, 1))
knightJumpCount()