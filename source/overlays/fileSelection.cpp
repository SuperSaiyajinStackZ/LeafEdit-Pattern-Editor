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
#include "fileBrowse.hpp"
#include "overlay.hpp"
#include <dirent.h>
#include <unistd.h>

/* Draw the Top (file) browse. */
static void DrawTop(std::string txt, uint Selection, std::vector<DirEntry> dirContents) {
	std::string files;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 190));
	Gui::DrawStringCentered(0, -2, 0.7f, C2D_Color32(255, 255, 255, 255), txt, 390);

	for (uint i = (Selection < 5) ? 0 : Selection - 5; i < dirContents.size() && i < ((Selection < 5) ? 6 : Selection + 1); i++) {
		if (i == Selection) {
			files += "> " + dirContents[i].name + "\n\n";
		} else {
			files += dirContents[i].name + "\n\n";
		}
	}

	for (uint i = 0; i < ((dirContents.size() < 6) ? 6 - dirContents.size() : 0); i++) {
		files += "\n\n";
	}

	Gui::DrawString(26, 32, 0.53f, C2D_Color32(255, 255, 255, 255), files, 360);
	Gui::DrawStringCentered(0, 217, 0.6f, C2D_Color32(255, 255, 255, 255), "Press START to refresh the list.", 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

static void DrawBottom() {
	UI::DrawBase(false, true);
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	C3D_FrameEnd(0);
}

std::string Overlays::SelectFile(std::string initialPath, std::vector<std::string> extensions, std::string txt) {
	s32 selectedFile = 0;
	std::vector<DirEntry> dirContents;
	bool dirChanged = true;

	/* Initial directory change. */
	dirContents.clear();

	chdir(initialPath.c_str());
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, extensions);
	for(uint i = 0; i < dirContentsTemp.size(); i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		/* Screen draw part. */
		DrawTop(txt, selectedFile, dirContents);
		DrawBottom();

		/* The input part. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		/* if directory changed -> Refresh it. */
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, extensions);
			for(uint i = 0; i < dirContentsTemp.size(); i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			dirChanged = false;
		}

		if (hDown & KEY_A) {
			if (!dirContents[selectedFile].isDirectory) {
				char path[PATH_MAX];
				getcwd(path, PATH_MAX);
				return path + dirContents[selectedFile].name;
			}
		}

		if (hRepeat & KEY_UP) {
			if (selectedFile > 0) {
				selectedFile--;
			}
		}
		
		if (hRepeat & KEY_DOWN) {
			if ((uint)selectedFile < dirContents.size()-1) {
				selectedFile++;
			}
		}
		
		if (hDown & KEY_B) {
			return "";
		}
		
		if (hDown & KEY_START) {
			dirChanged = true;
		}
	}
}