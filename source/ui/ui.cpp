/*
*   This file is part of LeafEdit-Pattern-Editor
*   Copyright (C) 2020 SuperSaiyajinStackZ
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "ui.hpp"

extern C2D_SpriteSheet sprites;

void UI::DrawSprite(int index, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, index, x, y, ScaleX, ScaleY);
}

void UI::DrawBase(bool onTop, bool bar) {
	onTop ? Gui::ScreenDraw(Top) : Gui::ScreenDraw(Bottom);

	if (bar) {
		Gui::Draw_Rect(0, 0, onTop ? 400 : 320, 25, C2D_Color32(0, 0, 128, 255));
		Gui::Draw_Rect(0, 25, onTop ? 400 : 320, 190, C2D_Color32(0, 0, 170, 255));
		Gui::Draw_Rect(0, 215, onTop ? 400 : 320, 25, C2D_Color32(0, 0, 128, 255));
	} else {
		Gui::Draw_Rect(0, 0, onTop ? 400 : 320, 240, C2D_Color32(0, 0, 180, 255));
	}
}

void UI::DrawButton(Button btn, float TextSize) {
	Gui::Draw_Rect(btn.X, btn.Y, btn.XSize, btn.YSize, C2D_Color32(0, 0, 230, 255));
	Gui::DrawStringCentered(btn.X - 160 + (btn.XSize / 2), btn.Y + (btn.YSize / 2) - 10, TextSize, C2D_Color32(255, 255, 255, 255), btn.Text, btn.XSize - 10, btn.YSize - 5);
}

/* Special Grid for the Palettes. Universal-Core ones won't work well. */
void UI::DrawPaletteGrid(float xPos, float yPos, float Width, float Height, u32 paletteColor, u32 gridColor) {
	static constexpr int w	= 1;
	C2D_DrawRectSolid(xPos, yPos, 0.5, Width, Height, paletteColor);

	/* Grid part. */
	C2D_DrawRectSolid(xPos, yPos, 0.5, Width, w, gridColor); // top
	C2D_DrawRectSolid(xPos, yPos + w, 0.5, w, Height - 2 * w, gridColor); // left
	C2D_DrawRectSolid(xPos + Width - w, yPos + w, 0.5, w, Height - 2 * w, gridColor); // right
	C2D_DrawRectSolid(xPos, yPos + Height - w, 0.5, Width, w, gridColor); // bottom
}