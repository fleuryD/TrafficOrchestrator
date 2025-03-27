#include "../include/traffic-orchestrator.hpp"


/**
* @brief
*
* @powershell:
*   clear; make;  if ($?) { .\orchestrator.exe }
*
*/





int	main() {

	if (!std::filesystem::exists("maps")) {
		if (std::filesystem::create_directory("maps")) {
			std::cout << "Dossier maps créé !" << std::endl;
		} else {
			std::cerr << "Erreur lors de la création du dossier maps." << std::endl;
		}
	}

	Game game(1, 1);

	UIPanelRoad uiPanelRoad;

	while (!WindowShouldClose()) {

		if (IsKeyPressed(game.config.keyBindings["Quit"])) {
			std::cout << "Quitting..." << std::endl;
        	break;
    	}

		game.update();
		game.updateMouseAndCursor();
		game.handleEvents(uiPanelRoad);

		BeginDrawing();

			if (IsWindowResized()) {
				game.screenWidth = GetScreenWidth();
				game.screenHeight = GetScreenHeight();
				//game.camera.offset = { game.screenWidth  / 2.0f, game.screenHeight / 2.0f };
				game.centerCameraOnNodes();
			}

			ClearBackground(BLACK);

			game.drawGrid();
			BeginMode2D(game.camera);
				game.drawMap();
			EndMode2D();
			game.drawUI();

			if (game.mode == M_CREATE_ROAD || game.selectedRoadId != -1) {
				game.drawUIPanelRoad(uiPanelRoad);
			}


			if (game.mode == M_MAP_OPEN) {
				game.drawMapSelector();
			}

		EndDrawing();

		game.frameCounter++;
	}

	CloseWindow();
	return 0;
}
