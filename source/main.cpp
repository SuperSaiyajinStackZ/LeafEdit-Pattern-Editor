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

#include <3ds.h>
#include "common.hpp"
#include "overlay.hpp"
#include "patternEditor.hpp"
#include "settings.hpp"
#include "sound.hpp"
#include "structs.hpp"

#include <dirent.h>
#include <unistd.h>

bool exiting = false;

bool dspFound = false, songIsFound = false;
std::unique_ptr<sound> bgm = nullptr;
C2D_SpriteSheet sprites;
C2D_Font fnt;

bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h)) return true;
	else return false;
}

static void loadSoundEffects(void) {
	if (dspFound) {
		if (access("sdmc:/3ds/LeafEdit/Pattern-Editor/Music.wav", F_OK ) == 0) {
			bgm = std::make_unique<sound>("sdmc:/3ds/LeafEdit/Pattern-Editor/Music.wav", 1, true);
			if (bgm->getValid()) songIsFound = true;
			else songIsFound = false;
		}
	}
}

static void playMusic(void) {
	if (songIsFound && bgm) {
		if (bgm->getValid()) bgm->play();
	}
}

static void stopMusic(void) {
	if (songIsFound && bgm) {
		if (bgm->getValid()) bgm->stop();
	}
}

int main() {
	/* Init all services. */
	romfsInit();
	gfxInitDefault();
	Gui::init();

	/* Create Directories. */
	mkdir("sdmc:/3ds/LeafEdit", 0777);
	mkdir("sdmc:/3ds/LeafEdit/Pattern-Editor", 0777); // Main path.
	mkdir("sdmc:/3ds/LeafEdit/Pattern-Editor/Pattern", 0777); // Pattern path.
	mkdir("sdmc:/3ds/LeafEdit/Pattern-Editor/defaults", 0777); // Defaults path.

	Gui::loadSheet("romfs:/gfx/sprites.t3x", sprites);
	Gui::loadFont(fnt, "romfs:/gfx/font.bcfnt");
	osSetSpeedupEnable(true); // Enable speed-up for New 3DS users.

	Settings::Read();
	Lang::load(Settings::getLang());
	
	Overlays::SplashOverlay();
	Gui::setScreen(std::make_unique<PatternEditor>(), false, true);
	hidSetRepeatParameters(10, 10);

 	if (access("sdmc:/3ds/dspfirm.cdc", F_OK ) == 0) {
		ndspInit();
		dspFound = true;
		loadSoundEffects();
		playMusic();
	}

	/* MainLoop part here. */
	while(aptMainLoop()) {
		u32 hDown = hidKeysDown();
		u32 hHeld = hidKeysHeld();
		touchPosition touch;
		hidScanInput();
		hidTouchRead(&touch);

		/* Drawing part. */
		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		Gui::DrawScreen(true);
		Gui::ScreenLogic(hDown, hHeld, touch, true, true);
		C3D_FrameEnd(0);

		if (exiting) {
			if (songIsFound && bgm) {
				if (bgm->getValid()) stopMusic();
			}

			bgm = nullptr;
			if (fadeout && !bgm) break;
		}

		Gui::fadeEffects(16, 16, true);
	}
	

	if (dspFound) ndspExit();
	/* Unload and deinit. */
	Gui::unloadSheet(sprites);
	Gui::unloadFont(fnt);
	Settings::Save();
	Gui::exit();
	gfxExit();
	romfsExit();
	return 0;
}