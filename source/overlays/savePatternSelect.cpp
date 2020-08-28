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
#include "structs.hpp"

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

/* 15 Pattern each page. */
static const std::vector<Structs::ButtonPos> Pattern15 = {
	{17, 40, 48, 48},
	{17, 100, 48, 48},
	{17, 160, 48, 48},

	{77, 40, 48, 48},
	{77, 100, 48, 48},
	{77, 160, 48, 48},

	{137, 40, 48, 48},
	{137, 100, 48, 48},
	{137, 160, 48, 48},

	{197, 40, 48, 48},
	{197, 100, 48, 48},
	{197, 160, 48, 48},

	{257, 40, 48, 48},	
	{257, 100, 48, 48},
	{257, 160, 48, 48}
};

/* If button Position pressed -> Do something. */
static bool touching(touchPosition touch, Button button) {
	if (touch.px >= button.X && touch.px <= (button.X + button.XSize) && touch.py >= button.Y && touch.py <= (button.Y + button.YSize)) return true;
	else return false;
}

static bool touchPattern(touchPosition touch, Structs::ButtonPos ptrn) {
	if (touch.px >= ptrn.x && touch.px <= (ptrn.x + ptrn.w) && touch.py >= ptrn.y && touch.py <= (ptrn.y + ptrn.h)) return true;
	else return false;
}

static void DrawHHDSelection(std::vector<C2D_Image> &images, int selection, int page) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_A_PATTERN"), 395, 0, fnt);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("X_SELECT"), 395, 0, fnt);
	UI::DrawBase(false, true);

	for (int i = 0; i < 15; i++) {
		if (images[i].subtex) {
			C2D_DrawImageAt(images[i], Pattern15[i].x, Pattern15[i].y, 0.5f, nullptr, 1.5f, 1.5f);
		}
	}

	UI::DrawSprite(sprites_pointer_idx, Pattern15[selection].x+10, Pattern15[selection].y+10);
	
	C3D_FrameEnd(0);
}

static void CategorySelect(int selection) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_CATEGORY"), 395, 0, fnt);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("X_SELECT"), 395, 0, fnt);
	UI::DrawBase(false, true);
	
	for (int i = 0; i < 3; i++) {
		UI::DrawButton(buttons[i], 0.55);
	}

	UI::DrawSprite(sprites_pointer_idx, buttons[selection].X + 100, buttons[selection].Y + 30);

	C3D_FrameEnd(0);
}

static void PlayerSelect(int selection, std::unique_ptr<Sav> &savefile, int playerAmount) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_PLAYER"), 395, 0, fnt);
	if (playerAmount == 0) Gui::DrawStringCentered(0, 150, 0.9f, C2D_Color32(0, 0, 0, 255), Lang::get("NO_PLAYERS_FOUND"), 395, 0, fnt);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("X_SELECT"), 395, 0, fnt);
	UI::DrawBase(false, true);
	
	if (savefile || playerAmount > 0) {
		for (int i = 0; i < 4; i++) {
			if (savefile->PlayerExist(i)) {
				UI::DrawButton(playerPos[i], 0.55);
			}
		}

		UI::DrawSprite(sprites_pointer_idx, playerPos[selection].X + 100, playerPos[selection].Y + 30);
	}

	C3D_FrameEnd(0);
}

