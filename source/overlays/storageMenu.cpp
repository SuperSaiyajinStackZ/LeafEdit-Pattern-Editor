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

#include "kbd.hpp"
#include "overlay.hpp"

extern bool exiting;
static const std::vector<Button> buttons = {
	{15, 34, 130, 48, "RESIZE_STORAGE"},
	{175, 34, 130, 48, "CHANGE_STORAGE"},
	{15, 97, 130, 48, "CREATE_STORAGE"},
	{175, 97, 130, 48, "MANAGE_STORAGE"}, 
	{15, 159, 130, 48, "SAVE_STORAGE"}
};

/* If button Position pressed -> Do something. */
static bool touching(touchPosition touch, Button button) {
	if (touch.px >= button.X && touch.px <= (button.X + button.XSize) && touch.py >= button.Y && touch.py <= (button.Y + button.YSize)) return true;
	else return false;
}

static void Draw(std::unique_ptr<Storage> &storage, int select) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("STORAGE_MENU"), 395, 0, fnt);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	UI::DrawSprite(sprites_storage_idx, 72, 56);

	if (storage) {
		Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("STORAGE_NAME") + storage->name(), 395, 0, fnt);
	} else {
		Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("STORAGE_NOT_LOADED"), 395, 0, fnt);
	}

	UI::DrawBase(false, true);

	for (int i = 0; i < 5; i++) {
		UI::DrawButton(buttons[i], 0.5);
	}

	UI::DrawSprite(sprites_pointer_idx, buttons[select].X + 100, buttons[select].Y + 30);
	UI::DrawSprite(sprites_top_bar_idx, 0, 0);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("X_SELECT"), 310, 0, fnt);

	C3D_FrameEnd(0);
}

/* Handle Storage resize. */
static void handleResize(std::unique_ptr<Storage> &storage) {
	int boxAmount = storage->boxes();

	int temp = KBD::setInt(50, Lang::get("ENTER_BOX_AMOUNT"));
	if (temp > 0 && temp < 51) {
		if (temp < boxAmount) return; // Do not allow to resize it smaller, since it would break it.
		boxAmount = temp;
	}

	storage->resize(boxAmount);
}

static void changeStorage(std::unique_ptr<Storage> &storage) {
	std::string file = "";
	if (Overlays::SelectFile("sdmc:/3ds/LeafEdit/Pattern-Editor/storage/", {"storage"}, file, Lang::get("SELECT_STORAGE"), false)) {
		file.erase(file.size()-8, 8);

		std::unique_ptr<Storage> tmpStorage = std::make_unique<Storage>(file);
		if (tmpStorage) {
			storage->save();
			storage = nullptr;
			storage = std::move(tmpStorage);
		}
	}
}

void Overlays::StorageMenu(std::unique_ptr<Storage> &storage, std::unique_ptr<Sav> &savefile) {
	bool doOut = false;
	int selection = 0;
	while(!doOut) {
		Draw(storage, selection);
		touchPosition touch;
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();
		hidTouchRead(&touch);

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
			else if (selection == 1) selection = 3;
			else if (selection == 2) selection = 4;
		}

		if (hRepeat & KEY_UP) {
			if (selection == 2) selection = 0;
			else if (selection == 3) selection = 1;
			else if (selection == 4) selection = 2;
		}

		if (hDown & KEY_X) {
			switch(selection) {
				case 0:
					handleResize(storage);
					gspWaitForVBlank();
					break;
				case 1:
					changeStorage(storage);
					gspWaitForVBlank();
					break;
				case 2:
					storage->name(KBD::kbdString(20, Lang::get("ENTER_STORAGE_NAME")));
					storage->createStorage();
					storage->load();
					gspWaitForVBlank();
					break;
				case 3:
					if (storage && savefile) {
						Overlays::StorageHandling(storage, savefile);
					} else {
						Msg::DisplayWaitMsg(Lang::get("STORAGE_MSG"));
					}

					gspWaitForVBlank();

					break;
				case 4:
					if (Msg::promptMsg("SAVE_STORAGE_MSG")) storage->save();
					gspWaitForVBlank();
					break;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, buttons[0])) {
				handleResize(storage);
				gspWaitForVBlank();
			} else if (touching(touch, buttons[1])) {
				changeStorage(storage);
				gspWaitForVBlank();
			} else if (touching(touch, buttons[2])) {
				storage->name(KBD::kbdString(20, Lang::get("ENTER_STORAGE_NAME")));
				storage->createStorage();
				storage->load();
				gspWaitForVBlank();
			} else if (touching(touch, buttons[3])) {
				if (storage && savefile) {
					Overlays::StorageHandling(storage, savefile);
				} else {
					Msg::DisplayWaitMsg(Lang::get("STORAGE_MSG"));
				}

				gspWaitForVBlank();
			} else if (touching(touch, buttons[4])) {
				if (Msg::promptMsg("SAVE_STORAGE_MSG")) storage->save();
				gspWaitForVBlank();
			}	
		}

		if (hDown & KEY_B) doOut = true;
	}

}