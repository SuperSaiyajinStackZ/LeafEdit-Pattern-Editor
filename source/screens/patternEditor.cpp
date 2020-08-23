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

#include "coreUtils.hpp"
#include "kbd.hpp"
#include "overlay.hpp"
#include "patternEditor.hpp"
#include "screenCommon.hpp"
#include "settings.hpp"
#include "stringUtils.hpp"
#include "utils.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool exiting;

/* Needed to display Palettes. */
static const u32 WWPaletteColors[] = {
	0xFF0000FF, 0xFF3173FF, 0xFF00ADFF, 0xFF00FFFF, 0xFF00FFAD, 0xFF00FF52, 0xFF00FF00, 0xFF52AD00, 0xFFAD5200, 0xFFFF0000, 0xFFFF0052, 0xFFFF00AD, 0xFFFF00FF, 0xFF000000, 0xFFFFFFFF,
	0xFF7B7BFF, 0xFF7BB5FF, 0xFF7BE7FF, 0xFF7BFFFF, 0xFF7BFFDE, 0xFF7BFFAD, 0xFF7BFF7B, 0xFF84AD52, 0xFFAD8452, 0xFFFF7B7B, 0xFFFF7BB5, 0xFFFF7BE7, 0xFFFF7BFF, 0xFF000000, 0xFFFFFFFF,
	0xFF0000A5, 0xFF0031A5, 0xFF0073A5, 0xFF00A5A5, 0xFF00A573, 0xFF00A531, 0xFF00A500, 0xFF215200, 0xFF522100, 0xFFA50000, 0xFFA50031, 0xFFA50073, 0xFFA500A5, 0xFF000000, 0xFFFFFFFF,
	0xFF009C00, 0xFF6BCE5A, 0xFFDEFFB5, 0xFF6B9C00, 0xFFA5CE52, 0xFFD6FFAD, 0xFFAD5200, 0xFFD68429, 0xFFFFAD5A, 0xFFFF0000, 0xFFFF6B4A, 0xFFDE4A31, 0xFFB52118, 0xFF8C0000, 0xFFFFFFFF,
	0xFF0073AD, 0xFF42ADD6, 0xFF8CDEFF, 0xFF3908FF, 0xFF6B4AFF, 0xFF9C94FF, 0xFFFF00AD, 0xFFFF63D6, 0xFFFFCEFF, 0xFF9CBDFF, 0xFF7394DE, 0xFF4A63BD, 0xFF21399C, 0xFF00107B, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0052FF, 0xFF5AB5FF, 0xFFADEFFF, 0xFF00107B, 0xFF314AA5, 0xFF6B84D6, 0xFF9CBDFF, 0xFFFFAD5A, 0xFFFFC684, 0xFFFFE7AD, 0xFFFFFFD6, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFF42FF42, 0xFF8CFF8C, 0xFFD6FFD6, 0xFFFF0000, 0xFFFF4242, 0xFFFF8C8C, 0xFFFFD6D6, 0xFF0000FF, 0xFF4242FF, 0xFF8C8CFF, 0xFFD6D6FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF003100, 0xFF426342, 0xFF849C84, 0xFFC6D6C6, 0xFF00107B, 0xFF294AA5, 0xFF5A8CD6, 0xFF8CC6FF, 0xFF00B5D6, 0xFF39CEE7, 0xFF7BDEF7, 0xFFBDF7FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFFFF0000, 0xFF0000FF, 0xFF00FFFF, 0xFFFF4242, 0xFF4242FF, 0xFF42FFFF, 0xFFFF8C8C, 0xFF8C8CFF, 0xFF8CFFFF, 0xFFFFD6D6, 0xFFD6D6FF, 0xFFD6FFFF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFFFF0000, 0xFFFF00FF, 0xFF42FF42, 0xFFFF4242, 0xFFFF42FF, 0xFF8CFF8C, 0xFFFF8C8C, 0xFFFF8CFF, 0xFFD6FFD6, 0xFFFFD6D6, 0xFFFFD6FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF0000FF, 0xFF007BFF, 0xFF00FFFF, 0xFF00FF84, 0xFF00FF00, 0xFF7B8400, 0xFFFF0000, 0xFFFF007B, 0xFFFF94FF, 0xFF00B5D6, 0xFF0010BD, 0xFF00105A, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF639410, 0xFF527B08, 0xFF398C10, 0xFF319C31, 0xFF4AA5CE, 0xFF3994CE, 0xFF4A8CBD, 0xFF318CD6, 0xFF4A73AD, 0xFF315A8C, 0xFF29426B, 0xFFFFEF84, 0xFFEFCE31, 0xFFC6A500, 0xFFFFFFFF,
	0xFFE7DED6, 0xFFDECEB5, 0xFFEFEFE7, 0xFFF7F7F7, 0xFF7B7384, 0xFF6B8C94, 0xFF637B84, 0xFF5A849C, 0xFFB59C73, 0xFF2929FF, 0xFF00FFFF, 0xFFFF2194, 0xFFBD9C00, 0xFF000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFEFEFF7, 0xFFDEDEE7, 0xFFCECED6, 0xFFB5B5C6, 0xFFA5A5B5, 0xFF9494A5, 0xFF84849C, 0xFF6B6B8C, 0xFF5A5A7B, 0xFF4A4A6B, 0xFF31315A, 0xFF21214A, 0xFF101042, 0xFF000031,
	0xFFFFFFFF, 0xFFEFEFEF, 0xFFDEDEDE, 0xFFCECECE, 0xFFB5B5B5, 0xFFA5A5A5, 0xFF949494, 0xFF848484, 0xFF6B6B6B, 0xFF5A5A5A, 0xFF4A4A4A, 0xFF313131, 0xFF212121, 0xFF101010, 0xFF000000,
	0xFF7B8CFF, 0xFF0000FF, 0xFF007BFF, 0xFF00FFFF, 0xFF008400, 0xFF00FF00, 0xFFFF0000, 0xFFFF9C00, 0xFFFF00D6, 0xFFFF6BFF, 0xFF00009C, 0xFF0094FF, 0xFF94BDFF, 0xFF000000, 0xFFFFFFFF
};

