/*
*   This file is part of LeafEdit-Core
*   Copyright (C) 2020 Universal-Team
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

#include "checksum.hpp"
#include "saveUtils.hpp"
#include "SavWA.hpp"

/* Return if player exist. */
bool SavWA::PlayerExist(int player) const {
	if (player > 3) return false;

	return SaveUtils::Read<u16>(this->savePointer(), (0xA0 + (player * 0xA480)) + 0x55A6) != 0;
}

/* Get Player Pattern. */
std::unique_ptr<Pattern> SavWA::playerPattern(int player, int pattern) const {
	if (player > 3 || pattern > 9) return nullptr;

	u32 playerOffset = 0xA0 + (player * 0xA480);

	/* Check, if Player exist. */
	if (this->PlayerExist(player)) {
		return std::make_unique<PatternWA>(this->dataPointer.get(), playerOffset + 0x2C + pattern * 0x870);
	}

	return nullptr;
}

/* Get Able Sister Pattern. */
std::unique_ptr<Pattern> SavWA::ableSisterPattern(int pattern) const {
	if (pattern > 7) return nullptr;
	
	return std::make_unique<PatternWA>(this->dataPointer.get(), 0x62338 + pattern * 0x870);
}

/* Get TownFlag Pattern. */
std::unique_ptr<Pattern> SavWA::townflag() const {
	return std::make_unique<PatternWA>(this->dataPointer.get(), 0x70F1C);
}

// Last call before writing to file. Update Checksum.
void SavWA::Finish(void) {
	Checksum::FixCRC32s(this->savePointer());
}