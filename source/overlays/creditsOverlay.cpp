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

static void Draw(void) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, 0, 0.9f, C2D_Color32(255, 255, 255, 255), "Credits", 395, 0, fnt);

	Gui::DrawStringCentered(0, 28, 0.8f, C2D_Color32(255, 255, 255, 255), "SuperSaiyajinStackZ", 395, 0, fnt);
	Gui::DrawStringCentered(0, 48, 0.7f, C2D_Color32(255, 255, 255, 255), "- For creating this app.", 395, 0, fnt);
	Gui::DrawStringCentered(0, 78, 0.8f, C2D_Color32(255, 255, 255, 255), "All LeafEdit-Core contributors", 395, 0, fnt);
	Gui::DrawStringCentered(0, 98, 0.7f, C2D_Color32(255, 255, 255, 255), "- For helping on LeafEdit-Core.", 395, 0, fnt);
	Gui::DrawStringCentered(0, 128, 0.8f, C2D_Color32(255, 255, 255, 255), "devkitPro", 395, 0, fnt);
	Gui::DrawStringCentered(0, 148, 0.7f, C2D_Color32(255, 255, 255, 255), "- For Libctru, Citro2D & Citro3D.", 395, 0, fnt);
	Gui::DrawStringCentered(0, 178, 0.8f, C2D_Color32(255, 255, 255, 255), "Universal-Team", 395, 0, fnt);
	Gui::DrawStringCentered(0, 198, 0.7f, C2D_Color32(255, 255, 255, 255), "- For LeafEdit's Pattern core and Universal-Core.", 395, 0, fnt);
	Gui::DrawString(395-Gui::GetStringWidth(0.8, V_STRING, fnt), 219, 0.8, C2D_Color32(255, 255, 255, 255), V_STRING, 400, 0, fnt);

	UI::DrawBase(false, true);
	C3D_FrameEnd(0);
}

void Overlays::CreditsOverlay() {
	bool doOut = false;

	while(!doOut) {
		Draw();
		hidScanInput();

		if (hidKeysDown()) doOut = true;
	}
}