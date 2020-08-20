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

// Displays a Warn Message.
void Msg::DisplayWarnMsg(std::string Text) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	UI::DrawBase(true, true);

	Gui::Draw_Rect(0, 80, 400, 88, C2D_Color32(0, 0, 230, 255));

	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, Text))/2, 0.8f, C2D_Color32(255, 255, 255, 255), Text, 395, 70, fnt);
	UI::DrawBase(false, true);
	C3D_FrameEnd(0);

	for (int i = 0; i < 60 * 2; i++) {
		gspWaitForVBlank();
	}
}

// Display a Message, which can be skipped with A.
void Msg::DisplayWaitMsg(std::string waitMsg, ...) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	Gui::Draw_Rect(0, 80, 400, 88, C2D_Color32(0, 0, 230, 255));
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, waitMsg))/2, 0.8f, C2D_Color32(255, 255, 255, 255), waitMsg, 390, 70, fnt);
	Gui::DrawStringCentered(0, 217, 0.9f, C2D_Color32(255, 255, 255, 255), "Press \uE000 to continue.", 395, 0, fnt);

	UI::DrawBase(false, true);
	C3D_FrameEnd(0);

	while(1) {
		hidScanInput();
		if ((hidKeysDown() & KEY_A)) break;
	}
}

void Msg::HelperBox(std::string Msg) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	int textBoxHeight = Gui::GetStringHeight(0.7f, Msg, fnt) + 5;

	Gui::Draw_Rect(40, 211 - textBoxHeight, 320, textBoxHeight, C2D_Color32(0, 0, 180, 255));
	Gui::Draw_Rect(44, 215 - textBoxHeight, 312, textBoxHeight - 8, C2D_Color32(0, 0, 230, 255));
	Gui::DrawStringCentered(0, 215 - textBoxHeight, 0.7, C2D_Color32(255, 255, 255, 255), Msg, 305, Gui::GetStringHeight(0.7f, Msg), fnt);
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	C3D_FrameEnd(0);
}