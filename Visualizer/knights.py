
# A breadth-first search of the minimum number of moves a knight needs to reach
# any given square on an nxm board

class Board:
	boardWidth = 8
	boardHeight = 8
	tiles = []

	# Constructor defaults to 8x8 board
	def __init__(self, width=8, height=8):
		self.boardWidth = width
		self.boardHeight = height
		self.tiles = [[0 for i in range(self.boardWidth)] for j in range(self.boardHeight)]


class Knight:
	board = []
	queue = []

	def __init__(self, width=8, height=8, startingPositions=[(7, 1),(7, 6)]):
		self.board = Board(width, height)
		for position in startingPositions:
			self.queue.append(position)
		self.knightJumpCount()

	# Brief implementation of a pop method for the list serving as our queue
	def pop(self):
		item = self.queue[0]
		self.queue.remove(self.queue[0])
		return item

	def knightMoves(self, Kx, Ky, jumpNum=1):
		if (jumpNum == 1):
			self.board.tiles[Kx][Ky] = 'K'
		for i in range (-2, 3):
			for j in range (-2, 3):
				if (i == 0 or j == 0 or abs(i) == abs(j)):
					continue
				if (0 <= Kx+i < self.board.boardHeight and 0 <= Ky+j < self.board.boardHeight):
					if (self.board.tiles[Kx+i][Ky+j] == 0):
						self.board.tiles[Kx+i][Ky+j] = jumpNum
						self.queue.append((Kx+i, Ky+j, jumpNum+1))
						#print("Checking [" + str(Kx+i) + "][" + str(Ky+j)  + "] = " + str(self.board.tiles[Kx+i][Ky+j]))

	def knightJumpCount(self):
		x=0
		y=1
		jumps=2
		while (self.queue):
			location = self.pop()
			#print(location)
			#location = queue[0]
			#print("(" + str(location[x]) + ", " + str(location[y]) + ") =>" + str(location[jumps]))
			#queue.remove(queue[0])
			if (len(location) == 2):
				location = (location[x], location[y], 1)

			self.knightMoves(location[x], location[y], location[jumps])
		for row in self.board.tiles:
			print(row)

# Knight in the center of a 15x15 board
Knight(15, 15, [(7, 7)])

# Knights at their starting positions of an 8x8 board
Knight()