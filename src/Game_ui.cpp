#include "../include/traffic-orchestrator.hpp"


// ! DRAW & HANDLE
void	Game::drawUIPanelRoad(UIPanelRoad &ui) {
	// * Dessiner le panneau
	DrawRectangleRec(ui.panelRec, LIGHTGRAY);
	DrawRectangleLinesEx(ui.panelRec, 2, DARKGRAY);

	float currY = ui.panelRec.y + 10;

	if (selectedRoadId == -1)
		DrawText("CREATE ROAD", ui.panelRec.x + 10,currY, 20, BLACK);
	else
		DrawText(TextFormat("EDIT ROAD # %d", selectedRoadId), ui.panelRec.x + 10,currY, 20, BLACK);
	currY += 30;
	// * Contrôle de la vitesse limite

	// Boutons "+" et "-"
	Rectangle btnSpeedMinus = { ui.panelRec.x + 10, currY, 20, 20 };
	Rectangle btnSpeedPlus =  { btnSpeedMinus.x + 30,    currY, 20, 20 };
	DrawRectangleRec(btnSpeedMinus, DARKGRAY);
	DrawRectangleRec(btnSpeedPlus, DARKGRAY);
	DrawText("-", btnSpeedMinus.x + 7, btnSpeedMinus.y + 2, 20, WHITE);
	DrawText("+", btnSpeedPlus.x + 7, btnSpeedPlus.y + 2, 20, WHITE);
	DrawText(TextFormat("%d km/h", ui.speedLimit), btnSpeedPlus.x + 40, currY, 20, RED);

	// Gestion des clics sur les boutons
	if (CheckCollisionPointRec(GetMousePosition(), btnSpeedMinus) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ui.speedLimit > 10) {
		ui.speedLimit -= 10;
		if (selectedRoadId != -1){
			roads[selectedRoadId]->speedLimit = ui.speedLimit;
			asUnsavedChanges = true;
		}
	}
	if (CheckCollisionPointRec(GetMousePosition(), btnSpeedPlus) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ui.speedLimit < 200) {
		ui.speedLimit += 10;
		if (selectedRoadId != -1){
			roads[selectedRoadId]->speedLimit = ui.speedLimit;
			asUnsavedChanges = true;
		}
	}

	currY+=30;

	// * Contrôle de nombre de lanes

	// Boutons "+" et "-"
	Rectangle btnLanesMinus = { ui.panelRec.x + 10, currY, 20, 20 };
	Rectangle btnLanesPlus =  { btnLanesMinus.x + 30, currY, 20, 20 };
	DrawRectangleRec(btnLanesMinus, DARKGRAY);
	DrawRectangleRec(btnLanesPlus, DARKGRAY);
	DrawText("-", btnLanesMinus.x + 7, btnLanesMinus.y + 2, 20, WHITE);
	DrawText("+", btnLanesPlus.x + 7, btnLanesPlus.y + 2, 20, WHITE);
	DrawText(TextFormat("%d lane(s)", ui.lanesCount), btnLanesPlus.x + 40, currY, 20, RED);

	// Gestion des clics sur les boutons
	if (CheckCollisionPointRec(GetMousePosition(), btnLanesMinus) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ui.lanesCount > 1) {
		ui.lanesCount -= 1;
		if (selectedRoadId != -1){
			roads[selectedRoadId]->lanesPtr.pop_back();
			roads[selectedRoadId]->caclRec();
			asUnsavedChanges = true;
		}
	}
	if (CheckCollisionPointRec(GetMousePosition(), btnLanesPlus) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ui.lanesCount < 6) {
		ui.lanesCount += 1;
		if (selectedRoadId != -1){
			roads[selectedRoadId]->lanesPtr.push_back(std::make_unique<Lane>(*roads[selectedRoadId], ui.lanesCount -1));
			roads[selectedRoadId]->caclRec();
			asUnsavedChanges = true;
		}
	}

	currY+=30;



	if (selectedRoadId == -1){

		// * Bouton à cocher (création en sens inverse)
		Rectangle checkbox = { ui.panelRec.x + 10, currY, 20, 20 };
		DrawRectangleRec(checkbox, WHITE);
		DrawRectangleLinesEx(checkbox, 2, BLACK);
		if (ui.reverseRoad){
			DrawLine(checkbox.x, checkbox.y, checkbox.x + checkbox.width, checkbox.y + checkbox.height, BLACK);
			DrawLine(checkbox.x+1, checkbox.y, checkbox.x + checkbox.width+1, checkbox.y + checkbox.height, BLACK);
			DrawLine(checkbox.x-1, checkbox.y, checkbox.x + checkbox.width-1, checkbox.y + checkbox.height, BLACK);
			DrawLine(checkbox.x + checkbox.width, checkbox.y, checkbox.x, checkbox.y + checkbox.height, BLACK);
			DrawLine(checkbox.x+1 + checkbox.width, checkbox.y, checkbox.x+1, checkbox.y + checkbox.height, BLACK);
			DrawLine(checkbox.x-1 + checkbox.width, checkbox.y, checkbox.x-1, checkbox.y + checkbox.height, BLACK);
		}

		DrawText("Both directions", checkbox.x + 30, checkbox.y + 2, 20, BLACK);

		// Gestion du clic sur le bouton à cocher
		if (CheckCollisionPointRec(GetMousePosition(), checkbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			ui.reverseRoad = !ui.reverseRoad;
		}

	}
	else{
		// * bouton supprimer:
		Rectangle btnDelete = { ui.panelRec.x + 10, currY, ui.panelRec.width-20, 20 };
		DrawRectangleRec(btnDelete, RED);
		DrawText("DELETE ROAD", btnDelete.x + 10, btnDelete.y + 5, 15, WHITE);
		if (CheckCollisionPointRec(GetMousePosition(), btnDelete) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			roads[selectedRoadId]->isSelected = false;
			//roads[selectedRoadId]->start = 0;
			//roads[selectedRoadId]->end = 0;
			//roads[selectedRoadId]->lanesPtr.clear();
			roads.erase(roads.begin() + selectedRoadId);
			selectedRoadId = -1;

		}

	}



	/*
	// Bouton de validation
	Rectangle btnCreate = { ui.panelRec.x + 10, ui.panelRec.y + 100, 200, 30 };
	DrawRectangleRec(btnCreate, BLUE);
	DrawText("Créer route", btnCreate.x + 50, btnCreate.y + 5, 20, WHITE);

	// Action lors du clic sur "Créer route"
	if (CheckCollisionPointRec(GetMousePosition(), btnCreate) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		std::cout << "Création de la route, sens inverse: " << (ui.reverseRoad ? "OUI" : "NON")
				<< ", Vitesse limite: " << ui.speedLimit << " km/h" << std::endl;
	}
	*/
}

void	Game::drawUI() {
	DrawRectangle(0, 0, screenWidth, ui.navHeaderHeight, LIGHTGRAY);
	DrawRectangleLines(0, 0, screenWidth, ui.navHeaderHeight, WHITE);

	// DrawText("T.Orch", 20, 20, 40, DARKBLUE);

	drawBtn(ui.btnMapNew.rec, ui.btnMapNew.text,		ui.btnFontSize, false);
	drawBtn(ui.btnMapOpen.rec, ui.btnMapOpen.text,		ui.btnFontSize, mode == M_MAP_OPEN);
	drawBtn(ui.btnMapSave.rec, ui.btnMapSave.text,		ui.btnFontSize, asUnsavedChanges == true);
	drawBtn(ui.btnMapSaveAs.rec, ui.btnMapSaveAs.text,	ui.btnFontSize, false);
	drawBtn(ui.btnAddRoad.rec, ui.btnAddRoad.text,		ui.btnFontSize, mode == M_CREATE_ROAD);

	DrawText(TextFormat("Show Help: [%s]", getKeyNameFromKey(config.keyBindings["Help"]).c_str()),	ui.btnAddRoad.rec.x + ui.btnAddRoad.rec.width + 30, 15, ui.btnFontSize, BLACK);

	drawDebug();
	drawHelpPanel();
}

void	Game::initUi(){

	ui.btnFontSize = 18;

	int btnTextMargin = 4;
	float btnHeight = ui.btnFontSize + btnTextMargin*2;

	ui.navHeaderHeight = btnHeight + 20;

	ui.btnMapNew.text		= "New";
	//ui.btnMapNew.text		= TextFormat("New (%s)", getKeyNameFromKey(config.keyBindings["MapNew"]).c_str());
	ui.btnMapNew.rec		= { 20, 10,  80, btnHeight};
	ui.btnMapNew.rec.width	= MeasureText(ui.btnMapNew.text.c_str(), ui.btnFontSize) + btnTextMargin*2;

	ui.btnMapOpen.text		= "Open";
	//ui.btnMapOpen.text	= TextFormat("Open (%s)", getKeyNameFromKey(config.keyBindings["MapOpen"]).c_str());
	ui.btnMapOpen.rec	= {150, 10,  80, btnHeight};
	ui.btnMapOpen.rec.width	= MeasureText(ui.btnMapOpen.text.c_str(), ui.btnFontSize) + btnTextMargin*2;
	ui.btnMapOpen.rec.x = ui.btnMapNew.rec.x + ui.btnMapNew.rec.width + 10;

	ui.btnMapSave.text		= "Save";
	//ui.btnMapSave.text	= TextFormat("Save (%s)", getKeyNameFromKey(config.keyBindings["MapSave"]).c_str());
	ui.btnMapSave.rec	= {280, 10,  80, btnHeight};
	ui.btnMapSave.rec.width	= MeasureText(ui.btnMapSave.text.c_str(), ui.btnFontSize) + btnTextMargin*2;
	ui.btnMapSave.rec.x = ui.btnMapOpen.rec.x + ui.btnMapOpen.rec.width + 10;

	ui.btnMapSaveAs.text		= "Save as";
	//ui.btnMapSaveAs.text	= TextFormat("Save as (%s)", getKeyNameFromKey(config.keyBindings["MapSaveAs"]).c_str());
	ui.btnMapSaveAs.rec	= {370, 10, 120, btnHeight};
	ui.btnMapSaveAs.rec.width	= MeasureText(ui.btnMapSaveAs.text.c_str(), ui.btnFontSize) + btnTextMargin*2;
	ui.btnMapSaveAs.rec.x = ui.btnMapSave.rec.x + ui.btnMapSave.rec.width + 10;

	ui.btnAddRoad.text	= TextFormat("Add Road (%s)", getKeyNameFromKey(config.keyBindings["CreateRoad"]).c_str());
	ui.btnAddRoad.rec	= {600, 10, 120, btnHeight};
	ui.btnAddRoad.rec.width	= MeasureText(ui.btnAddRoad.text.c_str(), ui.btnFontSize) + btnTextMargin*2;
	ui.btnAddRoad.rec.x = ui.btnMapSaveAs.rec.x + ui.btnMapSaveAs.rec.width + 100;

	ui.panelRoadRec = { 650, 60, 220, 130 };
	ui.reverseRoad = true;	// État du bouton à cocher
	ui.speedLimit = 80;
	ui.lanesCount = 3;

}