static void SelectPlayerPattern(std::vector<C2D_Image> &images, int selection, int maxP) {
	int selectX = 0, selectY = 0;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_A_PATTERN"), 395, 0, fnt);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("X_SELECT"), 395, 0, fnt);
	UI::DrawBase(false, true);

	/* 8 --> AC:WW. */
	if (maxP == 8) {
		for (int i = 0; i < 8; i++) {
			for (u32 y = 0; y < 2; y++) {
				for (u32 x = 0; x < 4; x++, i++) {
					if (images[i].subtex) {
						C2D_DrawImageAt(images[i], 40 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
					}
				}
			}
		}

		if (selection < 4)	selectY = 0;	else	selectY = 1;
		if (selection > 3)	selectX = selection - 4;	else selectX = selection;

		UI::DrawSprite(sprites_pointer_idx, 45 + (selectX * 60), 67 + (selectY * 80));

	/* 10 --> AC:NL | AC:WA. */
	} else {
		for (int i = 0; i < 10; i++) {
			for (u32 y = 0; y < 2; y++) {
				for (u32 x = 0; x < 5; x++, i++) {
					if (images[i].subtex) {
						C2D_DrawImageAt(images[i], 17 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
					}
				}
			}
		}

		if (selection < 5)	selectY = 0;	else	selectY = 1;
		if (selection > 4)	selectX = selection - 5;	else selectX = selection;

		UI::DrawSprite(sprites_pointer_idx, 24 + (selectX * 60), 67 + (selectY * 80));
	}
	
	C3D_FrameEnd(0);
}

static void SelectAbleSisterPattern(std::vector<C2D_Image> &images, int selection) {
	int selectX = 0, selectY = 0;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	UI::DrawBase(true, true);
	Gui::DrawStringCentered(0, -2, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("SELECT_A_PATTERN"), 395, 0, fnt);
	UI::DrawSprite(sprites_bottom_bar_idx, 0, 209);
	Gui::DrawStringCentered(0, 218, 0.9f, C2D_Color32(255, 255, 255, 255), Lang::get("X_SELECT"), 395, 0, fnt);
	UI::DrawBase(false, true);

	for (int i = 0; i < 8; i++) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 4; x++, i++) {
				if (images[i].subtex) {
					C2D_DrawImageAt(images[i], 40 + (x * 60), 60 + (y * 80), 0.5f, nullptr, 1.5f, 1.5f);
				}
			}
		}
	}

	if (selection < 4)	selectY = 0;	else	selectY = 1;
	if (selection > 3)	selectX = selection - 4;	else selectX = selection;

	UI::DrawSprite(sprites_pointer_idx, 45 + (selectX * 60), 67 + (selectY * 80));
	
	C3D_FrameEnd(0);
}


