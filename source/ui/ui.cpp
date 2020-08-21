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

void UI::DrawBox(int y, u8 rows) {
	// Draw Top & Bottom.
	UI::DrawSprite(sprites_box_top_idx, 0, y);
	UI::DrawSprite(sprites_box_bot_idx, 0, y + 24 + (40 * rows));

	C2D_Image sprite = C2D_SpriteSheetGetImage(sprites, sprites_box_middle_idx);
	for (u8 row = 0; row < rows; row++) {
		C2D_DrawImageAt(sprite, 0, y + 24 + ( row * sprite.subtex->height), 0.5f);
	}
}

/* Code from PKSM. https://github.com/FlagBrew/PKSM/blob/master/3ds/source/gui/gui.cpp#L73. */
Tex3DS_SubTexture _select_box(const C2D_Image& image, int x, int y, int endX, int endY) {
	Tex3DS_SubTexture tex = *image.subtex;
	if (x != endX) {
		int deltaX	= endX - x;
		float texRL	= tex.left - tex.right;
		tex.left	= tex.left - (float)texRL / tex.width * x;
		tex.right	= tex.left - (float)texRL / tex.width * deltaX;
		tex.width	= deltaX;
	}

	if (y != endY) {
		float texTB	= tex.top - tex.bottom;
		int deltaY	= endY - y;
		tex.top		= tex.top - (float)texTB / tex.height * y;
		tex.bottom	= tex.top - (float)texTB / tex.height * deltaY;
		tex.height	= deltaY;
	}

	return tex;
}

void UI::DrawSelector(bool top, int y) {
	C2D_Image sprite = C2D_SpriteSheetGetImage(sprites, sprites_selector_side_idx);
	Tex3DS_SubTexture tex = _select_box(sprite, 8, 0, 9, 21); // Get Height.

	if (top) {
		/* Draw Sides. */
		DrawSprite(sprites_selector_side_idx, 0, y);
		DrawSprite(sprites_selector_side_idx, 391, y, -1.0, 1.0);
		/* Stretch the middle. */
		C2D_DrawImageAt({sprite.tex, &tex}, 9, y, 0.5f, nullptr, 382.0, 1);
	} else {
		/* Draw Sides. */
		DrawSprite(sprites_selector_side_idx, 0, y);
		DrawSprite(sprites_selector_side_idx, 311, y, -1.0, 1.0);
		/* Stretch the middle. */
		C2D_DrawImageAt({sprite.tex, &tex}, 9, y, 0.5f, nullptr, 302.0, 1);
	}
}

void UI::DrawBtn(int x, int y, int xLength, int yLength) {
	C2D_Image sprite = C2D_SpriteSheetGetImage(sprites, sprites_button_corner_idx);
	Tex3DS_SubTexture tex;
	
	/* Corners. */
	DrawSprite(sprites_button_corner_idx, x, y, 1, 1);
	DrawSprite(sprites_button_corner_idx, x + xLength - 14, y, -1.0, 1.0);
	DrawSprite(sprites_button_corner_idx, x, y + yLength - 14, 1.0, -1.0);
	DrawSprite(sprites_button_corner_idx, x + xLength - 14, y + yLength - 14, -1.0, -1.0);

	/* Height draw. */
	tex = _select_box(sprite, 0, 11, 14, 12); // Get Height.

	C2D_DrawImageAt({sprite.tex, &tex}, x, y + 14, 0.5f, nullptr, 1, yLength - 28);
	C2D_DrawImageAt({sprite.tex, &tex}, x + xLength - 14, y + 14, 0.5f, nullptr, -1, yLength - 28);

	/* Width draw. */
	tex = _select_box(sprite, 11, 0, 12, 14); // Get Width.

	C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y, 0.5f, nullptr, xLength - 28, 1);
	C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y + yLength - 14, 0.5f, nullptr, xLength - 28, -1);	

	/* And now the middle. */
	tex = _select_box(sprite, 11, 11, 12, 12); // Get Corner pixel.

	C2D_DrawImageAt({sprite.tex, &tex}, x + 14, y + 14, 0.5f, nullptr, xLength - 28, yLength - 28);
}

