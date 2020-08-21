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

#include "overlay.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

static const std::vector<Structs::ButtonPos> types = {
	{30, 90, 48, 48},
	{130, 90, 48, 48},
	{230, 90, 48, 48}
};

static const std::vector<Structs::ButtonPos> regions = {
	{18, 120, 44, 30},
	{98, 120, 44, 30},
	{178, 120, 44, 30},
	{258, 120, 44, 30}
};

static void Draw(int mode, int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	if (mode == 0) {
		Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_SAVETYPE"), 395, 0, fnt);
		UI::DrawBase(false, true);

		
		UI::DrawSprite(sprites_iconNL_idx, types[0].x, types[0].y);
		UI::DrawSprite(sprites_iconWA_idx, types[1].x, types[1].y);
		UI::DrawSprite(sprites_iconWW_idx, types[2].x, types[2].y);

		Gui::drawGrid(types[selection].x, types[selection].y, types[selection].w, types[selection].h, C2D_Color32(180, 0, 0, 200));

		Gui::DrawString(20, 70, 0.75f, C2D_Color32(0, 0, 0, 255), "New Leaf", 0, 0, fnt);
		Gui::DrawString(105, 140, 0.75f, C2D_Color32(0, 0, 0, 255), "Welcome Amiibo", 0, 0, fnt);
		Gui::DrawString(220, 70, 0.75f, C2D_Color32(0, 0, 0, 255), "Wild World", 0, 0, fnt);
	} else {
		Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_REGION"), 395, 0, fnt);
		UI::DrawBase(false, true);
		
		UI::DrawSprite(sprites_japan_idx, regions[0].x, regions[0].y);
		UI::DrawSprite(sprites_usa_idx, regions[1].x, regions[1].y);
		UI::DrawSprite(sprites_europe_idx, regions[2].x, regions[2].y);
		UI::DrawSprite(sprites_korea_idx, regions[3].x, regions[3].y);

		Gui::drawGrid(regions[selection].x, regions[selection].y, regions[selection].w, regions[selection].h, C2D_Color32(180, 0, 0, 200));

		Gui::DrawStringCentered(-90 - 30, 95, 0.8, C2D_Color32(0, 0, 0, 255), "Japan", 0, 0, fnt);
		Gui::DrawStringCentered(-10 - 30, 95, 0.8, C2D_Color32(0, 0, 0, 255), "USA", 0, 0, fnt);
		Gui::DrawStringCentered(70 - 30, 95, 0.8, C2D_Color32(0, 0, 0, 255), "Europe", 0, 0, fnt);
		Gui::DrawStringCentered(150 - 30, 95, 0.8, C2D_Color32(0, 0, 0, 255), "Korea", 0, 0, fnt);
	}

	C3D_FrameEnd(0);
}

void Overlays::SaveSelect(SaveType &ST, WWRegion &region) {
	int mode = 0, selection = 0;
	bool doOut = false;

	gspWaitForVBlank();
	while(!doOut) {
		Draw(mode, selection);
		touchPosition touch;
		u32 hDown = hidKeysDown();
		hidScanInput();
		hidTouchRead(&touch);

		if (mode == 0) {
			/* SaveType Select. */
			if (hDown & KEY_RIGHT) {
				if (selection < 2) selection++;
			}

			if (hDown & KEY_LEFT) {
				if (selection > 0) selection--;
			}

			if (hDown & KEY_TOUCH) {
				if (touching(touch, types[0])) {
					ST = SaveType::NL;
					doOut = true;
				} else if (touching(touch, types[1])) {
					ST = SaveType::WA;
					doOut = true;
 				} else if (touching(touch, types[2])) {
					ST = SaveType::WW;
					gspWaitForVBlank();
					selection = 0;
					mode = 1;
				}
			}

			if (hDown & KEY_A) {
				switch(selection) {
					case 0:
						ST = SaveType::NL;
						doOut = true;
						break;
					case 1:
						ST = SaveType::WA;
						doOut = true;
						break;
					case 2:
						ST = SaveType::WW;
						gspWaitForVBlank();
						selection = 0;
						mode = 1;
						break;
				}
			}

		} else {
			/* Region Select. */
			if (hDown & KEY_RIGHT) {
				if (selection < 3) selection++;
			}

			if (hDown & KEY_LEFT) {
				if (selection > 0) selection--;
			}

			if (hDown & KEY_TOUCH) {
				if (touching(touch, regions[0])) {
					region = WWRegion::JPN_REV1;
					doOut = true;
				} else if (touching(touch, regions[1])) {
					region = WWRegion::USA_REV1;
					doOut = true;
 				} else if (touching(touch, regions[2])) {
					region = WWRegion::EUR_REV1;
					doOut = true;
				} else if (touching(touch, regions[3])) {
					region = WWRegion::KOR_REV1;
					doOut = true;
				}
			}

			if (hDown & KEY_A) {
				switch(selection) {
					case 0:
						region = WWRegion::JPN_REV1;
						doOut = true;
						break;
					case 1:
						region = WWRegion::USA_REV1;
						doOut = true;
						break;
					case 2:
						region = WWRegion::EUR_REV1;
						doOut = true;
						break;
					case 3:
						region = WWRegion::KOR_REV1;
						doOut = true;
						break;
				}
			}
		}
	}
}