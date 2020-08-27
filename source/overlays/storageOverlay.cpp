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
#include "overlay.hpp"
#include "storage.hpp"
#include "stringUtils.hpp"
#include "structs.hpp"
#include "utils.hpp"


/* Main Buttons. */
static const std::vector<Button> buttons = {
	{95, 34, 130, 48, "PLAYER"},
	{95, 97, 130, 48, "ABLE_SISTER"},
	{95, 159, 130, 48, "TOWNFLAG"}
};

/* Player Buttons. */
static const std::vector<Button> playerPos = {
	{15, 48, 130, 48, "PLAYER_1"},
	{175, 48, 130, 48, "PLAYER_2"},
	{15, 144, 130, 48, "PLAYER_3"},
	{175, 144, 130, 48, "PLAYER_4"}
};

static const std::vector<Structs::ButtonPos> Pattern8 = {
	{40, 60, 48, 48},
	{100, 60, 48, 48},
	{160, 60, 48, 48},
	{220, 60, 48, 48},

	{15, 140, 48, 48},
	{100, 140, 48, 48},
	{160, 140, 48, 48},
	{220, 140, 48, 48}
};

static const std::vector<Structs::ButtonPos> Pattern10 = {
	{17, 60, 48, 48},
	{77, 60, 48, 48},
	{137, 60, 48, 48},
	{197, 60, 48, 48},
	{257, 60, 48, 48},

	{17, 140, 48, 48},
	{77, 140, 48, 48},
	{137, 140, 48, 48},
	{197, 140, 48, 48},
	{257, 140, 48, 48}
};

/* If button Position pressed -> Do something. */
static bool touching(touchPosition touch, Button button) {
	if (touch.px >= button.X && touch.px <= (button.X + button.XSize) && touch.py >= button.Y && touch.py <= (button.Y + button.YSize)) return true;
	else return false;
}

