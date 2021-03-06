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

#ifndef _LEAFEDIT_PATTERN_EDITOR_CORE_UTILS_HPP
#define _LEAFEDIT_PATTERN_EDITOR_CORE_UTILS_HPP

#include "common.hpp"
#include "Pattern.hpp"
#include "PatternImage.hpp"
#include "types.hpp"
#include <citro2d.h>

struct PatternInformations {
	std::u16string PatternName;
	std::u16string CreatorName;
	std::u16string TownName;
	u16 CreatorID;
	u16 TownID;
	u8 CreatorGender;
};

namespace CoreUtils {
	void generateEmptyPattern(SaveType ST, WWRegion region, std::unique_ptr<u8[]> &data, u32 &size);
	C2D_Image savePatternImage(std::unique_ptr<PatternImage> image, SaveType ST = SaveType::UNUSED);
	C2D_Image patternImage(std::unique_ptr<PatternImage> &image, SaveType ST = SaveType::UNUSED);
	std::string getDefaultFile(SaveType ST, WWRegion region);
	PatternInformations getDefaultInformation(SaveType ST, WWRegion region);
	void dumpPatternInformation(SaveType ST, WWRegion region, std::unique_ptr<Pattern> &ptrn);
}

#endif