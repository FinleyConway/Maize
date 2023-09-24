#pragma once

namespace Maize {

	struct Tiles
	{

	};

	struct TilemapRendererComponent
	{
		sf::VertexArray tile;

		std::string sortingLayer = "Default";
		int32_t orderInLayer = 0;
	};

	class TilemapRendering
	{
	public:

	private:
	};

} // Maize