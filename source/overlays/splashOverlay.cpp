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
#include <ctime>

#define _SPLASH_X_LOGO_POS 140
#define _SPLASH_LOGO_INIT_DELAY 50
#define _SPLASH_WAIT_DELAY 200

static void Draw(const int &logoPos, const uint32_t &year) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, Lang::get("STACKZ_PRESENTS"), 0, 0, fnt);
	UI::DrawSprite(sprites_stackz_idx, 2, 74);
	if (logoPos < 400) UI::DrawSprite(sprites_banner_idx, logoPos, 56);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, "2020 - " + std::to_string(year), 0, 0, fnt);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	UI::DrawBase(false, false);
	UI::DrawSprite(sprites_universal_core_idx, 0, 26);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	C3D_FrameEnd(0);
}

void Overlays::SplashOverlay() {
	int delay = _SPLASH_WAIT_DELAY, logoPos = 402, swipeDelay = _SPLASH_LOGO_INIT_DELAY;
	bool doOut = false, swipedIn = false, doSwipe = false;

	/* Get current time / year. */
	time_t currentTime = time(NULL);
	struct tm *currentTimeStruct = localtime(&currentTime);
	uint32_t year = 1900 + currentTimeStruct->tm_year;

	while(!doOut) {
		Draw(logoPos, year);

		hidScanInput();

		if (hidKeysDown()) doOut = true;

		if (!swipedIn) {
			if (swipeDelay > 0) {
				swipeDelay--;

				if (swipeDelay == 0) {
					doSwipe = true;
				}
			}
		}

		if (doSwipe) {
			if (logoPos > _SPLASH_X_LOGO_POS) {
				logoPos--;

				if (logoPos == _SPLASH_X_LOGO_POS) {
					swipedIn = true;
				}
			}
		}

		if (swipedIn) {
			if (delay > 0) {
				delay--;

				if (delay == 0) doOut = true;
			}
		}
	}
}