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

#ifndef _LEAFEDIT_PATTERN_EDITOR_OVERLAY_HPP
#define _LEAFEDIT_PATTERN_EDITOR_OVERLAY_HPP

#include "common.hpp"
#include "Pattern.hpp"
#include "PatternImage.hpp"
#include <string>
#include <vector>

enum class PatternMode {
	/* Main modes. */
	Draw, // Always the default mode.
	Clear,
	Import,
	Export,
	Palette,
	Credits,
	Exit,
	SetDefault,
	LangSet,
	DefaultPattern,
	ExportInformation,
	Share,

	/* These are TODO's. */
	DrawToolSelect,
	ImportBMP,
	ExportBMP,

	/* Pattern changing stuff. TODO. */
	Name,
	CreatorName,
	CreatorID,
	TownName,
	TownID,
	CreatorGender
};

/* TODO. */
enum class DrawTool {
	Normal,
	Line
};

namespace Overlays {
	void SplashOverlay();
	void CreditsOverlay();
	void SelectLang();
	void ShareOverlay(std::shared_ptr<PatternImage> &pImg, C2D_Image &img, std::shared_ptr<Pattern> &ptrn, const std::string &msg);

	bool SelectPattern(int sltMode, std::string &file);
	bool SelectDestination(std::string txt, std::string &file);

	void PaletteToolWW(std::shared_ptr<PatternImage> &pImg, C2D_Image &img);
	void PaletteToolNL(std::shared_ptr<PatternImage> &pImg, C2D_Image &img);
	void PaletteTool(std::shared_ptr<PatternImage> &pImg, C2D_Image &img, SaveType ST);

	PatternMode ToolSelect(C2D_Image &Img);
	bool SaveSelect(SaveType &ST, WWRegion &region);
}

#endif