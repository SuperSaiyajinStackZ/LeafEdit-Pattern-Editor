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

#ifndef _LEAFEDIT_CORE_PATTERN_WA_HPP
#define _LEAFEDIT_CORE_PATTERN_WA_HPP

#include "Pattern.hpp"
#include "PatternImage.hpp"
#include "PatternImageNL.hpp"

#include <memory>

class PatternImage;
class PatternImageNL;
class PatternWA : public Pattern {
protected:
	u32 Offset;
	u8 *data;
public:
	virtual ~PatternWA() {}
	PatternWA(u8 *patternData, u32 offset) : Pattern(patternData, offset), Offset(offset), data(patternData) { }

	std::u16string name() const override;
	void name(std::u16string v) override;
	u16 creatorid() const override;
	void creatorid(u16 v) override;
	std::u16string creatorname() const override;
	void creatorname(std::u16string v) override;
	u8 creatorGender() const override;
	void creatorGender(u8 v) override;
	u16 origtownid() const override;
	void origtownid(u16 v) override;
	std::u16string origtownname() const override;
	void origtownname(std::u16string v) override;
	u8 designtype() const override;
	void designtype(u8 v) override;

	u32 getSize() const override {
		if (this->patternPointer()[0x69] == 0x09) {
			return 620;
		} else {
			return 2160;
		}
	}

	WWRegion getRegion() const override { return WWRegion::UNKNOWN; };
	SaveType getType() const override { return SaveType::WA; };

	/* Pattern Misc. */
	void dumpPattern(const std::string fileName) override;
	void injectData(u8 *buffer, u32 size) override;
	void injectPattern(const std::string fileName) override;

	/* Pattern Image. */
	std::unique_ptr<PatternImage> image(const int pattern) const override;
private:
	u8* patternPointer() const {
		return data + Offset;
	}
};

#endif