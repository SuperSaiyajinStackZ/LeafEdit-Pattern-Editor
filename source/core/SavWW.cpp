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
#include "SavWW.hpp"

#include <cstring>

/* Return if player exist. */
bool SavWW::PlayerExist(int player) const {
	if (player > 3) return false;

	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return SaveUtils::Read<u16>(this->savePointer(), (0x000C + (player * 0x228C)) + 0x2280) != 0;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return SaveUtils::Read<u16>(this->savePointer(), (0x000C + (player * 0x1D10)) + 0x1D04) != 0;
		case WWRegion::KOR_REV1:
			return SaveUtils::Read<u16>(this->savePointer(), (0x0014 + (player * 0x249C)) + 0x248C) != 0;
		case WWRegion::UNKNOWN:
			return false;
	}

	return false;
}

/* Get Player Pattern. */
std::unique_ptr<Pattern> SavWW::playerPattern(int player, int pattern) const {
	if (player > 3 || pattern > 9) return nullptr;

	u32 offset = 0; /* Offset to read. */

	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			offset = (0x000C + (player * 0x228C) + pattern * 0x228);
			break;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			offset = (0x000C + (player * 0x1D10) + pattern * 0x220);
			break;
		case WWRegion::KOR_REV1:
			offset = (0x0014 + (player * 0x249C) + pattern * 0x234);
			break;
		case WWRegion::UNKNOWN:
			return nullptr;
	}

	/* Check, if Player exist. */
	if (this->PlayerExist(player)) {
		return std::make_unique<PatternWW>(this->dataPointer, offset, this->region);
	}

	return nullptr;
}

/* Get Able Sister Pattern. */
std::unique_ptr<Pattern> SavWW::ableSisterPattern(int pattern) const {
	if (pattern > 7) return nullptr;

	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return std::make_unique<PatternWW>(this->dataPointer, 0xFAFC + pattern * 0x228, this->region);
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return std::make_unique<PatternWW>(this->dataPointer, 0xDAF8 + pattern * 0x220, this->region);
		case WWRegion::KOR_REV1:
			return std::make_unique<PatternWW>(this->dataPointer, 0x10AD0 + pattern * 0x234, this->region);
		case WWRegion::UNKNOWN:
			return nullptr;
	}

	return nullptr;
}

/* Get TownFlag Pattern. */
std::unique_ptr<Pattern> SavWW::townflag() const {
	switch(this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return std::make_unique<PatternWW>(this->dataPointer, 0x15930, this->region);
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return std::make_unique<PatternWW>(this->dataPointer, 0x11C5C, this->region);
		case WWRegion::KOR_REV1:
			return std::make_unique<PatternWW>(this->dataPointer, 0x16D0C, this->region);
		case WWRegion::UNKNOWN:
			return nullptr;
	}

	return nullptr;
}

// Last call before writing to file. Update Checksum.
void SavWW::Finish(void) {
	switch (this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Checksum::UpdateWWChecksum(this->region, this->savePointer(), reinterpret_cast<u16*>(this->savePointer()), 0x15FE0 / sizeof(u16));
			memcpy(this->savePointer() + 0x15FE0, this->savePointer(), 0x15FE0); // Copy SaveData to the second save copy.
			break;
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Checksum::UpdateWWChecksum(this->region, this->savePointer(), reinterpret_cast<u16*>(this->savePointer()), 0x12224 / sizeof(u16));
			memcpy(this->savePointer() + 0x12224, this->savePointer(), 0x12224); // Copy SaveData to the second save copy.
			break;
		case WWRegion::KOR_REV1:
			Checksum::UpdateWWChecksum(this->region, this->savePointer(), reinterpret_cast<u16*>(this->savePointer()), 0x173FC / sizeof(u16));
			memcpy(this->savePointer() + 0x173FC, this->savePointer(), 0x173FC); // Copy SaveData to the second save copy.
			break;
		case WWRegion::UNKNOWN:
			break;
	}
}