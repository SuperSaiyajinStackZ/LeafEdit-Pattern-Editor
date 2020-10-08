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

extern bool touching(touchPosition touch, Structs::ButtonPos);
static const std::vector<Structs::ButtonPos> langBlocks = {
	{37, 32, 20, 20},
	{37, 72, 20, 20}
};

static void Draw(int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, -2, 0.9, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_LANGUAGE"), 390, 0, fnt);
	UI::DrawBase(false, true);

	for (int language = 0; language < 2; language++) {
		Gui::Draw_Rect(langBlocks[language].x, langBlocks[language].y, langBlocks[language].w, langBlocks[language].h, C2D_Color32(120, 120, 120, 255));
	}

	UI::DrawSprite(sprites_pointer_idx, langBlocks[selection].x + 3, langBlocks[selection].y + 2);

	Gui::DrawString(langBlocks[0].x + 25, langBlocks[0].y, 0.7f, C2D_Color32(0, 0, 0, 255), "Deutsch", 320, 0, fnt);
	Gui::DrawString(langBlocks[1].x + 25, langBlocks[1].y, 0.7f, C2D_Color32(0, 0, 0, 255), "English", 320, 0, fnt);

	C3D_FrameEnd(0);
}

void Overlays::SelectLang() {
	int selection = Settings::getLang();
	bool doOut = false;

	while(!doOut) {
		touchPosition touch;
		Draw(selection);
		hidScanInput();
		hidTouchRead(&touch);

		if (hidKeysDown() & KEY_DOWN) {
			if (selection == 0) selection = 1;
		}

		if (hidKeysDown() & KEY_UP) {
			if (selection == 1) selection = 0;
		}

		if (hidKeysDown() & KEY_A) {
			Settings::setLang(selection);
			Lang::load(Settings::getLang());
			doOut = true;
		}

		if (hidKeysDown() & KEY_TOUCH) {
			for (int i = 0; i < 2; i++) {
				if (touching(touch, langBlocks[i])) {
					Settings::setLang(i);
					Lang::load(Settings::getLang());
					doOut = true;
				}
			}
		}

		if (hidKeysDown() & KEY_B) {
			doOut = true;
		}
	}
}