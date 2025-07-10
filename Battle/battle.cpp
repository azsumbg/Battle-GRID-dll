#include "pch.h"
#include "battle.h"


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
dll::FPOINT dll::GRID::GetTileDims(int tile_number)
{
	int myrow = tile_number / MAX_COLS;
	int mycol = tile_number - myrow * MAX_COLS;

	FPOINT ret{};

	ret.x = (float)(mycol * 50);
	ret.y = (float)(myrow * 50 + 50);

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