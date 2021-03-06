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
	{15, 97, 130, 48, "SET_DEFAULT_PATTERN"},
	{15, 159, 130, 48, "SHARE"},
	{175, 34, 130, 48, "SET_DEFAULT"},
	{175, 97, 130, 48, "EXPORT_INFORMATION"},
	{175, 159, 130, 48, "LOAD_SAVE"},

	{15, 34, 130, 48, "LOAD_FROM_SAVE"},
	{175, 34, 130, 48, "SAVE_SAVE"},
	{15, 97, 130, 48, "UNLOAD_SAVE"},
	{175, 97, 130, 48, "STORAGE"}
};

static const std::vector<Button> buttonsSave = {
	{15, 34, 130, 48, "INJECT_PATTERN"},
	{15, 97, 130, 48, "CHANGE_PALETTE"},
	{15, 159, 130, 48, "CREDITS"},
	{175, 34, 130, 48, "EXPORT_PATTERN"},
	{175, 97, 130, 48, "CLEAR_PATTERN"},
	{175, 159, 130, 48, "EXIT"},

	{15, 34, 130, 48, "LANGUAGE"},
	{15, 97, 130, 48, "SET_DEFAULT_PATTERN"},
	{15, 159, 130, 48, "SHARE"},
	{175, 34, 130, 48, "SET_DEFAULT"},
	{175, 97, 130, 48, "EXPORT_INFORMATION"},
	{175, 159, 130, 48, "LOAD_SAVE"},

	{15, 34, 130, 48, "LOAD_FROM_SAVE"},
	{175, 34, 130, 48, "SAVE_SAVE"},
	{15, 97, 130, 48, "UNLOAD_SAVE"},
	{175, 97, 130, 48, "STORAGE"}
};

/* If button Position pressed -> Do something. */
bool touching(touchPosition touch, Button button) {
	if (touch.px >= button.X && touch.px <= (button.X + button.XSize) && touch.py >= button.Y && touch.py <= (button.Y + button.YSize)) return true;
	else return false;
}

static void Draw(int select, int page, C2D_Image &Img, bool isSave) {
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
			UI::DrawButton(isSave ? buttonsSave[i] : buttons[i], 0.5);
		}

		UI::DrawSprite(sprites_pointer_idx, buttons[select].X + 100, buttons[select].Y + 30);
	} else if (page == 1) {
		for (int i = 6; i < 12; i++) {
			UI::DrawButton(isSave ? buttonsSave[i] : buttons[i], 0.5);
		}

		UI::DrawSprite(sprites_pointer_idx, buttons[6 + select].X + 100, buttons[6 + select].Y + 30);
	} else {
		for (int i = 12; i < 16; i++) {
			UI::DrawButton(isSave ? buttonsSave[i] : buttons[i], 0.5);
		}

		UI::DrawSprite(sprites_pointer_idx, buttons[12 + select].X + 100, buttons[12 + select].Y + 30);
	}

	UI::DrawSprite(sprites_top_bar_idx, 0, 0);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("PAGE") + std::to_string(page + 1) + " | 3", 310, 0, fnt);

	C3D_FrameEnd(0);
}

PatternMode Overlays::ToolSelect(C2D_Image &Img, bool isSave) {
	int selection = 0, page = 0;
	while(1) {
		Draw(selection, page, Img, isSave);
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();
		touchPosition touch;
		hidScanInput();
		hidTouchRead(&touch);

		if (hRepeat & KEY_R) {
			if (page < 2) {
				selection = 0;
				page++;
			}
		}

		if (hRepeat & KEY_L) {
			if (page > 0) {
				selection = 0;
				page--;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (page == 0) {
				if (touching(touch, buttons[0])) {
					return isSave ? PatternMode::Inject : PatternMode::Import;
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

			} else if (page == 1) {
				if (touching(touch, buttons[6])) {
					return PatternMode::LangSet;
				} else if (touching(touch, buttons[7])) {
					return PatternMode::DefaultPattern;
				} else if (touching(touch, buttons[8])) {
					return PatternMode::Share;
				} else if (touching(touch, buttons[9])) {
					return PatternMode::SetDefault;
				} else if (touching(touch, buttons[10])) {
					return PatternMode::ExportInformation;
				} else if (touching(touch, buttons[11])) {
					return PatternMode::LoadSave;
				}

			} else {
				if (touching(touch, buttons[12])) {
					return PatternMode::LoadFromSave;
				} else if (touching(touch, buttons[13])) {
					return PatternMode::DoSave;
				} else if (touching(touch, buttons[14])) {
					return PatternMode::UnloadSave;
				} else if (touching(touch, buttons[15])) {
					return PatternMode::StorageHandling;
				}
			}
		}

		if (page == 0 || page == 1) {
			if (hRepeat & KEY_UP) {
				if (selection > 0) selection--;
			}

			if (hRepeat & KEY_DOWN) {
				if (selection < 5) selection++;
			}

			if (hRepeat & KEY_RIGHT) {
				if (selection < 3) {
					selection += 3;
				}
			}

			if (hRepeat & KEY_LEFT) {
				if (selection < 6 && selection > 2) {
					selection -= 3;
				}
			}


		} else {
			if (hRepeat & KEY_RIGHT) {
				if (selection == 0) selection = 1;
				else if (selection == 2) selection = 3;
			}

			if (hRepeat & KEY_LEFT) {
				if (selection == 1) selection = 0;
				else if (selection == 3) selection = 2;
			}

			if (hRepeat & KEY_DOWN) {
				if (selection == 0) selection = 2;
				if (selection == 1) selection = 3;
			}

			if (hRepeat & KEY_UP) {
				if (selection == 2) selection = 0;
				if (selection == 3) selection = 1;
			}
		}

		if (hDown & KEY_A) {
			if (page == 0) {
				switch(selection) {
					case 0:
						return isSave ? PatternMode::Inject : PatternMode::Import;
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
					case 6:
						return PatternMode::LoadSave;
				}
				
			} else if (page == 1) {
				switch(selection) {
					case 0:
						return PatternMode::LangSet;
					case 1:
						return PatternMode::DefaultPattern;
					case 2:
						return PatternMode::Share;
					case 3:
						return PatternMode::SetDefault;
					case 4:
						return PatternMode::ExportInformation;
					case 5:
						return PatternMode::LoadSave;
				}

			} else {
				switch(selection) {
					case 0:
						return PatternMode::LoadFromSave;
					case 1:
						return PatternMode::DoSave;
					case 2:
						return PatternMode::UnloadSave;
					case 3:
						return PatternMode::StorageHandling;
				}
			}
		}

		if (hDown & KEY_B) {
			return PatternMode::Draw;
		}
	}
}