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
#include "stringUtils.hpp"

static void Draw(std::unique_ptr<PatternImage> &pImg, C2D_Image &img, std::unique_ptr<Pattern> &ptrn, const std::string &msg) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	UI::DrawBase(true, true);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, -2, 0.9, C2D_Color32(255, 255, 255, 255), StringUtils::UTF16toUTF8(ptrn->name()) + " " + Lang::get("CREATED_BY") + StringUtils::UTF16toUTF8(ptrn->creatorname()) + ".", 395, 100, fnt);
	if (img.subtex != nullptr) C2D_DrawImageAt(img, 18, 42, 0.5f, nullptr, 5, 5); // 160x160. 160/32 -> 5.

	Gui::DrawString(210, 60, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_NAME") + StringUtils::UTF16toUTF8(ptrn->name()), 160, 0, fnt);
	Gui::DrawString(210, 80, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_NAME") + StringUtils::UTF16toUTF8(ptrn->creatorname()), 160, 0, fnt);
	Gui::DrawString(210, 100, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_ID") + std::to_string(ptrn->creatorid()), 160, 0, fnt);
	Gui::DrawString(210, 120, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_ORIGIN_TOWNNAME") + StringUtils::UTF16toUTF8(ptrn->origtownname()), 160, 0, fnt);
	Gui::DrawString(210, 140, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_ORIGIN_TOWNID") + std::to_string(ptrn->origtownid()), 160, 0, fnt);

	if (ptrn->creatorGender()) {
		Gui::DrawString(210, 160, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_GENDER") + Lang::get("FEMALE"), 160, 0, fnt);
	} else {
		Gui::DrawString(210, 160, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_GENDER") + Lang::get("MALE"), 160, 0, fnt);
	}

	Gui::DrawStringCentered(0, 218, 0.8f, C2D_Color32(255, 255, 255, 255), msg, 395, 0, fnt);

	UI::DrawBase(false, true);
	UI::DrawSprite(sprites_top_bar_idx, 0, 0);
	Gui::DrawStringCentered(0, 0, 0.8, C2D_Color32(255, 255, 255, 255), Lang::get("SHARE_NOTE"), 310, 100, fnt);

	C3D_FrameEnd(0);
}

void Overlays::ShareOverlay(std::unique_ptr<PatternImage> &pImg, C2D_Image &img, std::unique_ptr<Pattern> &ptrn, const std::string &msg) {
	bool doOut = false;

	while(!doOut) {
		Draw(pImg, img, ptrn, msg);
		hidScanInput();

		if (hidKeysDown() & KEY_B) doOut = true;
	}
}