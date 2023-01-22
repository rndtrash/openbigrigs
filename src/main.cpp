#include <stdexcept>

#include <spdlog/spdlog.h>

#include "game.h"

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	spdlog::info("Hellorld!");

	try
	{
		const auto game = std::make_unique<OBR::Game>();
		game->run();
	}
	catch (std::runtime_error& e)
	{
		spdlog::error("{}", e.what());
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
