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

#include "inifile.hpp"
#include "settings.hpp"


std::string Settings::DefaultPath, Settings::DefaultCreatorName, Settings::DefaultTownName;
u16 Settings::DefaultCreatorID, Settings::DefaultTownID;
u8 Settings::DefaultGender;

void Settings::Read() {
	CIniFile settingsfile("sdmc:/3ds/LeafEdit/Pattern-Editor/Settings.ini");
	
	Settings::DefaultPath = settingsfile.GetString("Defaults", "defaultPattern ", "romfs:/pattern/empty/nl.acnl");
	Settings::DefaultCreatorName = settingsfile.GetString("Defaults", "defaultCreatorName ", "Unknown");
	Settings::DefaultTownName = settingsfile.GetString("Defaults", "defaultTownName ", "Unknown");

	Settings::DefaultCreatorID = (u16)settingsfile.GetInt("Defaults", "defaultCreatorID ", 0);
	Settings::DefaultTownID = (u16)settingsfile.GetInt("Defaults", "defaultTownID ", 0);

	Settings::DefaultGender = (u8)settingsfile.GetInt("Defaults", "defaultGender ", 0);
}

void Settings::Save() {
	CIniFile settingsfile("sdmc:/3ds/LeafEdit/Pattern-Editor/Settings.ini");

	settingsfile.SetString("Defaults", "defaultPattern ", Settings::DefaultPath);
	settingsfile.SetString("Defaults", "defaultCreatorName ", Settings::DefaultCreatorName);
	settingsfile.SetString("Defaults", "defaultTownName ", Settings::DefaultTownName);

	settingsfile.SetInt("Defaults", "defaultCreatorID ", (int)Settings::DefaultCreatorID);
	settingsfile.SetInt("Defaults", "defaultTownID ", (int)Settings::DefaultTownID);

	settingsfile.SetInt("Defaults", "defaultGender ", (int)Settings::DefaultGender);

	settingsfile.SaveIniFileModified("sdmc:/3ds/LeafEdit/Pattern-Editor/Settings.ini");
}