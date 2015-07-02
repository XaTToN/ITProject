#ifndef __MAP_MGR_H__
#define __MAP_MGR_H__

#include <SFML\Graphics.hpp>
#include <vector>
#include <string>
#include <map>

class Game;

struct Tile
{
	sf::Sprite sprite;
	sf::Vector2<int> coords;

	sf::Vector2f TransformCoordinates();
};


class MapMgr
{
	friend class Game;
public:
	MapMgr(const std::string& mapfile);
private:
	std::vector<Tile> tiles;
	std::vector<Tile> selected_tiles;

public:
	void SelectTiles(const float& offset);
	void UpdateTilePositions(const float& offset);

	//Tiles
	static std::map<int, sf::Texture> tileset;
};

#endif //__MAP_MGR_H__