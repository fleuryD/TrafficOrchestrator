
#include "../include/traffic-orchestrator.hpp"


void	drawRectWithBorder(Rectangle rec, Color fillColor, Color borderColor) {
	DrawRectangle(rec.x, rec.y, rec.width, rec.height, fillColor);
	DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, borderColor);

}


void	drawCross(int x, int y, int size, Color color) {
	DrawLine(x - size, y, x + size, y, color);	// Ligne horizontale
	DrawLine(x, y - size, x, y + size, color);	// Ligne verticale
}


/*
void	drawBtnCenter(Rectangle rec, const std::string &text, bool isOn){
	bool	isHovered = recIsHovered(rec);
	Color	buttonColor = isHovered ? (isOn ? GREEN : GRAY) : (isOn ? DARKGREEN : DARKGRAY);

	drawRectWithBorder(rec, buttonColor, WHITE);

	// Affiche le texte centré
	int textWidth = MeasureText(text.c_str(), 16);
	DrawText(text.c_str(), rec.x + (rec.width - textWidth) / 2, rec.y + (rec.height - 20) / 2 + 2, 16, WHITE);
}
*/


void	drawBtn(Rectangle rec, const std::string &text, int fontSize, bool isOn){
	bool	isHovered = recIsHovered(rec);
	Color	buttonColor = isHovered ? (isOn ? GREEN : GRAY) : (isOn ? DARKGREEN : DARKGRAY);

	drawRectWithBorder(rec, buttonColor, WHITE);
	DrawText(text.c_str(), rec.x+4 , rec.y+4, fontSize, WHITE);
}


bool	recIsHovered(Rectangle rec) {
	Vector2 mouse = GetMousePosition();
	return (mouse.x >= rec.x && mouse.x <= rec.x + rec.width &&
			mouse.y >= rec.y && mouse.y <= rec.y + rec.height);
}



