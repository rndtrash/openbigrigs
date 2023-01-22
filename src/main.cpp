#include <stdexcept>

#include <spdlog/spdlog.h>

#include "game.h"

int main(int argc, char **argv)
{
	spdlog::info("Hellorld!");

	try
	{
		const auto game = std::make_unique<OBR::Game>(argc, argv);
		game->run();
	}
	catch (std::runtime_error& e)
	{
		spdlog::error("{}", e.what());
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
