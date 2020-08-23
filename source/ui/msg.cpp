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

#include "common.hpp"
#include "screenCommon.hpp"

/* If button Position pressed -> Do something. */
static bool touching(touchPosition touch, Button button) {
	if (touch.px >= button.X && touch.px <= (button.X + button.XSize) && touch.py >= button.Y && touch.py <= (button.Y + button.YSize)) return true;
	else return false;
}

const std::vector<Button> promptBtn = {
	{0, 85, 149, 52, "YES"}, // Yes.
	{162, 85, 149, 52, "NO"} // No.
};

/* Display a Message, which needs to be confirmed with A/B. */
bool Msg::promptMsg(std::string promptMsg) {
	s32 selection = 1;

	while(1) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		UI::DrawBase(true, true);
		UI::DrawBox(80, 1);

		Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, Lang::get(promptMsg), fnt)) / 2, 0.8f, C2D_Color32(255, 255, 255, 255), Lang::get(promptMsg), 390, 70, fnt);

		UI::DrawBase(false, true);
		/* Draw Bottom Screen part. */
		for (int i = 0; i < 2; i++) {
			UI::DrawButton(promptBtn[i], 0.65);
			if (i == selection)	UI::DrawSprite(sprites_pointer_idx, promptBtn[i].X + 100, promptBtn[i].Y + 30);
		}

		C3D_FrameEnd(0);

		/* Selection part. */
		gspWaitForVBlank();
		touchPosition touch;
		hidScanInput();
		hidTouchRead(&touch);

		if (hidKeysDown() & KEY_LEFT) selection = 0;
		else if(hidKeysDown() & KEY_RIGHT) selection = 1;

		if (hidKeysDown() & KEY_A) {
			if (selection == 0) return true;
			else return false;
		}

		if (hidKeysDown() & KEY_TOUCH && touching(touch, promptBtn[0])) return true;

		if (hidKeysDown() & KEY_TOUCH && touching(touch, promptBtn[1])) return false;
	}
}

/* Displays a Warn Message. */
void Msg::DisplayWarnMsg(std::string Text) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	UI::DrawBase(true, true);

	UI::DrawBox(80, 1);

	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, Text))/2, 0.8f, C2D_Color32(255, 255, 255, 255), Text, 395, 70, fnt);
	UI::DrawBase(false, true);
	C3D_FrameEnd(0);

	for (int i = 0; i < 60 * 2; i++) {
		gspWaitForVBlank();
	}
}

/* Display a Message, which can be skipped with A. */
void Msg::DisplayWaitMsg(std::string waitMsg, ...) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	UI::DrawBox(80, 1);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, waitMsg))/2, 0.8f, C2D_Color32(255, 255, 255, 255), waitMsg, 390, 70, fnt);
	Gui::DrawStringCentered(0, 217, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("A_CONTINUE"), 395, 0, fnt);

	UI::DrawBase(false, true);
	C3D_FrameEnd(0);

	while(1) {
		hidScanInput();
		if ((hidKeysDown() & KEY_A)) break;
	}
}

/* Helperbox for instructions. */
void Msg::HelperBox(std::string Msg) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	int textBoxHeight = Gui::GetStringHeight(0.7f, Msg, fnt) + 5;

	UI::DrawBox(textBoxHeight, 1);
	Gui::DrawStringCentered(0, textBoxHeight + 10, 0.7, C2D_Color32(255, 255, 255, 255), Msg, 305, Gui::GetStringHeight(0.7f, Msg, fnt), fnt);
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	C3D_FrameEnd(0);
}