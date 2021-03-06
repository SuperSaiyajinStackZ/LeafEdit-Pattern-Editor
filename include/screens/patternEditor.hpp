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

#ifndef _LEAFEDIT_PATTERN_EDITOR_HPP
#define _LEAFEDIT_PATTERN_EDITOR_HPP

#include "common.hpp"
#include "overlay.hpp"
#include "Pattern.hpp"
#include "PatternHHD.hpp"
#include "PatternNL.hpp"
#include "PatternWA.hpp"
#include "PatternWW.hpp"
#include "PatternImage.hpp"
#include "Sav.hpp"
#include "storage.hpp"
#include "structs.hpp"

#include <vector>

class PatternEditor : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	PatternEditor();
	~PatternEditor();
private:
	/* Functions. */
	void load(const std::string ptrnFile, bool fromFile);
	void getPattern(const std::string ptrnFile);
	const std::string getSaveName() const;
	const std::string getRegionName() const;
	void saveStuff();
	void overloadSaveStuff();
	void unloadSave();
	void loadSave();
	void PatternFromSaveLoad();

	PatternMode mode = PatternMode::Draw;
	DrawTool drawMode = DrawTool::Normal;
	
	/* Save / Region Related stuff. */
	bool isValid = false;
	SaveType savetype = SaveType::UNUSED;
	WWRegion saveregion = WWRegion::UNKNOWN;

	std::unique_ptr<Sav> savefile = nullptr;
	bool saveLoaded = false;
	bool patternFromSave = false;

	/* Data. */
	std::unique_ptr<u8[]> data = nullptr;
	u32 patternSize = 0;
	std::unique_ptr<Pattern> pattern;
	std::unique_ptr<PatternImage> image;
	std::unique_ptr<Storage> storage;
	C2D_Image patternImage;

	/* Palette Stuff. */
	int color = 0, xPos = 0, yPos = 0;

	const std::vector<Structs::ButtonPos> palettePos = {
		{260, 7, 22, 22},
		{290, 7, 22, 22},
		{260, 35, 22, 22},
		{290, 35, 22, 22},
		{260, 63, 22, 22},
		{290, 63, 22, 22},
		{260, 91, 22, 22},
		{290, 91, 22, 22},
		{260, 119, 22, 22},
		{290, 119, 22, 22},
		{260, 147, 22, 22},
		{290, 147, 22, 22},
		{260, 175, 22, 22},
		{290, 175, 22, 22},
		{260, 203, 22, 22},
		{290, 203, 22, 22}
	};
};

#endif