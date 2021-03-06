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
static void DrawTop(uint Selection, std::vector<DirEntry> dirContents) {
	std::string files;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawFileBrowseBG(true);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_SAVE_FILE"), 390, 0, fnt);

	for (uint i = (Selection < 8) ? 0 : Selection - 8; i < dirContents.size() && i < ((Selection < 8) ? 9 : Selection + 1); i++) {
		files += dirContents[i].name + "\n";
	}

	for (uint i = 0; i < ((dirContents.size() < 9) ? 9 - dirContents.size() : 0); i++) {
		files += "\n";
	}

	if (Selection < 9) UI::DrawSelector(true, 24 + ((int)Selection * 21));
	else UI::DrawSelector(true, 24 + (8 * 21));
	Gui::DrawString(5, 25, 0.85f, C2D_Color32(0, 0, 0, 255), files, 360, 0, fnt);
	Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("START_REFRESH"), 390, 0, fnt);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

static void DrawBottom() {
	UI::DrawFileBrowseBG(false);
	C3D_FrameEnd(0);
}

static void operation(std::unique_ptr<Sav> &savefile, std::string fileName) {
	savefile = nullptr;
	FILE *file = fopen(fileName.c_str(), "rb");
	
	if (file) {
		fseek(file, 0, SEEK_END);
		u32 size = ftell(file);
		fseek(file, 0, SEEK_SET);
		std::shared_ptr<u8[]> saveData = std::shared_ptr<u8[]>(new u8[size]);
		fread(saveData.get(), 1, size, file);
		fclose(file);
		savefile = Sav::getSave(saveData, size, fileName);
	}
}

bool Overlays::SelectSaveFile(std::unique_ptr<Sav> &savefile) {
	s32 selectedFile = 0;
	std::vector<DirEntry> dirContents;
	bool dirChanged = true;

	/* Initial directory change. */
	dirContents.clear();

	chdir("sdmc:/3ds/LeafEdit/");
	std::vector<DirEntry> dirContentsTemp;
	getDirectoryContents(dirContentsTemp, {"dat", "sav"});
	for(uint i = 0; i < dirContentsTemp.size(); i++) {
		dirContents.push_back(dirContentsTemp[i]);
	}

	while (1) {
		/* Screen draw part. */
		DrawTop(selectedFile, dirContents);
		DrawBottom();

		/* The input part. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		/* if directory changed -> Refresh it. */
		if (dirChanged) {
			dirContents.clear();
			std::vector<DirEntry> dirContentsTemp;
			getDirectoryContents(dirContentsTemp, {"dat", "sav"});
			for(uint i = 0; i < dirContentsTemp.size(); i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}

			dirChanged = false;
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
		
		if (hDown & KEY_A) {
			if (dirContents.size() > 0) {
				if (dirContents[selectedFile].isDirectory) {
					chdir(dirContents[selectedFile].name.c_str());
					selectedFile = 0;
					dirChanged = true;
				} else {
					char path[PATH_MAX];
					getcwd(path, PATH_MAX);
					operation(savefile, path + dirContents[selectedFile].name);
					return true;
				}
			}
		}

		if (hDown & KEY_B) {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			if (strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
				return false;
			} else {
				chdir("..");
				selectedFile = 0;
				dirChanged = true;
			}
		}
		
		if (hDown & KEY_START) {
			selectedFile = 0;
			dirChanged = true;
		}
	}
}