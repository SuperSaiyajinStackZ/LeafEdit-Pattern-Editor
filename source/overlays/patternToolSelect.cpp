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

extern bool exiting;
static const std::vector<Button> buttons = {
	{15, 34, 130, 48, "IMPORT_PATTERN"},
	{15, 97, 130, 48, "CHANGE_PALETTE"},
	{15, 159, 130, 48, "CREDITS"},
	{175, 34, 130, 48, "EXPORT_PATTERN"},
	{175, 97, 130, 48, "CLEAR_PATTERN"},
	{175, 159, 130, 48, "EXIT"},

	{15, 34, 130, 48, "LANGUAGE"},
	{175, 34, 130, 48, "SET_DEFAULT"},
	{15, 97, 130, 48, "SET_DEFAULT_PATTERN"},
	{175, 97, 130, 48, "EXPORT_INFORMATION"},
	{15, 159, 130, 48, "SHARE"}
};

/* If button Position pressed -> Do something. */
bool touching(touchPosition touch, Button button) {
	if (touch.px >= button.X && touch.px <= (button.X + button.XSize) && touch.py >= button.Y && touch.py <= (button.Y + button.YSize)) return true;
	else return false;
}

static void Draw(int select, int page, C2D_Image &Img) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("PATTERN_TOOL_MENU"), 395, 0, fnt);
	if (Img.subtex != nullptr) C2D_DrawImageAt(Img, 125, 42, 0.5f, nullptr, 5, 5); // 160x160. 160/32 -> 5.
	UI::DrawBase(false, true);

	/* TODO: Buttons. */
	if (page == 0) {
		for (int i = 0; i < 6; i++) {
			UI::DrawButton(buttons[i], 0.55);
		}

		UI::DrawSprite(sprites_pointer_idx, buttons[select].X + 100, buttons[select].Y + 30);
	} else {
		for (int i = 6; i < 11; i++) {
			UI::DrawButton(buttons[i], 0.55);
		}

		UI::DrawSprite(sprites_pointer_idx, buttons[6 + select].X + 100, buttons[6 + select].Y + 30);
	}

	UI::DrawSprite(sprites_top_bar_idx, 0, 0);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("PAGE") + std::to_string(page + 1) + " | 2", 310, 0, fnt);

	C3D_FrameEnd(0);
}

PatternMode Overlays::ToolSelect(C2D_Image &Img) {
	int selection = 0, page = 0;
	while(1) {
		Draw(selection, page, Img);
		u32 hDown = hidKeysDown();
		touchPosition touch;
		hidScanInput();
		hidTouchRead(&touch);

		if (hDown & KEY_R) {
			if (page == 0) {
				selection = 0;
				page = 1;
			}
		}

		if (hDown & KEY_L) {
			if (page == 1) {
				selection = 0;
				page = 0;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (page == 0) {
				if (touching(touch, buttons[0])) {
					return PatternMode::Import;
				} else if (touching(touch, buttons[1])) {
					return PatternMode::Palette;
				} else if (touching(touch, buttons[2])) {
					return PatternMode::Credits;
				} else if (touching(touch, buttons[3])) {
					return PatternMode::Export;
				} else if (touching(touch, buttons[4])) {
					return PatternMode::Clear;
				} else if (touching(touch, buttons[5])) {
					return PatternMode::Exit;
				}
			} else {
				if (touching(touch, buttons[6])) {
					return PatternMode::LangSet;
				} else if (touching(touch, buttons[7])) {
					return PatternMode::SetDefault;
				} else if (touching(touch, buttons[8])) {
					return PatternMode::DefaultPattern;
				} else if (touching(touch, buttons[9])) {
					return PatternMode::ExportInformation;
				} else if (touching(touch, buttons[10])) {
					return PatternMode::Share;
				}
			}
		}

		if (page == 0) {
			if (hDown & KEY_UP) {
				if (selection > 0) selection--;
			}

			if (hDown & KEY_DOWN) {
				if (selection < 5) selection++;
			}

			if (hDown & KEY_RIGHT) {
				if (selection < 3) {
					selection += 3;
				}
			}

			if (hDown & KEY_LEFT) {
				if (selection < 6 && selection > 2) {
					selection -= 3;
				}
			}


		} else {
			if (hDown & KEY_RIGHT) {
				if (selection == 0) selection = 1;
				else if (selection == 2) selection = 3;
			}

			if (hDown & KEY_LEFT) {
				if (selection == 1) selection = 0;
				else if (selection == 3) selection = 2;
			}

			if (hDown & KEY_UP) {
				if (selection == 2) selection = 0;
				else if (selection == 3) selection = 1;
				else if (selection == 4) selection = 2;
			}

			if (hDown & KEY_DOWN) {
				if (selection == 0) selection = 2;
				else if (selection == 1) selection = 3;
				else if (selection == 2) selection = 4;
			}
		}

		if (hDown & KEY_A) {
			if (page == 0) {
				switch(selection) {
					case 0:
						return PatternMode::Import;
					case 1:
						return PatternMode::Palette;
					case 2:
						return PatternMode::Credits;
					case 3:
						return PatternMode::Export;
					case 4:
						return PatternMode::Clear;
					case 5:
						return PatternMode::Exit;
				}
			} else {
				switch(selection) {
					case 0:
						return PatternMode::LangSet;
					case 1:
						return PatternMode::SetDefault;
					case 2:
						return PatternMode::DefaultPattern;
					case 3:
						return PatternMode::ExportInformation;
					case 4:
						return PatternMode::Share;
				}
			}
		}

		if (hDown & KEY_B) {
			return PatternMode::Draw;
		}
	}
}