bool Overlays::SelectSavePattern(std::unique_ptr<Sav> &savefile, std::unique_ptr<Pattern> &ptrn) {
	if (!savefile) return false;
	std::vector<C2D_Image> images;
	touchPosition touch;
	int category = 0, selection = 0, SelectedPlayer = 0, playerAmount = 0, page = 0;
	bool generatePattern = false;

	if (savefile->getType() == SaveType::HHD) {
		/* Generate Pattern for HHD. */
		C3D_FrameEnd(0);
		images.clear();

		/* Push Images. */
		for (int i = 0; i < 15; i++) {
			images.push_back({CoreUtils::savePatternImage(savefile->HHDPattern(i)->image(0), SaveType::WA)});
		}

		category = 4;
	}


	gspWaitForVBlank();

	while(1) {
		if (generatePattern) {
			if (savefile->getType() == SaveType::HHD) {
				/* Generate Pattern for HHD. */
				C3D_FrameEnd(0);
				images.clear();

				/* Push Images. */
				for (int i = 0 + (page * 15); i < 15 + (page * 15); i++) {
					images.push_back({CoreUtils::savePatternImage(savefile->HHDPattern(i)->image(0), SaveType::WA)});
				}
			}

			generatePattern = false;
		}


		switch(category) {
			case 0:
				CategorySelect(selection);
				break;
			case 1:
				PlayerSelect(selection, savefile, playerAmount);
				break;
			case 2:
				SelectPlayerPattern(images, selection, savefile->getPlayerAmount());
				break;
			case 3:
				SelectAbleSisterPattern(images, selection);
				break;
			case 4:
				DrawHHDSelection(images, selection, page);
				break;
		}

		u32 hDown = hidKeysDown();
		hidScanInput();
		hidTouchRead(&touch);

		/* Sub Menu. */
		if (category == 0) {
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
					category = 1;

				} else if (touching(touch, buttons[1])) {
					C3D_FrameEnd(0);
					images.clear();

					/* Push Images. */
					for (int i = 0; i < 8; i++) {
						images.push_back({CoreUtils::savePatternImage(savefile->ableSisterPattern(i)->image(0), savefile->getType())});
					}

					selection = 0;
					gspWaitForVBlank();
					category = 3;

				} else if (touching(touch, buttons[2])) {
					if (savefile->townflag()) {
						ptrn = savefile->townflag(); /* Return Townflag. */
						return true;
					}

					gspWaitForVBlank();
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
						category = 1;
						break;

					case 1:
						C3D_FrameEnd(0);
						images.clear();

						/* Push Images. */
						for (int i = 0; i < 8; i++) {
							images.push_back({CoreUtils::savePatternImage(savefile->ableSisterPattern(i)->image(0), savefile->getType())});
						}

						selection = 0;
						gspWaitForVBlank();
						category = 3;
						break;

					case 2:
						if (savefile->townflag()) {
							ptrn = savefile->townflag(); /* Return Townflag. */
							return true;
						}

						gspWaitForVBlank();
						break;
				}

				if (hDown & KEY_B) return false;

			}

		/* Player Selection. */
		} else if (category == 1) {
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

								/* Push Images. */
								for (int x = 0; x < savefile->getPlayerAmount(); x++) {
									images.push_back({CoreUtils::savePatternImage(savefile->playerPattern(SelectedPlayer, x)->image(0), savefile->getType())});
								}

								gspWaitForVBlank();
								category = 2;
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

						/* Push Images. */
						for (int i = 0; i < savefile->getPlayerAmount(); i++) {
							images.push_back({CoreUtils::savePatternImage(savefile->playerPattern(SelectedPlayer, i)->image(0), savefile->getType())});
						}

						gspWaitForVBlank();
						category = 2;
					}
				}
			}

			if (hDown & KEY_B) {
				gspWaitForVBlank();
				selection = 0;
				category = 0;
			}

		/* Player Pattern Selection. */
		} else if (category == 2) {
			if (hDown & KEY_RIGHT) {
				if (selection < savefile->getPlayerAmount()-1) {
					selection++;
				}
			}

			if (hDown & KEY_LEFT) {
				if (selection > 0) {
					selection--;
				}
			}

			if (hDown & KEY_X) {
				gspWaitForVBlank();
				if (savefile->playerPattern(SelectedPlayer, selection)) {
					ptrn = savefile->playerPattern(SelectedPlayer, selection); /* Return player pattern. */
					return true;
				}
			}

			if (hDown & KEY_TOUCH) {
				if (savefile->getType() == SaveType::WW) {
					for (int i = 0; i < savefile->getPlayerAmount(); i++) {
						if (touchPattern(touch, Pattern8[i])) {
							gspWaitForVBlank();
							if (savefile->playerPattern(SelectedPlayer, i)) {
								ptrn = savefile->playerPattern(SelectedPlayer, i); /* Return player pattern. */
								return true;
							}
						}
					}
				} else {
					for (int i = 0; i < savefile->getPlayerAmount(); i++) {
						if (touchPattern(touch, Pattern10[i])) {
							gspWaitForVBlank();
							if (savefile->playerPattern(SelectedPlayer, i)) {
								ptrn = savefile->playerPattern(SelectedPlayer, i); /* Return player pattern. */
								return true;
							}
						}
					}
				}
			}

			if (hDown & KEY_B) {
				C3D_FrameEnd(0);
				selection = 0;
				images.clear();
				gspWaitForVBlank();
				category = 1;
			}

		/* Able Sister Pattern Selection. */
		} else if (category == 3) {
			if (hDown & KEY_RIGHT) {
				if (selection < 7) {
					selection++;
				}
			}

			if (hDown & KEY_LEFT) {
				if (selection > 0) {
					selection--;
				}
			}

			if (hDown & KEY_TOUCH) {
				for (int i = 0; i < savefile->getPlayerAmount(); i++) {
					if (touchPattern(touch, Pattern8[i])) {
						gspWaitForVBlank();
						if (savefile->ableSisterPattern(i)) {
							ptrn = savefile->ableSisterPattern(i); /* Return able sister pattern. */
							return true;
						}
					}
				}
			}

			if (hDown & KEY_X) {
				gspWaitForVBlank();
				if (savefile->ableSisterPattern(selection)) {
					ptrn = savefile->ableSisterPattern(selection); /* Return able sister pattern. */
					return true;
				}
			}

			if (hDown & KEY_B) {
				C3D_FrameEnd(0);
				selection = 0;
				images.clear();
				gspWaitForVBlank();
				category = 0;
			}
		} else if (category == 4) {
			
			/* HHD. */
			if (hDown & KEY_R) {
				if (page < 7) {
					page++;
					generatePattern = true;
				}
			}

			if (hDown & KEY_L) {
				if (page > 0) {
					page--;
					generatePattern = true;
				}
			}

			if (hDown & KEY_RIGHT) {
				if (selection < 12) selection += 3;
			}

			if (hDown & KEY_LEFT) {
				if (selection > 2) selection -= 3;
			}

			if (hDown & KEY_DOWN) {
				if (selection < 14) selection++;
			}

			if (hDown & KEY_UP) {
				if (selection > 0) selection--;
			}

			if (hDown & KEY_X) {
				if (savefile->HHDPattern((15 * page) + selection)) {
					ptrn = savefile->HHDPattern((15 * page) + selection); /* Return HHD pattern. */
					return true;
				}
			}

			if (hDown & KEY_TOUCH) {
				for (int i = 0; i < 15; i++) {
					if (touchPattern(touch, Pattern15[i])) {
						gspWaitForVBlank();
						if (savefile->HHDPattern((15 * page) + i)) {
							ptrn = savefile->HHDPattern((15 * page) + i); /* Return HHD pattern. */
							return true;
						}
					}
				}
			}

			if (hDown & KEY_B) {
				return false;
			}
		}
	}
}