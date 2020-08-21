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

#include "json.hpp"
#include "settings.hpp"
#include <unistd.h>

static nlohmann::json config;
static bool configChanged = false;

/* Return Language. */
int Settings::getLang() {
	if (config.contains("Language")) return config["Language"];
	else return 1; // English.
}

/* Set Language. */
void Settings::setLang(int lang) {
	config["Language"] = lang;
	if (!configChanged) configChanged = true;
}

/* Get Default Pattern. */
std::string Settings::getDefaultPath() {
	if (config.contains("Default_Pattern")) return config["Default_Pattern"];
	else return "romfs:/pattern/empty/nl.acnl"; // Default pattern.
}

/* Set Default Pattern. */
void Settings::setDefaultPath(std::string path) {
	config["Default_Pattern"] = path;
	if (!configChanged) configChanged = true;
}

/* Read Config. */
void Settings::Read() {
	if (access("sdmc:/3ds/LeafEdit/Pattern-Editor/Settings.json", F_OK) != 0) {
		FILE *file = fopen("sdmc:/3ds/LeafEdit/Pattern-Editor/Settings.json", "w");

		config["Language"] = 1;
		config["Default_Pattern"] = "romfs:/pattern/empty/nl.acnl";

		const std::string dump = config.dump(1, '\t');
		fwrite(dump.c_str(), 1, config.dump(1, '\t').size(), file);
		fclose(file); // Now we have the file and can properly access it.
	}

	FILE* file = fopen("sdmc:/3ds/LeafEdit/Pattern-Editor/Settings.json", "r");
	config = nlohmann::json::parse(file, nullptr, false);
	fclose(file);
}

/* Save Config. */
void Settings::Save() {
	if (configChanged) {
		FILE *file = fopen("sdmc:/3ds/LeafEdit/Pattern-Editor/Settings.json", "wb");
		const std::string dump = config.dump(1, '\t');
		fwrite(dump.c_str(), 1, config.dump(1, '\t').size(), file);
		fclose(file);
	}
}