#pragma once

#ifdef BATTLE_EXPORTS
#define BATTLE_API __declspec(dllexport)
#else
#define BATTLE_API __declspec(dllimport)
#endif

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr int MAX_COLS{ 18 };
constexpr int MAX_ROWS{ 15 };

namespace dll
{
	enum class BATTLE_API positions { vert = 0, hor = 1 };
	
	enum class BATTLE_API ships { small_ship = 0, mid_ship = 1, big_ship = 2, battleship = 3 };

	enum class BATTLE_API content { free = 0, used = 1, near_ship = 2 };

	struct BATTLE_API FPOINT
	{
		float x{ 0 };
		float y{ 0 };
	};
	struct BATTLE_API TILE
	{
		FPOINT start{};
		FPOINT end{};

		int number = 0;
		int col{ 0 };
		int row{ 0 };
		
		content state = content::free;
	};

	class BATTLE_API GRID
	{
	private:
		float width{ 50.0f };
		float height{ 50.0f };

	public:
		TILE grid[18][15]{};

		GRID();

		FPOINT GetTileDims(int tile_number);
		int GetTileNumber(FPOINT position);
		int GetTileCol(float x_position) const;
		int GetTileRow(float y_position) const;
	};

}