static const u32 NLPaletteColors[] = {
	0xFFFFEEFF, 0xFFAA99FF, 0xFF9955EE, 0xFFAA66FF, 0xFF6600FF, 0xFF7744BB, 0xFF5500CC, 0xFF330099, 0xFF332255, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFCCBBFF, 0xFF7777FF, 0xFF1133DD, 0xFF4455FF, 0xFF0000FF, 0xFF6666CC, 0xFF4444BB, 0xFF0000BB, 0xFF222288, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFEEEEEE,
	0xFFBBCCDD, 0xFF66CCFF, 0xFF2266DD, 0xFF22AAFF, 0xFF0066FF, 0xFF5588BB, 0xFF0044DD, 0xFF0044BB, 0xFF113366, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFDDDDDD,
	0xFFDDEEFF, 0xFFCCDDFF, 0xFFAACCFF, 0xFF88BBFF, 0xFF88AAFF, 0xFF6688DD, 0xFF4466BB, 0xFF335599, 0xFF224488, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFCCCCCC,
	0xFFFFCCFF, 0xFFFF88EE, 0xFFDD66CC, 0xFFCC88BB, 0xFFFF00CC, 0xFF996699, 0xFFAA0088, 0xFF770055, 0xFF440033, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFBBBBBB,
	0xFFFFBBFF, 0xFFFF99FF, 0xFFBB22DD, 0xFFEE55FF, 0xFFCC00FF, 0xFF775588, 0xFF9900BB, 0xFF660088, 0xFF440055, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFAAAAAA,
	0xFF99BBDD, 0xFF77AACC, 0xFF334477, 0xFF4477AA, 0xFF003399, 0xFF223377, 0xFF002255, 0xFF001133, 0xFF001122, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF999999,
	0xFFCCFFFF, 0xFF77FFFF, 0xFF22DDDD, 0xFF00FFFF, 0xFF00DDFF, 0xFF00AACC, 0xFF009999, 0xFF007788, 0xFF005555, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF888888,
	0xFFFFBBDD, 0xFFEE99BB, 0xFFCC3366, 0xFFFF5599, 0xFFFF0066, 0xFF884455, 0xFF990044, 0xFF660022, 0xFF331122, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF777777,
	0xFFFFBBBB, 0xFFFF9988, 0xFFAA3333, 0xFFEE5533, 0xFFFF0000, 0xFF883333, 0xFFAA0000, 0xFF661111, 0xFF220000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF666666,
	0xFFBBEE99, 0xFF77CC66, 0xFF116622, 0xFF33AA44, 0xFF338800, 0xFF557755, 0xFF005522, 0xFF223311, 0xFF112200, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF555555,
	0xFFBBFFDD, 0xFF88FFCC, 0xFF55AA88, 0xFF88DDAA, 0xFF00FF88, 0xFF99BBAA, 0xFF00BB66, 0xFF009955, 0xFF006633, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF444444,
	0xFFFFDDBB, 0xFFFFCC77, 0xFF995533, 0xFFFF9966, 0xFFFF7711, 0xFFAA7744, 0xFF774422, 0xFF772200, 0xFF441100, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF333333,
	0xFFFFFFAA, 0xFFFFFF55, 0xFFBB8800, 0xFFCCBB55, 0xFFFFCC00, 0xFFAA9944, 0xFF886600, 0xFF554400, 0xFF332200, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF222222,
	0xFFEEFFCC, 0xFFDDEEAA, 0xFFAACC33, 0xFFBBEE55, 0xFFCCFF00, 0xFFAAAA77, 0xFF99AA00, 0xFF778800, 0xFF334400, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000,
	0xFFAAFFAA, 0xFF77FF77, 0xFF44DD66, 0xFF00FF00, 0xFF22DD22, 0xFF55BB55, 0xFF00BB00, 0xFF008800, 0xFF224422, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

/* Load a savefile. */
void PatternEditor::loadSave() {
	if (!this->saveLoaded) {
		if (Overlays::SelectSaveFile(this->savefile)) {
			this->saveLoaded = true;
			this->overloadSaveStuff();
		}
	} else {
		Msg::DisplayWaitMsg(Lang::get("SAVE_LOAD_MSG"));
	}
}

/* Overload region and savetype from the savefile. */
void PatternEditor::overloadSaveStuff() {
	if (this->savefile && this->saveLoaded) {
		this->savetype = this->savefile->getType();
		this->saveregion = this->savefile->getRegion();
	}
}

/* Save changes to file and close the savefile. */
void PatternEditor::saveStuff() {
	if (this->savefile) {
		if (this->saveLoaded) {
			this->savefile->Finish();
			FILE *file = fopen(this->savefile->getFileLocation().c_str(), "wb");
			fwrite(this->savefile->rawData().get(), 1, this->savefile->getLength(), file);
			fclose(file);
			Msg::DisplayWaitMsg(Lang::get("SAVED_CHANGES"));
			this->unloadSave();
			return;
		}
	}

	Msg::DisplayWaitMsg(Lang::get("NO_SAVE_LOADED"));
}

/* Unload a savefile. */
void PatternEditor::unloadSave() {
	if (this->saveLoaded) {
		this->saveLoaded = false;
		this->savefile = nullptr;
	} else {
		Msg::DisplayWaitMsg(Lang::get("ALREADY_UNLOADED"));
	}
}

/* Load a pattern from the savefile. */
void PatternEditor::PatternFromSaveLoad() {
	if (this->savefile) {
		if (this->saveLoaded) {
			if (Overlays::SelectSavePattern(this->savefile, this->pattern)) {
				C3D_FrameEnd(0);
				this->overloadSaveStuff();
				this->image = this->pattern->image(0);
				this->patternImage = CoreUtils::patternImage(this->image, this->savetype);
				return;
			}
		}
	}

	Msg::DisplayWaitMsg(Lang::get("NO_SAVE_LOADED2"));
}

/* Return the save name here. */
const std::string PatternEditor::getSaveName() const {
	switch(this->savetype) {
		case SaveType::WW:
			return "Wild World";
		case SaveType::NL:
			return "New Leaf";
		case SaveType::WA:
			return "Welcome Amiibo";
		case SaveType::UNUSED:
			return "?";
	}

	return "?";
}

/* Return the region name here. */
const std::string PatternEditor::getRegionName() const {
	switch(this->saveregion) {
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return "Japanese";
		case WWRegion::USA_REV0:
			return "USA";
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return "Europe";
		case WWRegion::KOR_REV1:
			return "Korean";
		case WWRegion::UNKNOWN:
			return "?";
	}

	return "?";
}

/* Automatically detect Pattern type here. */
void PatternEditor::getPattern(const std::string ptrnFile) {
	// Reset to invalid.
	this->saveregion = WWRegion::UNKNOWN;
	this->savetype = SaveType::UNUSED;
	this->patternSize = 0;
	this->isValid = false;

	FILE *file = fopen(ptrnFile.c_str(), "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		this->patternSize = ftell(file);
		fseek(file, 0, SEEK_SET);
		this->data = std::shared_ptr<u8[]>(new u8[this->patternSize]);
		fread(this->data.get(), 1, this->patternSize, file);
		fclose(file);

		/* Detect format here. */
		switch(this->patternSize) {
			case 0x220:
				this->saveregion = WWRegion::JPN_REV0;
				this->savetype = SaveType::WW;
				this->isValid = true;
				break;

			case 0x228:
				this->saveregion = WWRegion::EUR_REV1; // Is also USA.
				this->savetype = SaveType::WW;
				this->isValid = true;
				break;

			case 0x234:
				this->saveregion = WWRegion::KOR_REV1;
				this->savetype = SaveType::WW;
				this->isValid = true;
				break;

			case 0x26C:
			case 0x870:
				this->saveregion = WWRegion::UNKNOWN;
				this->savetype = SaveType::NL; // Is also AC:WA format, but they're the same.
				this->isValid = true;
				break;
		}
	}
}

/* Load the C2D_Image of the pattern data and such. */
void PatternEditor::load(const std::string ptrnFile, bool fromFile) {
	if (fromFile) this->getPattern(ptrnFile);

	if (this->isValid && this->data != nullptr) {
		switch(this->savetype) {
			case SaveType::NL:
				this->pattern = std::make_shared<PatternNL>(this->data, 0);
				break;
			case SaveType::WA:
				this->pattern = std::make_shared<PatternWA>(this->data, 0);
				break;
			case SaveType::WW:
				this->pattern = std::make_shared<PatternWW>(this->data, 0, this->saveregion);
				break;
			case SaveType::UNUSED:
				break;
		}

		C3D_FrameEnd(0);
		this->image = this->pattern->image(0);
		this->patternImage = CoreUtils::patternImage(this->image, this->savetype);
	}
}

/* Load Empty Pattern. */
PatternEditor::PatternEditor() {
	this->load(Settings::getDefaultPath(), true);
	PatternInformations info = CoreUtils::getDefaultInformation(this->savetype, this->saveregion);

	/* Strings. */
	this->pattern->creatorid(info.CreatorID);
	this->pattern->origtownid(info.TownID);
	this->pattern->creatorGender(info.CreatorGender);

	/* ID's. */
	this->pattern->creatorname(info.CreatorName);
	this->pattern->origtownname(info.TownName);
	this->pattern->name(info.PatternName);
}

/* Destroy C2D_Image, if exist. */
PatternEditor::~PatternEditor() {
	if (this->isValid) {
		if (this->patternImage.subtex != nullptr) C2DUtils::C2D_ImageDelete(this->patternImage);
	}
}

void PatternEditor::Draw(void) const {
	bool isAcww = false;
	if (this->savetype == SaveType::WW) isAcww = true;

	UI::DrawBase(true, true);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);

	if (this->isValid) {

		if (this->saveLoaded) {
			Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("PATTERN_EDITOR") + " - " + Lang::get("SAVE_MODE"), 395, 0, fnt);
		} else {
			Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("PATTERN_EDITOR") + " - " + Lang::get("RAW_MODE"), 395, 0, fnt);
		}

		Gui::DrawStringCentered(0, isAcww ? 30 : 40, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_NAME") + StringUtils::UTF16toUTF8(this->pattern->name()), 395, 0, fnt);
		Gui::DrawStringCentered(0, isAcww ? 50 : 60, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_NAME") + StringUtils::UTF16toUTF8(this->pattern->creatorname()), 395, 0, fnt);
		Gui::DrawStringCentered(0, isAcww ? 70 : 80, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_ID") + std::to_string(this->pattern->creatorid()), 395, 0, fnt);
		Gui::DrawStringCentered(0, isAcww ? 90 : 100, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_ORIGIN_TOWNNAME") + StringUtils::UTF16toUTF8(this->pattern->origtownname()), 395, 0, fnt);
		Gui::DrawStringCentered(0, isAcww ? 110 : 120, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_ORIGIN_TOWNID") + std::to_string(this->pattern->origtownid()), 395, 0, fnt);

		if (this->pattern->creatorGender()) {
			Gui::DrawStringCentered(0, isAcww ? 130 : 140, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_GENDER") + Lang::get("FEMALE"), 395, 0, fnt);
		} else {
			Gui::DrawStringCentered(0, isAcww ? 130 : 140, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_GENDER") + Lang::get("MALE"), 395, 0, fnt);
		}
		
		Gui::DrawStringCentered(0, isAcww ? 150 : 160, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("DESIGNTYPE") + std::to_string(this->pattern->designtype()), 395, 0, fnt);

		/* Display Savetype. */
		Gui::DrawStringCentered(0, isAcww ? 170 : 180, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("GAME") + this->getSaveName(), 395, 0, fnt);

		/* Display Region, if on AC:WW. */
		if (this->savetype == SaveType::WW) {
			Gui::DrawStringCentered(0, 190, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("REGION") + this->getRegionName(), 395, 0, fnt);
		}

		Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SHOW_INSTRUCTION"), 395, 0, fnt);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

		UI::DrawBase(false, false);
		if (this->patternImage.subtex) C2D_DrawImageAt(this->patternImage, 8, 8, 0.5f, nullptr, 7, 7); // 224x224. 224/32 -> 7.

		/* Drawing Palette. */
		if (this->savetype == SaveType::WW) {
			for (int i = 0; i < 15; i++) {
				if (i == this->color) {
					UI::DrawPaletteGrid(palettePos[i].x, palettePos[i].y, palettePos[i].w, palettePos[i].h, WWPaletteColors[this->image->getPaletteColor(i)], C2D_Color32(160, 0, 0, 255));
				} else {
					UI::DrawPaletteGrid(palettePos[i].x, palettePos[i].y, palettePos[i].w, palettePos[i].h, WWPaletteColors[this->image->getPaletteColor(i)], C2D_Color32(20, 20, 20, 255));
				}
			}

		} else if (this->savetype == SaveType::NL || this->savetype == SaveType::WA) {
			for (int i = 0; i < 15; i++) {
				if (i == this->color) {
					UI::DrawPaletteGrid(palettePos[i].x, palettePos[i].y, palettePos[i].w, palettePos[i].h, NLPaletteColors[this->image->getPaletteColor(i)], C2D_Color32(160, 0, 0, 255));
				} else {
					UI::DrawPaletteGrid(palettePos[i].x, palettePos[i].y, palettePos[i].w, palettePos[i].h, NLPaletteColors[this->image->getPaletteColor(i)], C2D_Color32(20, 20, 20, 255));
				}
			}
		}

		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

		/* Invalid!! */
	} else {
		Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("PATTERN_EDITOR") + " - " + Lang::get("INVALID_PATTERN"), 395, 0, fnt);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		UI::DrawBase(false, false);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	}
}

void PatternEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	/* Pattern Tool Menu. */
	if (hDown & KEY_START) {
		this->mode = Overlays::ToolSelect(this->patternImage);
	}

	if (this->mode == PatternMode::LoadSave) {
		this->loadSave();
		this->mode = PatternMode::Draw;
	}

	if (this->mode == PatternMode::LoadFromSave) {
		this->PatternFromSaveLoad();
		this->mode = PatternMode::Draw;
	}

	if (this->mode == PatternMode::UnloadSave) {
		this->unloadSave();
		CoreUtils::generateEmptyPattern(this->savetype, this->saveregion, this->data, this->patternSize);
		this->load("", false);
		this->mode = PatternMode::Draw;
	}

	if (this->mode == PatternMode::DoSave) {
		this->saveStuff();
		CoreUtils::generateEmptyPattern(this->savetype, this->saveregion, this->data, this->patternSize);
		this->load("", false);
		this->mode = PatternMode::Draw;
	}

	/* Exit the app. */
	if (this->mode == PatternMode::Exit) {
		exiting = true;
	}

	/* Credits Mode. */
	if (this->mode == PatternMode::Credits) {
		Overlays::CreditsOverlay();
		this->mode = PatternMode::Draw;
	}

	if (this->mode == PatternMode::ExportInformation) {
		if (Msg::promptMsg("EXPORT_INF_PROMPT")) {
			CoreUtils::dumpPatternInformation(this->savetype, this->saveregion, this->pattern);
			Msg::DisplayWaitMsg(Lang::get("EXPORT_INFORMATION_PROMPT"));
		}

		this->mode = PatternMode::Draw;
	}

	/* Default Pattern Set Mode. */
	if (this->mode == PatternMode::DefaultPattern) {
		if (Msg::promptMsg("DEFAULT_PATTERN_PROMPT")) {
			std::string file;
			bool result = Overlays::SelectPattern(1, file);

			if (result) Settings::setDefaultPath(file);
		}

		this->mode = PatternMode::Draw;
	}

	/* Language Selection Mode. */
	if (this->mode == PatternMode::LangSet) {
		Overlays::SelectLang();
		this->mode = PatternMode::Draw;
	}

	/* Share Mode. */
	if (this->mode == PatternMode::Share) {
		std::string comment = "";
		if (Msg::promptMsg("ENTER_COMMENT_PROMPT")) {
			comment = KBD::kbdString(40, Lang::get("ENTER_COMMENT"));
		}
		
		Overlays::ShareOverlay(this->image, this->patternImage, this->pattern, comment);
		this->mode = PatternMode::Draw;
	}

	if (this->mode == PatternMode::SetDefault) {
		if (Msg::promptMsg("SET_DEFAULT_PROMPT")) {
			PatternInformations info = CoreUtils::getDefaultInformation(this->savetype, this->saveregion);
		
			/* ID's. */
			this->pattern->creatorid(info.CreatorID);
			this->pattern->origtownid(info.TownID);
			this->pattern->creatorGender(info.CreatorGender);

			/* Strings. */
			this->pattern->creatorname(info.CreatorName);
			this->pattern->origtownname(info.TownName);
			this->pattern->name(info.PatternName);
		}

		this->mode = PatternMode::Draw;
	}

	/* Clear Mode. */
	if (this->mode == PatternMode::Clear) {
		/* Select SaveType. */
		if (this->saveLoaded) {
			CoreUtils::generateEmptyPattern(this->savetype, this->saveregion, this->data, this->patternSize);
			this->load("", false);
		} else {
			bool result = Overlays::SaveSelect(this->savetype, this->saveregion);
			if (result) {
				CoreUtils::generateEmptyPattern(this->savetype, this->saveregion, this->data, this->patternSize);
				this->load("", false);
			}
		}

		this->mode = PatternMode::Draw;
	}

	/* Palette mode. */
	if (this->mode == PatternMode::Palette) {
		Overlays::PaletteTool(this->image, this->patternImage, this->savetype);
		this->mode = PatternMode::Draw;
	}

	/* Import mode. */
	if (this->mode == PatternMode::Import) {
		if (Msg::promptMsg("IMPORT_PROMPT")) {
			std::string file;
			bool result = Overlays::SelectPattern(0, file);

			if (result) {
				this->load(file, true);
			}
		}

		this->mode = PatternMode::Draw;
	}

	/* Export mode. */
	if (this->mode == PatternMode::Export) {
		if (Msg::promptMsg("EXPORT_PTRN_PROMPT")) {
			if (this->patternSize > 0 && this->data != nullptr) {
				/* Here we save the pattern. */
				std::string destination;
				bool result = Overlays::SelectDestination(Lang::get("SELECT_PATTERN_DEST"), destination);

				if (result) {
					/* Enter the name of the pattern. */
					destination += KBD::kbdString(20, Lang::get("ENTER_PATTERN_NAME"));

					/* Get the extension for the Pattern. */
					switch(this->savetype) {
						case SaveType::WW:
							destination += ".acww";
							break;
						case SaveType::NL:
						case SaveType::WA:
							destination += ".acnl";
							break;
						case SaveType::UNUSED:
							destination += ".invalid";
							break;
					}

					FILE *file = fopen(destination.c_str(), "wb");
					fwrite(this->data.get(), 1, this->patternSize, file);
					fclose(file);
					Msg::DisplayWaitMsg(Lang::get("SAVED_TO_PROMPT") + "\n" + destination + ".");
				}
			}
		}

		this->mode = PatternMode::Draw;
	}


	/* Pattern drawing mode. */
	if (this->mode == PatternMode::Draw) {
		if (this->isValid) {

			/* Normal 1x1 PX mode. */
			if (this->drawMode == DrawTool::Normal) {

				if (hHeld & KEY_TOUCH) {
					bool didTouch = false;
					for (int x = 0; x < 32; x++) {
						for (int y = 0; y < 32; y++) {
							if (touch.px <= (8 + 7 + x * 7) && touch.px >= (8 + x * 7) && touch.py <= (8 + 7 + y * 7) && touch.py >= (8 + y * 7)) {
								if (this->savetype == SaveType::WW) this->image->setPixel(x, y, this->color + 1);
								else if (this->savetype == SaveType::NL || this->savetype == SaveType::WA) this->image->setPixel(x, y, this->color);
								didTouch = true;
								break;
							}
						}
					}

					/* If we didn't touched the Pattern. */
					if (!didTouch) {
						for (int i = 0; i < 15; i++) {
							if (touching(touch, palettePos[i])) {
								this->color = i;
							}
						}
					}

					if (didTouch) {
						C3D_FrameEnd(0);
						if (this->patternImage.subtex != nullptr) C2DUtils::C2D_ImageDelete(this->patternImage);
						this->patternImage = CoreUtils::patternImage(this->image, this->savetype);
					}
				}

				/* Line Mode. TODO.*/
			} else if (this->drawMode == DrawTool::Line) {

			}
		}
	}

	/* Instructions. */
	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("EDITOR_INSTRUCTIONS"));
	}
}