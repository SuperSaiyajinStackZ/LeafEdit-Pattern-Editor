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

#ifndef _LEAFEDIT_PATTERN_EDITOR_UI_HPP
#define _LEAFEDIT_PATTERN_EDITOR_UI_HPP

#include "common.hpp"
#include "sprites.h"

struct Button {
	int X;
	int Y;
	int XSize;
	int YSize;
	std::string Text;
};

namespace UI {
	void DrawSprite(int index, int x, int y, float ScaleX = 1, float ScaleY = 1);
	void DrawBase(bool onTop, bool bar);
	void DrawFileBrowseBG(bool isTop);
	void DrawSelector(bool top, int y);
	void DrawBtn(int x, int y, int xLength, int yLength);
	void DrawButton(Button btn, float TextSize = 0.5f);
	void DrawPaletteGrid(float xPos, float yPos, float Width, float Height, u32 paletteColor, u32 gridColor);
}

#endif