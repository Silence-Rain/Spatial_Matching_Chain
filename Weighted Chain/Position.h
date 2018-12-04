struct Position
{
	int x;
	int y;

	Position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Position(const Position& p)
	{
		this->x = p.x;
		this->y = p.y;
	}
};