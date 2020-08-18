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
#include "stringUtils.hpp"
#include "utils.hpp"

/* Return the save name here. */
const std::string PatternEditor::getSaveName() const {
	switch(this->savetype) {
		case SaveType::WW:
			return "Wild World";
		case SaveType::NL:
		case SaveType::WA:
			return "New Leaf | Welcome Amiibo";
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
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return "Europe | USA";
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
	this->savetype = SaveType::NL;
	CoreUtils::generateEmptyPattern(this->savetype, this->saveregion, this->data);
	this->isValid = true;
	this->load("", false);
}

/* Destroy C2D_Image, if exist. */
PatternEditor::~PatternEditor() {
	if (this->isValid) {
		if (this->patternImage.subtex != nullptr) C2DUtils::C2D_ImageDelete(this->patternImage);
	}
}

void PatternEditor::Draw(void) const {
	UI::DrawBase(true, true);
	if (this->isValid) {
		Gui::DrawStringCentered(0, -2, 0.8f, C2D_Color32(255, 255, 255, 255), "Pattern Editor", 395, 0);
		Gui::DrawStringCentered(0, 40, 0.6f, C2D_Color32(255, 255, 255, 255), "Pattern Name: " + StringUtils::UTF16toUTF8(this->pattern->name()), 395, 0);
		Gui::DrawStringCentered(0, 60, 0.6f, C2D_Color32(255, 255, 255, 255), "Creator Name: " + StringUtils::UTF16toUTF8(this->pattern->creatorname()), 395, 0);
		Gui::DrawStringCentered(0, 80, 0.6f, C2D_Color32(255, 255, 255, 255), "Creator ID: " + std::to_string(this->pattern->creatorid()), 395, 0);
		Gui::DrawStringCentered(0, 100, 0.6f, C2D_Color32(255, 255, 255, 255), "Origin Town Name: " + StringUtils::UTF16toUTF8(this->pattern->origtownname()), 395, 0);
		Gui::DrawStringCentered(0, 120, 0.6f, C2D_Color32(255, 255, 255, 255), "Origin Town ID: " + std::to_string(this->pattern->origtownid()), 395, 0);
		if (this->pattern->creatorGender()) {
			Gui::DrawStringCentered(0, 140, 0.6f, C2D_Color32(255, 255, 255, 255), "Gender: Female", 395, 0);
		} else {
			Gui::DrawStringCentered(0, 140, 0.6f, C2D_Color32(255, 255, 255, 255), "Gender: Male", 395, 0);
		}

		/* Display Region and such. */
		Gui::DrawStringCentered(0, 160, 0.6f, C2D_Color32(255, 255, 255, 255), "Savetype: " + this->getSaveName(), 395, 0);

		/* Display Region, if on AC:WW. */
		if (this->savetype == SaveType::WW) {
			Gui::DrawStringCentered(0, 180, 0.6f, C2D_Color32(255, 255, 255, 255), "Region: " + this->getRegionName(), 395, 0);
		}

		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		UI::DrawBase(false, false);
		if (this->patternImage.subtex) C2D_DrawImageAt(this->patternImage, 8, 8, 0.5f, nullptr, 7, 7); // 224x224. 224/32 -> 7.

		/* TODO: Grid. */
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

		/* Invalid!! */
	} else {
		Gui::DrawStringCentered(0, -2, 0.8f, C2D_Color32(255, 255, 255, 255), "Pattern Editor - Invalid Pattern!", 395, 0);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		UI::DrawBase(false, false);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	}
}

void PatternEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_SELECT) {
		Gui::screenBack(true);
	}

	/* Loading | Selecting another pattern. */
	if (hDown & KEY_X) {
		const std::string file = Overlays::SelectPattern();

		if (file != "!NO_PATTERN") {
			this->load(file, true);
		}
	}

	if (hDown & KEY_START) {
		/* Here we save the pattern. */
		std::string destination = Overlays::SelectDestination("Select the destination for the pattern.", "sdmc:/3ds/LeafEdit/Pattern-Editor/");

		/* Enter the name of the pattern. */
		destination += KBD::kbdString(20, "Enter the Pattern name (without extension).");

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
		Msg::DisplayWaitMsg("Properly saved pattern to file.");
	}
}