void UI::DrawBase(bool onTop, bool bar) {
	onTop ? Gui::ScreenDraw(Top) : Gui::ScreenDraw(Bottom);

	if (onTop) {
		if (bar) {
			/* Draw Tiled BG. */
			DrawSprite(sprites_tileBG_idx, 0, 0);
			DrawSprite(sprites_tileBG_idx, 135, 0);
			DrawSprite(sprites_tileBG_idx, 270, 0);
			DrawSprite(sprites_tileBG_idx, 0, 132);
			DrawSprite(sprites_tileBG_idx, 135, 132);
			DrawSprite(sprites_tileBG_idx, 270, 132);
			/* Draw grass bar on bottom. */
			DrawSprite(sprites_bar_idx, 0, 215);
			/* Draw Text bar, so Text is readable. */
			DrawSprite(sprites_top_bar_idx, 0, 0);
		} else {
			/* Just solid. */
			Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(215, 178, 111, 255)); // Draw BG based on Tile's color.
		}
	} else {
		DrawSprite(sprites_tileBG_idx, 0, 0);
		DrawSprite(sprites_tileBG_idx, 135, 0);
		DrawSprite(sprites_tileBG_idx, 270, 0);
		DrawSprite(sprites_tileBG_idx, 0, 132);
		DrawSprite(sprites_tileBG_idx, 135, 132);
		DrawSprite(sprites_tileBG_idx, 270, 132);
		if (bar) {
			DrawSprite(sprites_bar_idx, 0, 0, 1, -1);
			DrawSprite(sprites_bar_idx, 0, 215);
		}
	}
}

void UI::DrawFileBrowseBG(bool isTop) {
	if (isTop) {
		Gui::ScreenDraw(Top);
		/* Draw Bakground. */
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
		/* Draw FileBrowse Bars. */
		Gui::Draw_Rect(0, 45, 400, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		Gui::Draw_Rect(0, 87, 400, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		Gui::Draw_Rect(0, 129, 400, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		Gui::Draw_Rect(0, 171, 400, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		/* Draw Textbox bars. */
		DrawSprite(sprites_top_bar_idx, 0, 0);
		DrawSprite(sprites_bottom_bar_idx, 0, 208);
	} else {
		Gui::ScreenDraw(Bottom);
		/* Draw Background. */
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0xD5, 0xB0, 0x6E, 255));
		/* Draw FileBrowse Bars. */
		Gui::Draw_Rect(0, 24, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		Gui::Draw_Rect(0, 67, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		Gui::Draw_Rect(0, 109, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		Gui::Draw_Rect(0, 152, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		Gui::Draw_Rect(0, 194, 320, 21, C2D_Color32(0xDF, 0xBB, 0x78, 255));
		/* Draw grass Bars. */
		DrawSprite(sprites_bar_idx, 0, 0, 1, -1);
		DrawSprite(sprites_bar_idx, 0, 215);
	}
}

void UI::DrawButton(Button btn, float TextSize) {
	DrawBtn(btn.X, btn.Y, btn.XSize, btn.YSize);
	Gui::DrawStringCentered(btn.X - 160 + (btn.XSize / 2), btn.Y + (btn.YSize / 2) - 10, TextSize, C2D_Color32(0, 0, 0, 255), Lang::get(btn.Text), btn.XSize - 15, btn.YSize - 10);
}

/* Special Grid for the Palettes. Universal-Core ones won't work well. */
void UI::DrawPaletteGrid(float xPos, float yPos, float Width, float Height, u32 paletteColor, u32 gridColor) {
	static constexpr int w	= 1;
	C2D_DrawRectSolid(xPos, yPos, 0.5, Width, Height, paletteColor);

	/* Grid part. */
	C2D_DrawRectSolid(xPos, yPos, 0.5, Width, w, gridColor); // top.
	C2D_DrawRectSolid(xPos, yPos + w, 0.5, w, Height - 2 * w, gridColor); // left.
	C2D_DrawRectSolid(xPos + Width - w, yPos + w, 0.5, w, Height - 2 * w, gridColor); // right.
	C2D_DrawRectSolid(xPos, yPos + Height - w, 0.5, Width, w, gridColor); // bottom.
}