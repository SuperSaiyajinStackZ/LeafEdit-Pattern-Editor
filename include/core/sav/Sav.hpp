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

#ifndef _LEAFEDIT_CORE_SAV_HPP
#define _LEAFEDIT_CORE_SAV_HPP

#include "Pattern.hpp"
#include "types.hpp"

#include <cstring>
#include <memory>
#include <string>
#include <vector>

class Pattern;
class Sav {
protected:
	// Protected stuff.
	std::shared_ptr<u8[]> saveData;
	u32 saveLength;
	std::string filelocation;
public:
	// Constructor, Destructor and stuff.
	virtual ~Sav() {}
	Sav(std::shared_ptr<u8[]> data, u32 length, std::string Location) : saveData(data), saveLength(length), filelocation(Location) {
		this->changesMade(false); // Initialize as false here.
	}

	Sav(const Sav& save) = delete;
	Sav& operator=(const Sav& save) = delete;
	
	// Call this when finished editing.
	virtual void Finish(void) = 0;

	/* Pattern. */
	virtual bool PlayerExist(int player) const = 0;
	virtual std::unique_ptr<Pattern> playerPattern(int player, int pattern) const = 0;
	virtual int getPlayerAmount() const = 0;
	virtual std::unique_ptr<Pattern> ableSisterPattern(int pattern) const = 0;
	virtual int getAbleSisterAmount() const = 0;
	virtual std::unique_ptr<Pattern> townflag() const = 0;

	// Call this when getting the SaveType.
	static std::unique_ptr<Sav> getSave(std::shared_ptr<u8[]> dt, u32 length, std::string Loc);
	static std::unique_ptr<Sav> check080000(std::shared_ptr<u8[]> dt, u32 length, std::string Loc);

	// return Sav stuff.
	u32 getLength() const { return saveLength; }
	std::shared_ptr<u8[]> rawData() const { return saveData; }
	std::string getFileLocation() const { return this->filelocation; }

	// Pass game | version.
	const u8 version() {
		switch(this->getType()) {
			case SaveType::WW:		return 1; // AC:WW.
			case SaveType::NL:		return 2; // AC:NL.
			case SaveType::WA:		return 3; // AC:WA.
			case SaveType::UNUSED:	return 0; // Not Valid / Unused.
		}

		return 0; // Should not happen actually.
	}

	// Pass Region. -> Only needed for AC:WW.
	const u8 region() {
		switch(this->getRegion()) {
			case WWRegion::USA_REV0:	return 1; // USA Revision 0.
			case WWRegion::USA_REV1:	return 2; // USA Revision 1.
			case WWRegion::EUR_REV1:	return 3; // EUR Revision 1.
			case WWRegion::JPN_REV0:	return 4; // JPN Revision 0.
			case WWRegion::JPN_REV1:	return 5; // JPN Revision 1.
			case WWRegion::KOR_REV1:	return 6; // KOR Revision 1.
			case WWRegion::UNKNOWN:		return 0; // Unknown.
		}

		return 0; // Should not happen actually.
	}

	virtual SaveType getType() const = 0;
	virtual WWRegion getRegion() const = 0;
	bool changes = false;

	void changesMade(bool v) { if (v != this->changes) this->changes = v; }
	bool changesMade() const { return this->changes; }

	u8 *savePointer() const {
		return this->saveData.get();
	}
};

#endif