/* Return the save name here. */
static std::string getSaveName(SaveType savetype) {
	switch(savetype) {
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
static std::string getRegionName(WWRegion saveregion) {
	switch(saveregion) {
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

static void DisplayInfo(std::unique_ptr<Pattern> &ptrn, C2D_Image &img) {
	bool isAcww = false;

	if (ptrn) {
		if (ptrn->getType() == SaveType::WW) isAcww = true;
	}

	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
	UI::DrawBase(true, true);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	if (img.subtex != nullptr) C2D_DrawImageAt(img, 18, 42, 0.5f, nullptr, 5, 5); // 160x160. 160/32 -> 5.

	if (ptrn) {
		Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("PATTERN_INFORMATION"), 395, 0, fnt);
		Gui::DrawString(210, isAcww ? 30 : 40, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_NAME") + StringUtils::UTF16toUTF8(ptrn->name()), 160, 0, fnt);
		Gui::DrawString(210, isAcww ? 50 : 60, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_NAME") + StringUtils::UTF16toUTF8(ptrn->creatorname()), 160, 0, fnt);
		Gui::DrawString(210, isAcww ? 70 : 80, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_ID") + std::to_string(ptrn->creatorid()), 160, 0, fnt);
		Gui::DrawString(210, isAcww ? 90 : 100, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_ORIGIN_TOWNNAME") + StringUtils::UTF16toUTF8(ptrn->origtownname()), 160, 0, fnt);
		Gui::DrawString(210, isAcww ? 110 : 120, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_ORIGIN_TOWNID") + std::to_string(ptrn->origtownid()), 160, 0, fnt);

		if (ptrn->creatorGender()) {
			Gui::DrawString(210, isAcww ? 130 : 140, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_GENDER") + Lang::get("FEMALE"), 160, 0, fnt);
		} else {
			Gui::DrawString(210, isAcww ? 130 : 140, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("PATTERN_CREATOR_GENDER") + Lang::get("MALE"), 160, 0, fnt);
		}

		Gui::DrawString(210, isAcww ? 150 : 160, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("DESIGNTYPE") + std::to_string(ptrn->designtype()), 395, 0, fnt);

		/* Display Savetype. */
		Gui::DrawString(210, isAcww ? 170 : 180, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("GAME") + getSaveName(ptrn->getType()), 395, 0, fnt);

		/* Display Region, if on AC:WW. */
		if (ptrn->getType() == SaveType::WW) {
			Gui::DrawString(210, 190, 0.7f, C2D_Color32(0, 0, 0, 255), Lang::get("REGION") + getRegionName(ptrn->getRegion()), 395, 0, fnt);
		}
	}

	UI::DrawBase(false, true);
	C3D_FrameEnd(0);
}

static void storageInject(std::unique_ptr<Storage> &storage, std::unique_ptr<Pattern> &ptrn, int slot) {
	if (!ptrn) return;
	u32 size = ptrn->getSize();
	std::unique_ptr<Pattern> res;

	u8* buffer = new u8[size];

	for (int i = 0; i < (int)size; i++) {
		buffer[i] = ptrn->returnData()[i];
	}

	if (ptrn->getType() == SaveType::WW) {
		res = std::make_unique<PatternWW>(buffer, 0, ptrn->getRegion());
		storage->pattern(*res, slot);
		if (buffer) delete[] buffer;
		return;
	} else if (ptrn->getType() == SaveType::NL) {
		res = std::make_unique<PatternNL>(buffer, 0);
		storage->pattern(*res, slot);
		if (buffer) delete[] buffer;
		return;
	} else if (ptrn->getType() == SaveType::WA) {
		res = std::make_unique<PatternWA>(buffer, 0);
		storage->pattern(*res, slot);
		if (buffer) delete[] buffer;
		return;
	}

	if (buffer) delete[] buffer;
}


static void injectToGame(std::unique_ptr<Pattern> &ptrn, std::unique_ptr<Pattern> &ptrn2) {
	/* Some checks before we do anything. */
	SaveType st1 = ptrn->getType();
	SaveType st2 = ptrn2->getType();

	WWRegion r1 = ptrn->getRegion();
	WWRegion r2 = ptrn2->getRegion();

	if (st1 == st2) {
		if (r1 == r2) {

			u8 *buffer = new u8[ptrn->getSize()];
			for (int i = 0; i < (int)ptrn->getSize(); i++) {
				buffer[i] = ptrn->returnData()[i];
			}

			ptrn2->injectData(buffer, ptrn->getSize());

			delete[] buffer;
		}
	}
}


void Overlays::StorageHandling(std::unique_ptr<Storage> &storage, std::unique_ptr<Sav> &savefile) {
	if (!storage) return;
	if (!savefile) return;

	bool displayInfo = false, refreshBank = true, refreshGame = true;
	int box = 0, maxSavePTN = 0, subMode = 0, playerAmount = 0, SelectedPlayer = 0;
	int selection = 0, lastMode = 0;
	bool topSelect = false;
	
	bool grab = false;
	C2D_Image grabImg = {nullptr};
	std::pair<int, bool> grabInf = {-1, true};

	/* Storage | Bank Variables. */
	std::unique_ptr<Pattern> bankPattern[10];
	std::unique_ptr<PatternImage> bankPatternImage[10];
	C2D_Image bankImages[10];
	bool bankHasImage[10] = {false};

	/* Save Variables. */
	std::unique_ptr<Pattern> savePattern[10];
	std::unique_ptr<PatternImage> savePatternImage[10];
	C2D_Image saveImages[10];
	bool saveHasImage[10] = {false};

	bool doOut = false;

	/* Initialize max save pattern. */
	if (savefile->getType() == SaveType::NL || savefile->getType() == SaveType::WA) maxSavePTN = 10;
	else if (savefile->getType() == SaveType::WW) maxSavePTN = 8;
	else if (savefile->getType() == SaveType::UNUSED) return;

	while(!doOut) {
		touchPosition touch;
		u32 hDown = hidKeysDown();
		hidScanInput();
		hidTouchRead(&touch);

		/* Drawing part. */
		if (displayInfo) {
			if (topSelect) DisplayInfo(bankPattern[selection], grabImg);
			else DisplayInfo(savePattern[selection], grabImg);

			/* handling. */
			if (hDown & KEY_B) {
				displayInfo = false;
				C2DUtils::C2D_ImageDelete(grabImg);
				grabImg = {nullptr};

			}
		} else {
			Gui::clearTextBufs();
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
			C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
			UI::DrawBase(true, true);
			Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("STORAGE_BOX") + " " + std::to_string(box + 1), 395, 0, fnt);

			for (int i = 0; i < 10; i++) {
				for (int y = 0; y < 2; y++) {
					for (int x = 0; x < 5; x++, i++) {
						if (bankImages[i].tex && bankHasImage[i]) {
							C2D_DrawImageAt(bankImages[i], 45 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
						}

						if (grab) {
							if (grabInf.second) {
								if (grabInf.first == i) {
									UI::bankSelect(44 + (x * 60), 60 + (y * 80));
								}
							}
						}

						if (topSelect) {
							if (i == selection) UI::DrawSprite(sprites_pattern_border_idx, 44 + (x * 60), 59 + (y * 80));
						}
					}
				}
			}

			UI::DrawBase(false, true);

			if (subMode == 0) {
				/* Sub Menu. */
				UI::DrawSprite(sprites_top_bar_idx, 0, 0);
				Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_CATEGORY"), 395, 0, fnt);
				UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
				Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("X_SELECT"), 395, 0, fnt);
	
				for (int i = 0; i < 3; i++) {
					UI::DrawButton(buttons[i], 0.55);
				}

				UI::DrawSprite(sprites_pointer_idx, buttons[selection].X + 100, buttons[selection].Y + 30);

			} else if (subMode == 1) {
				/* Player Selection. */
				UI::DrawSprite(sprites_top_bar_idx, 0, 0);
				Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_PLAYER"), 395, 0, fnt);
				if (playerAmount == 0) Gui::DrawStringCentered(0, 150, 0.9f, C2D_Color32(0, 0, 0, 255), Lang::get("NO_PLAYERS_FOUND"), 395, 0, fnt);
				UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
				Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("X_SELECT"), 395, 0, fnt);
				if (playerAmount > 0) {
					for (int i = 0; i < 4; i++) {
						if (savefile->PlayerExist(i)) {
							UI::DrawButton(playerPos[i], 0.55);
						}
					}
				}

				UI::DrawSprite(sprites_pointer_idx, playerPos[selection].X + 100, playerPos[selection].Y + 30);

			} else if (subMode == 2 || subMode == 3 || subMode == 4) {
				/* Player Pattern. */
				if (maxSavePTN == 8) {
					for (int i = 0; i < 8; i++) {
						for (u32 y = 0; y < 2; y++) {
							for (u32 x = 0; x < 4; x++, i++) {

								if (saveImages[i].tex && saveHasImage[i]) {
									C2D_DrawImageAt(saveImages[i], 40 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
								}

								if (grab) {
									if (!grabInf.second) {
										if (grabInf.first == i) {
											UI::bankSelect(40 + (x * 60), 60 + (y * 80));
										}
									}
								}
							
								if (!topSelect) {
									if (i == selection) UI::DrawSprite(sprites_pattern_border_idx, 39 + (x * 60), 59 + (y * 80));
								}
							}
						}
					}
				} else if (maxSavePTN == 10) {

					for (int i = 0; i < 10; i++) {
						for (u32 y = 0; y < 2; y++) {
							for (u32 x = 0; x < 5; x++, i++) {

								if (saveImages[i].tex && saveHasImage[i]) {
									C2D_DrawImageAt(saveImages[i], 17 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
								}

								if (grab) {
									if (!grabInf.second) {
										if (grabInf.first == i) {
											UI::bankSelect(17 + (x * 60), 60 + (y * 80));
										}
									}
								}

								if (!topSelect) {
									if (i == selection) UI::DrawSprite(sprites_pattern_border_idx, 16 + (x * 60), 59 + (y * 80));
								}
							}
						}
					}
				} else {
					if (saveImages[0].tex && saveHasImage[0]) {
						C2D_DrawImageAt(saveImages[0], 17, 60, 0.5f, nullptr, 1.5f, 1.5f);
					}

					if (grab) {
						if (!grabInf.second) {
							if (grabInf.first == 0) {
								UI::bankSelect(17, 60);
							}
						}
					}

					if (!topSelect) {
						if (0 == selection) UI::DrawSprite(sprites_pattern_border_idx, 16, 59);
					}
				}
			}

			C3D_FrameEnd(0);

			if (refreshBank) {
				/* Reset here. */
				for (int i = 0 + (box * 10), i2 = 0; i < 10 + (box * 10); i++, i2++) {
					bankPattern[i2] = nullptr;
					bankPatternImage[i2] = nullptr;
					if (bankImages[i2].tex && bankHasImage[i]) C2DUtils::C2D_ImageDelete(bankImages[i2]);

					bankImages[i2] = {nullptr};
					bankHasImage[i2] = false;
				}

				/* Reload here. */
				C3D_FrameEnd(0);
				for (int i = 0 + (box * 10), i2 = 0; i < 10 + (box * 10); i++, i2++) {

					if (storage->pattern(i)) {
						bankPattern[i2] = storage->pattern(i);
						if (bankPattern[i2]) {

							if (bankPattern[i2]) {
								bankPatternImage[i2] = bankPattern[i2]->image(0);
								if (bankPatternImage[i2]) {
									bankImages[i2] = CoreUtils::patternImage(bankPatternImage[i2], bankPattern[i2]->getType());
									bankHasImage[i2] = true;
								}
							}
						}
					}	
				}

				refreshBank = false;
			}

			if (refreshGame) {
				if (savefile)
				/* Reset here. */
				for (int i = 0; i < maxSavePTN; i++) {
					savePattern[i] = nullptr;
					savePatternImage[i] = nullptr;
					if (saveImages[i].tex && saveHasImage[i]) C2DUtils::C2D_ImageDelete(saveImages[i]);

					saveImages[i] = {nullptr};
					saveHasImage[i] = false;
				}

				/* Reload here. */
				C3D_FrameEnd(0);
				for (int i = 0; i < maxSavePTN; i++) {

					if (savefile->playerPattern(0, i)) {
						if (subMode == 2) {
							savePattern[i] = savefile->playerPattern(0, i);
						} else if (subMode == 3) {
							savePattern[i] = savefile->ableSisterPattern(i);
						} else if (subMode == 4) {
							savePattern[0] = savefile->townflag();
						}

						if (savePattern[i]) {

							if (savePattern[i]) {
								savePatternImage[i] = savePattern[i]->image(0);
								if (savePatternImage[i]) {
									saveImages[i] = CoreUtils::patternImage(savePatternImage[i], savePattern[i]->getType());
									saveHasImage[i] = true;
								}
							}
						}
					}	
				}

				refreshGame = false;
			}

			if (subMode == 0) {
				/* Sub Mode. */

				if (hDown & KEY_DOWN) {
					if (selection < 2) selection++;
				}

				if (hDown & KEY_UP) {
					if (selection > 0) selection--;
				}

				if (hDown & KEY_TOUCH) {
					if (touching(touch, buttons[0])) {
						playerAmount = 0;

						for (int i = 0; i < 4; i++) {
							if (savefile->PlayerExist(i)) playerAmount++;
						}

						selection = 0;
						gspWaitForVBlank();
						subMode = 1;

					} else if (touching(touch, buttons[1])) {
						C3D_FrameEnd(0);

						selection = 0;
						gspWaitForVBlank();
						maxSavePTN = 8;
						lastMode = 0;
						refreshGame = true;
						subMode = 3;

					} else if (touching(touch, buttons[2])) {
						C3D_FrameEnd(0);

						selection = 0;
						gspWaitForVBlank();
						maxSavePTN = 1;
						lastMode = 0;
						refreshGame = true;
						subMode = 4;
					}
				}

				if (hDown & KEY_X) {
					switch(selection) {
						case 0:
							playerAmount = 0;

							for (int i = 0; i < 4; i++) {
								if (savefile->PlayerExist(i)) playerAmount++;
							}

							selection = 0;
							gspWaitForVBlank();
							subMode = 1;
							break;

						case 1:
							C3D_FrameEnd(0);
							lastMode = 0;
							selection = 0;
							gspWaitForVBlank();
							maxSavePTN = 8;
							refreshGame = true;
							subMode = 3;
							break;

						case 2:
							C3D_FrameEnd(0);

							selection = 0;
							gspWaitForVBlank();
							maxSavePTN = 1;
							lastMode = 0;
							refreshGame = true;
							subMode = 4;
							break;
					}
				}

			} else if (subMode == 1) {
				/* Player Selection. */
				/* Only do this, if playerAmount larger than 0. */
				if (playerAmount > 0) {
					if (hDown & KEY_RIGHT) {
						if (SelectedPlayer < 3) {
							if (savefile->PlayerExist(SelectedPlayer + 1)) {
								SelectedPlayer++;
							}
						}
					}

					if (hDown & KEY_TOUCH) {
						for (int i = 0; i < 4; i++) {
							if (touching(touch, playerPos[i])) {
								if (savefile->PlayerExist(i)) {
									SelectedPlayer = i;
									/* Push back player pattern. */
									C3D_FrameEnd(0);

									if (savefile->getType() == SaveType::WW) {
										maxSavePTN = 8;
										refreshGame = true;
									} else if (savefile->getType() == SaveType::NL || savefile->getType() == SaveType::WA) {
										maxSavePTN = 10;
										refreshGame = true;
									}

									gspWaitForVBlank();
									subMode = 2;
									break;
								}
							}
						}
					}

					if (hDown & KEY_LEFT) {
						if (SelectedPlayer > 0) {
							SelectedPlayer--;
						}
					}

					if (hDown & KEY_X) {
						if (savefile->PlayerExist(SelectedPlayer)) {
							/* Push back player pattern. */
							C3D_FrameEnd(0);

							if (savefile->getType() == SaveType::WW) {
								maxSavePTN = 8;
								refreshGame = true;
							} else if (savefile->getType() == SaveType::NL || savefile->getType() == SaveType::WA) {
								maxSavePTN = 10;
								refreshGame = true;
							}

							gspWaitForVBlank();
							lastMode = 1;
							subMode = 2;
						}
					}
				}

				if (hDown & KEY_B) {
					gspWaitForVBlank();
					selection = 0;
					subMode = 0;
				}
			} else if (subMode == 2 || subMode == 3 || subMode == 4) {

				if (hDown & KEY_X) {
					if (topSelect) {
						if (bankPattern[selection]) {
							grabImg = CoreUtils::patternImage(bankPatternImage[selection], bankPattern[selection]->getType());
							displayInfo = true;
						}
					} else {
						if (savePattern[selection]) {
							grabImg = CoreUtils::patternImage(savePatternImage[selection], savePattern[selection]->getType());
							displayInfo = true;
						}
					}
				}

				if (hDown & KEY_A) {
					if (!grab) {
						/* Check if valid too. */
						if (topSelect) {
							if (bankPattern[selection]) {
								grabInf = {selection, topSelect};
								grab = true;
							}
						} else {
							if (savePattern[selection]) {
								grabInf = {selection, topSelect};
								grab = true;
							}
						}

					} else {
						if (topSelect) {
							storageInject(storage, grabInf.second ? bankPattern[grabInf.first] : savePattern[grabInf.first], selection);
							refreshBank = true;
						} else {
							injectToGame(grabInf.second ? bankPattern[grabInf.first] : savePattern[grabInf.first], savePattern[selection]);
							refreshGame = true;
						}

						grabInf = {-1, true};
						grab = false;
					}
				}

				/* Navigation. */
				if (hDown & KEY_RIGHT) {
					if (topSelect) {
						if (selection < 9) selection++;
					} else {
						if (subMode != 4) {
							if (selection < maxSavePTN - 1) selection++;
						}
					}
				}

				if (hDown & KEY_LEFT) {
					if (selection > 0) selection--;
				}

				if (hDown & KEY_UP) {
					if (topSelect) {
						if (selection > 4) selection -= 5;
					} else {
						if (maxSavePTN == 8) {
							if (selection > 3) selection -= 4;
							else if (selection < 5) {
								topSelect = true;
								if (selection == 0) selection = 5;
								else selection = selection + 5;
							}
						} else if (maxSavePTN == 10) {
							if (selection > 4) selection -= 5;
							else if (selection < 5) {
								topSelect = true;
								selection = selection + 5;
							}
						} else if (maxSavePTN == 1) {
							topSelect = true;
							selection = 5;
						}
					}
				}

				if (hDown & KEY_DOWN) {
					if (topSelect) {
						if (selection < 5) selection += 5;
						else if (selection > 4) {
							topSelect = false;
							if (maxSavePTN == 8) {
								if (selection == 5) selection = 0;
								else selection = selection - 4;
							} else if (maxSavePTN == 10) {
								selection = selection - 5;
							} else if (maxSavePTN == 1) {
								selection = 0;
							}
						}
					} else {
						if (maxSavePTN == 8) {
							if (selection < 4) selection += 4;
						} else if (maxSavePTN == 10) {
							if (selection < 5) selection += 5;
						}
					}
				}

				if (hDown & KEY_B) {
					selection = 0;
					topSelect = false;

					subMode = lastMode;
				}
			}

			if (hDown & KEY_START) doOut = true;
		}
	}
}