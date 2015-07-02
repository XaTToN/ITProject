#include <DumpJump/Map/MapMgr.h>
#include <DumpJump/Shared.h>

#include <fstream>
#include <sstream>

std::map<int, sf::Texture> MapMgr::tileset;

MapMgr::MapMgr(const std::string& mapfile)
{
	//initialize tileset on first construction
	sf::Texture txt1;
	txt1.loadFromFile(PATH_RESSOURCES"tiles"OS_SEP"1.png");
	tileset.insert(std::pair<int, sf::Texture>(1, txt1));

	//parse given map-file
	std::ifstream infile(mapfile.c_str());

	std::string current_line;

	int coord_y = 0;
	int coord_x = 0;

	while (std::getline(infile, current_line))
	{
		for (unsigned int itr = 0; itr < current_line.length(); ++itr)
		{
			if (current_line[itr] == '1')
			{
				Tile t;
				t.sprite.setTexture(tileset[1]);
				t.sprite.setScale(2, 2);
				t.coords.x = coord_x;
				t.coords.y = coord_y;
				this->tiles.push_back(t);
			}

			coord_x++;
		}
		coord_x = 0;
		coord_y++;
	}
}
 
void MapMgr::SelectTiles(const float& offset)
{
	selected_tiles.clear();

	for (std::vector<Tile>::iterator itr = tiles.begin(); itr != tiles.end();)
	{
		float pos_x = itr->TransformCoordinates().x;

		if (pos_x - offset < -100)
		{
			itr = tiles.erase(itr);
			continue;
		}

		if (pos_x - offset > -100 && pos_x - offset < 1400)
		{
			selected_tiles.push_back(*itr);
		}

		++itr;
	}
}

void MapMgr::UpdateTilePositions(const float& offset)
{
	for (std::vector<Tile>::iterator itr = tiles.begin(); itr != tiles.end(); ++itr)
	{
		sf::Vector2f loc(itr->TransformCoordinates().x - offset, itr->TransformCoordinates().y);
		itr->sprite.setPosition(loc);
	}
}

sf::Vector2f Tile::TransformCoordinates()
{
	sf::Vector2f vec;
	vec.x = 64 * this->coords.x;
	vec.y = 64 * this->coords.y;

	return vec;
}