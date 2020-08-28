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

#ifndef _LEAFEDIT_CORE_SAV_NL_HPP
#define _LEAFEDIT_CORE_SAV_NL_HPP

#include "Pattern.hpp"
#include "PatternNL.hpp"
#include "Sav.hpp"
#include "types.hpp"

#include <string>

class Pattern;
class PatternNL;
class SavNL : public Sav {
protected:
	std::shared_ptr<u8[]> dataPointer;
	u32 saveSize;
public:
	SavNL(std::shared_ptr<u8[]> dt, u32 ssize, std::string Loc) : Sav(dt, ssize, Loc), dataPointer(dt), saveSize(ssize) { }
	virtual ~SavNL() {}
	void Finish(void) override;

	bool PlayerExist(int player) const override;
	
	/* Pattern. */
	std::unique_ptr<Pattern> playerPattern(int player, int pattern) const override;
	int getPlayerAmount() const override { return 10; }
	std::unique_ptr<Pattern> ableSisterPattern(int pattern) const override;
	int getAbleSisterAmount() const override { return 8; }
	std::unique_ptr<Pattern> townflag() const override;

	std::unique_ptr<Pattern> HHDPattern(u32 slot) const override { return nullptr; };

	SaveType getType() const override { return SaveType::NL; }
	WWRegion getRegion() const override { return WWRegion::UNKNOWN; }
private:
	u8 *savePointer() const {
		return dataPointer.get();
	}
};

#endif