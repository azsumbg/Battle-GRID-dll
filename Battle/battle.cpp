#include "pch.h"
#include "battle.h"

// GRID **************************************

dll::GRID::GRID()
{
	for (int cols = 0; cols < MAX_COLS; ++cols)
	{
		for (int rows = 0; rows < MAX_ROWS; ++rows)
		{
			grid[cols][rows].col = cols;
			grid[cols][rows].row = rows;

			grid[cols][rows].start.x = cols * 50.0f;
			grid[cols][rows].end.x = cols * 50.0f + 49.0f;

			grid[cols][rows].start.y = rows * 50.0f + 50.0f;
			grid[cols][rows].end.y = rows * 50.0f + 99.0f;

			grid[cols][rows].number = rows * MAX_COLS + cols;
		}
	}
}
dll::TILE dll::GRID::GetTileDims(int tile_number)
{
	int myrow = tile_number / MAX_COLS;
	int mycol = tile_number - myrow * MAX_COLS;

	TILE ret{};

	ret.start.x = (float)(mycol * 50);
	ret.start.y = (float)(myrow * 50 + 50);
	ret.end.x = ret.start.x + 49.0f;
	ret.end.y = ret.start.y + 49.0f;
	
	ret.col = mycol;
	ret.row = myrow;
	ret.number = tile_number;

	return ret;
}
int dll::GRID::GetTileNumber(FPOINT position)
{
	int myrow = (int)((position.y - 50.0f) / 50.0f);
	int mycol = (int)(position.x / 50.0f);

	return myrow * MAX_COLS + mycol;
}
int dll::GRID::GetTileCol(float x_position) const
{
	return (int)(x_position / 50.0f);
}
int dll::GRID::GetTileRow(float y_position) const
{
	return (int)((y_position - 50.0f) / 50.0f);
}
bool dll::GRID::IsAvailable(int tile_number)const
{
	int my_row = tile_number / 18;
	int my_col = tile_number - my_row * MAX_COLS;

	if (tile_number < 0 || tile_number > 238 || my_row < 0 || my_row >= MAX_ROWS ||
		my_col < 0 || my_col >= MAX_COLS || grid[my_col][my_row].state != content::free)return false;

	return true;
}
void dll::GRID::Release()
{
	delete this;
}

/////////////////////////////////////////////

// SHIP ************************************

void dll::SHIP::sort_tiles()
{
	if (ship_tile[1].state == content::free)return;

	if (type == ships::small_ship)
	{
		if (dir == dirs::hor)
		{
			if (ship_tile[0].col > ship_tile[1].col)
			{
				TILE tmp_tile = ship_tile[0];
				ship_tile[0] = ship_tile[1];
				ship_tile[1] = tmp_tile;
				return;
			}
		}
		else if (dir == dirs::vert)
		{
			if (ship_tile[0].row > ship_tile[1].row)
			{
				TILE tmp_tile = ship_tile[0];
				ship_tile[0] = ship_tile[1];
				ship_tile[1] = tmp_tile;
				return;
			}
		}
	}
	else if (type == ships::mid_ship1 || type == ships::mid_ship2)
	{
		if (dir == dirs::hor)
		{
			bool ok = false;
			while (!ok)
			{
				ok = true;

				for (int i = 0; i < 2; ++i)
				{
					if (ship_tile[i].col > ship_tile[i + 1].col)
					{
						TILE tmp_tile = ship_tile[i];
						ship_tile[i] = ship_tile[i + 1];
						ship_tile[i + 1] = tmp_tile;
						ok = false;
						break;
					}
				}
			}
		}
		else if (dir == dirs::vert)
		{
			bool ok = false;
			while (!ok)
			{
				ok = true;

				for (int i = 0; i < 2; ++i)
				{
					if (ship_tile[i].row > ship_tile[i + 1].row)
					{
						TILE tmp_tile = ship_tile[i];
						ship_tile[i] = ship_tile[i + 1];
						ship_tile[i + 1] = tmp_tile;
						ok = false;
						break;
					}
				}
			}
		}
	}
	else if (type == ships::big_ship1 || type == ships::big_ship2)
	{
		if (dir == dirs::hor)
		{
			bool ok = false;
			while (!ok)
			{
				ok = true;

				for (int i = 0; i < 3; ++i)
				{
					if (ship_tile[i].col > ship_tile[i + 1].col)
					{
						TILE tmp_tile = ship_tile[i];
						ship_tile[i] = ship_tile[i + 1];
						ship_tile[i + 1] = tmp_tile;
						ok = false;
						break;
					}
				}
			}
		}
		else if (dir == dirs::vert)
		{
			bool ok = false;
			while (!ok)
			{
				ok = true;

				for (int i = 0; i < 3; ++i)
				{
					if (ship_tile[i].row > ship_tile[i + 1].row)
					{
						TILE tmp_tile = ship_tile[i];
						ship_tile[i] = ship_tile[i + 1];
						ship_tile[i + 1] = tmp_tile;
						ok = false;
						break;
					}
				}
			}
		}
	}
}
dll::SHIP::SHIP(ships _type, TILE* _ship_tile, int size, dirs _to_where)
{
	type = _type;
	dir = _to_where;
	for (int i = 0; i < size; ++i)ship_tile[i] = _ship_tile[i];

	sort_tiles();

	switch (type)
	{
	case ships::small_ship:
		++ship_parts_alive;
		break;

	case ships::mid_ship1:
		ship_parts_alive += 2;
		break;

	case ships::mid_ship2:
		ship_parts_alive += 2;
		break;

	case ships::big_ship1:
		ship_parts_alive += 3;
		break;

	case ships::big_ship2:
		ship_parts_alive += 3;
		break;

	}
}
int dll::SHIP::ship_healt() const
{
	return ship_parts_alive;
}
void dll::SHIP::hit_ship(int which_part)
{
	--ship_parts_alive;
	ship_tile[which_part].state = content::explosion;
}
dll::ships dll::SHIP::get_type()const
{
	return type;
}
void dll::SHIP::Release()
{
	delete this;
}

/////////////////////////////////////////////

// FUNCTIONS ********************************

void dll::SetNearTiles(GRID& set_grid, TILE* ship, int size)
{
	for (int i = 0; i < size; ++i)
	{
		int this_col = ship[i].col;
		int this_row = ship[i].row;

		set_grid.grid[this_col][this_row].state = content::used;

		if (i == 0)
		{
			if (this_col > 0)set_grid.grid[this_col - 1][this_row].state = content::used;
			if (this_row > 0)set_grid.grid[this_col][this_row - 1].state = content::used;
			if (this_row < MAX_ROWS - 1)set_grid.grid[this_col][this_row + 1].state = content::used;
		}
		else if (i == size - 1)
		{
			if (this_col < MAX_COLS - 1)set_grid.grid[this_col + 1][this_row].state = content::used;
			if (this_row > 0)set_grid.grid[this_col][this_row - 1].state = content::used;
			if (this_row < MAX_ROWS - 1)set_grid.grid[this_col][this_row + 1].state = content::used;
		}
		else
		{
			if (this_row > 0)set_grid.grid[this_col][this_row - 1].state = content::used;
			if (this_row < MAX_ROWS - 1)set_grid.grid[this_col][this_row + 1].state = content::used;
		}
	}
}

dll::SHIP* dll::ShipFactory(ships what, TILE* myTiles, int size, dirs where,GRID& host_grid)
{
	SHIP* ret{ nullptr };

	ret = new SHIP(what, myTiles, size, where);

	dll::SetNearTiles(host_grid, myTiles, size);

	return ret;
}