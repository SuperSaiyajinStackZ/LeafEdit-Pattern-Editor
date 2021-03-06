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

#ifndef _LEAFEDIT_CORE_PATTERN_HPP
#define _LEAFEDIT_CORE_PATTERN_HPP

#include "PatternImage.hpp"
#include "types.hpp"

#include <memory>

class PatternImage;
class Pattern {
protected:
	u8 *data;
	u32 Offset;
public:
	virtual ~Pattern() {}
	Pattern(u8 *dt, u32 offset) : data(dt), Offset(offset) {}
	Pattern(const Pattern& pattern) = delete;
	Pattern& operator=(const Pattern& pattern) = delete;

	virtual std::u16string name() const = 0;
	virtual void name(std::u16string v) = 0;
	virtual u16 creatorid() const = 0;
	virtual void creatorid(u16 v) = 0;
	virtual std::u16string creatorname() const = 0;
	virtual void creatorname(std::u16string v) = 0;
	virtual u8 creatorGender() const = 0;
	virtual void creatorGender(u8 v) = 0;
	virtual u16 origtownid() const = 0;
	virtual void origtownid(u16 v) = 0;
	virtual std::u16string origtownname() const = 0;
	virtual void origtownname(std::u16string v) = 0;
	virtual u8 designtype() const = 0;
	virtual void designtype(u8 v) = 0;

	/* Pattern Misc. */
	virtual void dumpPattern(const std::string fileName) = 0;
	virtual void injectData(u8 *buffer, u32 size) = 0;
	virtual void injectPattern(const std::string fileName) = 0;

	virtual u32 getSize() const = 0;

	const u8 *returnData() const {
		return this->data + this->Offset;
	}

	virtual WWRegion getRegion() const = 0;
	virtual SaveType getType() const = 0;

	/* Pattern Image. */
	virtual std::unique_ptr<PatternImage> image(const int pattern) const = 0;
};

#endif