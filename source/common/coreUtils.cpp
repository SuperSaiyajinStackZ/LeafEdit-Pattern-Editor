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
#include "saveUtils.hpp"
#include "stringUtils.hpp"
#include "utils.hpp"
#include <unistd.h>

/* Generate an empty pattern using the empty pattern files of the romfs. */
void CoreUtils::generateEmptyPattern(SaveType ST, WWRegion region, std::unique_ptr<u8[]> &data, u32 &size) {
	data = nullptr; // Reset here.
	std::string path;

	switch(ST) {
		case SaveType::WW:
			/* Switch Region. */
			switch(region) {
				case WWRegion::JPN_REV0:
				case WWRegion::JPN_REV1:
					path = "romfs:/pattern/empty/ww_jpn.acww";
					break;
				case WWRegion::USA_REV0:
				case WWRegion::USA_REV1:
				case WWRegion::EUR_REV1:
					path = "romfs:/pattern/empty/ww_eur.acww";
					break;
				case WWRegion::KOR_REV1:
					path = "romfs:/pattern/empty/ww_kor.acww";
					break;
				case WWRegion::UNKNOWN:
					return; // Because invalid.
			}
			break;

		case SaveType::NL:
		case SaveType::WA:
		case SaveType::HHD:
			path = "romfs:/pattern/empty/nl.acnl";
			break;
		case SaveType::UNUSED:
			return; // Because invalid.
	}

	FILE *file = fopen(path.c_str(), "rb");
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	data = std::unique_ptr<u8[]>(new u8[size]);
	fread(data.get(), 1, size, file);
	fclose(file);
}

/* All Palettes. */
/* Color format seems to be: RGBA. */
static const u32 NLPaletteColors[] = {
	0xFFEEFFFF, 0xFF99AAFF, 0xEE5599FF, 0xFF66AAFF, 0xFF0066FF, 0xBB4477FF, 0xCC0055FF, 0x990033FF, 0x552233FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFBBCCFF, 0xFF7777FF, 0xDD3311FF, 0xFF5544FF, 0xFF0000FF, 0xCC6666FF, 0xBB4444FF, 0xBB0000FF, 0x882222FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xEEEEEEFF,
	0xDDCCBBFF, 0xFFCC66FF, 0xDD6622FF, 0xFFAA22FF, 0xFF6600FF, 0xBB8855FF, 0xDD4400FF, 0xBB4400FF, 0x663311FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xDDDDDDFF,
	0xFFEEDDFF, 0xFFDDCCFF, 0xFFCCAAFF, 0xFFBB88FF, 0xFFAA88FF, 0xDD8866FF, 0xBB6644FF, 0x995533FF, 0x884422FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xCCCCCCFF,
	0xFFCCFFFF, 0xEE88FFFF, 0xCC66DDFF, 0xBB88CCFF, 0xCC00FFFF, 0x996699FF, 0x8800AAFF, 0x550077FF, 0x330044FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xBBBBBBFF,
	0xFFBBFFFF, 0xFF99FFFF, 0xDD22BBFF, 0xFF55EEFF, 0xFF00CCFF, 0x885577FF, 0xBB0099FF, 0x880066FF, 0x550044FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xAAAAAAFF,
	0xDDBB99FF, 0xCCAA77FF, 0x774433FF, 0xAA7744FF, 0x993300FF, 0x773322FF, 0x552200FF, 0x331100FF, 0x221100FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x999999FF,
	0xFFFFCCFF, 0xFFFF77FF, 0xDDDD22FF, 0xFFFF00FF, 0xFFDD00FF, 0xCCAA00FF, 0x999900FF, 0x887700FF, 0x555500FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x888888FF,
	0xDDBBFFFF, 0xBB99EEFF, 0x6633CCFF, 0x9955FFFF, 0x6600FFFF, 0x554488FF, 0x440099FF, 0x220066FF, 0x221133FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x777777FF,
	0xBBBBFFFF, 0x8899FFFF, 0x3333AAFF, 0x3355EEFF, 0x0000FFFF, 0x333388FF, 0x0000AAFF, 0x111166FF, 0x000022FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x666666FF,
	0x99EEBBFF, 0x66CC77FF, 0x226611FF, 0x44AA33FF, 0x008833FF, 0x557755FF, 0x225500FF, 0x113322FF, 0x002211FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x555555FF,
	0xDDFFBBFF, 0xCCFF88FF, 0x88AA55FF, 0xAADD88FF, 0x88FF00FF, 0xAABB99FF, 0x66BB00FF, 0x559900FF, 0x336600FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x444444FF,
	0xBBDDFFFF, 0x77CCFFFF, 0x335599FF, 0x6699FFFF, 0x1177FFFF, 0x4477AAFF, 0x224477FF, 0x002277FF, 0x001144FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x333333FF,
	0xAAFFFFFF, 0x55FFFFFF, 0x0088BBFF, 0x55BBCCFF, 0x00CCFFFF, 0x4499AAFF, 0x006688FF, 0x004455FF, 0x002233FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x222222FF,
	0xCCFFEEFF, 0xAAEEDDFF, 0x33CCAAFF, 0x55EEBBFF, 0x00FFCCFF, 0x77AAAAFF, 0x00AA99FF, 0x008877FF, 0x004433FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF,
	0xAAFFAAFF, 0x77FF77FF, 0x66DD44FF, 0x00FF00FF, 0x22DD22FF, 0x55BB55FF, 0x00BB00FF, 0x008800FF, 0x224422FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

/* Color format seems to be: BRGA. */
static const u32 WWPaletteColors[] = {
	0xFF0000FF, 0xFF7331FF, 0xFFAD00FF, 0xFFFF00FF, 0xADFF00FF, 0x52FF00FF, 0x00FF00FF, 0x00AD52FF, 0x0052ADFF, 0x0000FFFF, 0x5200FFFF, 0xAD00FFFF, 0xFF00FFFF, 0x000000FF, 0xFFFFFFFF,
	0xFF7B7BFF, 0xFFB57BFF, 0xFFE77BFF, 0xFFFF7BFF, 0xDEFF7BFF, 0xADFF7BFF, 0x7BFF7BFF, 0x52AD84FF, 0x5284ADFF, 0x7B7BFFFF, 0xB57BFFFF, 0xE77BFFFF, 0xFF7BFFFF, 0x000000FF, 0xFFFFFFFF,
	0xA50000FF, 0xA53100FF, 0xA57300FF, 0xA5A500FF, 0x73A500FF, 0x31A500FF, 0x00A500FF, 0x005221FF, 0x002152FF, 0x0000A5FF, 0x3100A5FF, 0x7300A5FF, 0xA500A5FF, 0x000000FF, 0xFFFFFFFF,
	0x009C00FF, 0x5ACE6BFF, 0xB5FFDEFF, 0x009C6BFF, 0x52CEA5FF, 0xADFFD6FF, 0x0052ADFF, 0x2984D6FF, 0x5AADFFFF, 0x0000FFFF, 0x4A6BFFFF, 0x314ADEFF, 0x1821B5FF, 0x00008CFF, 0xFFFFFFFF,
	0xAD7300FF, 0xD6AD42FF, 0xFFDE8CFF, 0xFF0839FF, 0xFF4A6BFF, 0xFF949CFF, 0xAD00FFFF, 0xD663FFFF, 0xFFCEFFFF, 0xFFBD9CFF, 0xDE9473FF, 0xBD634AFF, 0x9C3921FF, 0x7B1000FF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF5200FF, 0xFFB55AFF, 0xFFEFADFF, 0x7B1000FF, 0xA54A31FF, 0xD6846BFF, 0xFFBD9CFF, 0x5AADFFFF, 0x84C6FFFF, 0xADE7FFFF, 0xD6FFFFFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x00FF00FF, 0x42FF42FF, 0x8CFF8CFF, 0xD6FFD6FF, 0x0000FFFF, 0x4242FFFF, 0x8C8CFFFF, 0xD6D6FFFF, 0xFF0000FF, 0xFF4242FF, 0xFF8C8CFF, 0xFFD6D6FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x003100FF, 0x426342FF, 0x849C84FF, 0xC6D6C6FF, 0x7B1000FF, 0xA54A29FF, 0xD68C5AFF, 0xFFC68CFF, 0xD6B500FF, 0xE7CE39FF, 0xF7DE7BFF, 0xFFF7BDFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x0000FFFF, 0xFF0000FF, 0xFFFF00FF, 0x4242FFFF, 0xFF4242FF, 0xFFFF42FF, 0x8C8CFFFF, 0xFF8C8CFF, 0xFFFF8CFF, 0xD6D6FFFF, 0xFFD6D6FF, 0xFFFFD6FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x00FF00FF, 0x0000FFFF, 0xFF00FFFF, 0x42FF42FF, 0x4242FFFF, 0xFF42FFFF, 0x8CFF8CFF, 0x8C8CFFFF, 0xFF8CFFFF, 0xD6FFD6FF, 0xD6D6FFFF, 0xFFD6FFFF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF7B00FF, 0xFFFF00FF, 0x84FF00FF, 0x00FF00FF, 0x00847BFF, 0x0000FFFF, 0x7B00FFFF, 0xFF94FFFF, 0xD6B500FF, 0xBD1000FF, 0x5A1000FF, 0x6B6B6BFF, 0x000000FF, 0xFFFFFFFF,
	0x109463FF, 0x087B52FF, 0x108C39FF, 0x319C31FF, 0xCEA54AFF, 0xCE9439FF, 0xBD8C4AFF, 0xD68C31FF, 0xAD734AFF, 0x8C5A31FF, 0x6B4229FF, 0x84EFFFFF, 0x31CEEFFF, 0x00A5C6FF, 0xFFFFFFFF,
	0xD6DEE7FF, 0xB5CEDEFF, 0xE7EFEFFF, 0xF7F7F7FF, 0x84737BFF, 0x948C6BFF, 0x847B63FF, 0x9C845AFF, 0x739CB5FF, 0xFF2929FF, 0xFFFF00FF, 0x9421FFFF, 0x009CBDFF, 0x000000FF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xF7EFEFFF, 0xE7DEDEFF, 0xD6CECEFF, 0xC6B5B5FF, 0xB5A5A5FF, 0xA59494FF, 0x9C8484FF, 0x8C6B6BFF, 0x7B5A5AFF, 0x6B4A4AFF, 0x5A3131FF, 0x4A2121FF, 0x421010FF, 0x310000FF,
	0xFFFFFFFF, 0xEFEFEFFF, 0xDEDEDEFF, 0xCECECEFF, 0xB5B5B5FF, 0xA5A5A5FF, 0x949494FF, 0x848484FF, 0x6B6B6BFF, 0x5A5A5AFF, 0x4A4A4AFF, 0x313131FF, 0x212121FF, 0x101010FF, 0x000000FF,
	0xFF8C7BFF, 0xFF0000FF, 0xFF7B00FF, 0xFFFF00FF, 0x008400FF, 0x00FF00FF, 0x0000FFFF, 0x009CFFFF, 0xD600FFFF, 0xFF6BFFFF, 0x9C0000FF, 0xFF9400FF, 0xFFBD94FF, 0x000000FF, 0xFFFFFFFF
};

/* Get a C2D_Image from the savefile for example. */
C2D_Image CoreUtils::savePatternImage(std::unique_ptr<PatternImage> image, SaveType ST) {
	if (image) {
		u32 *buffer = (u32*)linearAlloc(sizeof(u32) * 32 * 32); // Allocate Buffer.

		switch(ST) {
			case SaveType::NL:
			case SaveType::WA:
			case SaveType::HHD:
				for (int i = 0; i < 0x200; i++) {
					buffer[i * 2] = NLPaletteColors[image->getPaletteColor(image->getPixel(i).left)]; // Left pixel.
					buffer[i * 2 + 1] = NLPaletteColors[image->getPaletteColor(image->getPixel(i).right)]; // Right pixel.
				}
				break;

			case SaveType::WW:
				for (int i = 0; i < 0x200; i++) {
					buffer[i * 2] = WWPaletteColors[std::max<u8>(0, image->getPaletteColor(image->getPixel(i).left) - 1)]; // Left pixel.
					buffer[i * 2 + 1] = WWPaletteColors[std::max<u8>(0, image->getPaletteColor(image->getPixel(i).right) - 1)]; // Right pixel.
				}
				break;

			case SaveType::UNUSED:
				linearFree(buffer); // Free buffer cause unneeded.
				return {nullptr};
		}


		C2D_Image tmp = C2DUtils::ImageDataToC2DImage(buffer, 32, 32, GPU_RGBA8);
		linearFree(buffer); // Free buffer cause unneeded.
		return tmp;
	}

	return {nullptr};
}

/* Get a C2D_Image of the pattern buffer. */
C2D_Image CoreUtils::patternImage(std::unique_ptr<PatternImage> &image, SaveType ST) {
	if (image) {
		u32 *buffer = (u32*)linearAlloc(sizeof(u32) * 32 * 32); // Allocate Buffer.

		switch(ST) {
			case SaveType::NL:
			case SaveType::WA:
			case SaveType::HHD:
				for (int i = 0; i < 0x200; i++) {
					buffer[i * 2] = NLPaletteColors[image->getPaletteColor(image->getPixel(i).left)]; // Left pixel.
					buffer[i * 2 + 1] = NLPaletteColors[image->getPaletteColor(image->getPixel(i).right)]; // Right pixel.
				}
				break;

			case SaveType::WW:
				for (int i = 0; i < 0x200; i++) {
					buffer[i * 2] = WWPaletteColors[std::max<u8>(0, image->getPaletteColor(image->getPixel(i).left) - 1)]; // Left pixel.
					buffer[i * 2 + 1] = WWPaletteColors[std::max<u8>(0, image->getPaletteColor(image->getPixel(i).right) - 1)]; // Right pixel.
				}
				break;

			case SaveType::UNUSED:
				linearFree(buffer); // Free buffer cause unneeded.
				return {nullptr};
		}


		C2D_Image tmp = C2DUtils::ImageDataToC2DImage(buffer, 32, 32, GPU_RGBA8);
		linearFree(buffer); // Free buffer cause unneeded.
		return tmp;
	}

	return {nullptr};
}

/* Get the default file. */
std::string CoreUtils::getDefaultFile(SaveType ST, WWRegion region) {
	switch(ST) {
		case SaveType::WW:
			/* Switch Region. */
			switch(region) {
				case WWRegion::JPN_REV0:
				case WWRegion::JPN_REV1:
					if (access("sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwJPNDefault.pt", F_OK) == 0) {
						return "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwJPNDefault.pt";
					} else {
						return "romfs:/defaults/wwJPNDefault.pt";
					}

				case WWRegion::USA_REV0:
				case WWRegion::USA_REV1:
					if (access("sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwUSADefault.pt", F_OK) == 0) {
						return "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwUSADefault.pt";
					} else {
						return "romfs:/defaults/wwUSADefault.pt";
					}

				case WWRegion::EUR_REV1:
					if (access("sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwEURDefault.pt", F_OK) == 0) {
						return "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwEURDefault.pt";
					} else {
						return "romfs:/defaults/wwEURDefault.pt";
					}

				case WWRegion::KOR_REV1:
					if (access("sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwKORDefault.pt", F_OK) == 0) {
						return "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwKORDefault.pt";
					} else {
						return "romfs:/defaults/wwKORDefault.pt";
					}

				case WWRegion::UNKNOWN:
					return "";
			}

		case SaveType::NL:
			if (access("sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/nlDefault.pt", F_OK) == 0) {
				return "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/nlDefault.pt";
			} else {
				return "romfs:/defaults/nlDefault.pt";
			}

		case SaveType::WA:
		case SaveType::HHD:
			if (access("sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/waDefault.pt", F_OK) == 0) {
				return "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/waDefault.pt";
			} else {
				return "romfs:/defaults/waDefault.pt";
			}

		case SaveType::UNUSED:
			return "";
	}

	return "";
}


PatternInformations CoreUtils::getDefaultInformation(SaveType ST, WWRegion region) {
	PatternInformations output = {StringUtils::UTF8toUTF16("?"), StringUtils::UTF8toUTF16("?"), StringUtils::UTF8toUTF16("?"), 0, 0, 0}; // Default.

	bool UTF8Read = true; /* If UTF-8 or UTF-16 Read. */
	u8 patternLength = 0, creatorLength = 0, townLength = 0; /* Name Reading Length. */
	u32 creatorNameStart = 0, townNameStart = 0, creatorIDStart = 0, townIDStart = 0, creatorGenderStart = 0;
	u32 pSize = 0;

	switch(ST) {
		case SaveType::WW:
			/* Switch Region. */
			switch(region) {
				case WWRegion::JPN_REV0:
				case WWRegion::JPN_REV1:
					patternLength = 9;
					creatorNameStart = 0x9;
					creatorLength = 6;
					townNameStart = 0xF;
					townLength = 6;
					creatorIDStart = 0x15;
					townIDStart = 0x17;
					creatorGenderStart = 0x19;
					pSize = 0x1A;
					break;

				case WWRegion::USA_REV0:
				case WWRegion::USA_REV1:
				case WWRegion::EUR_REV1:
					patternLength = 15;
					creatorNameStart = 0xF;
					creatorLength = 7;
					townNameStart = 0x16;
					townLength = 7;
					creatorIDStart = 0x1D;
					townIDStart = 0x1F;
					creatorGenderStart = 0x21;
					pSize = 0x22;
					break;

				case WWRegion::KOR_REV1:
					patternLength = 10;
					creatorNameStart = 0x14;
					creatorLength = 6;
					townNameStart = 0x20;
					townLength = 6;
					creatorIDStart = 0x2C;
					townIDStart = 0x2E;
					creatorGenderStart = 0x30;
					pSize = 0x31;
					UTF8Read = false;
					break;

				case WWRegion::UNKNOWN:
					break; // Because invalid.
			}

			break;

		case SaveType::NL:
			patternLength = 20;
			creatorNameStart = 0x28;
			creatorLength = 8;
			townNameStart = 0x38;
			townLength = 8;
			creatorIDStart = 0x48;
			townIDStart = 0x4A;
			creatorGenderStart = 0x4C;
			pSize = 0x4D;
			UTF8Read = false;
			break;

		case SaveType::WA:
		case SaveType::HHD:
			patternLength = 20;
			creatorNameStart = 0x28;
			creatorLength = 8;
			townNameStart = 0x38;
			townLength = 8;
			creatorIDStart = 0x48;
			townIDStart = 0x4A;
			creatorGenderStart = 0x4C;
			pSize = 0x4D;
			UTF8Read = false;
			break;

		case SaveType::UNUSED:
			break;
	}

	/* Real part here with fetching data. */
	if (CoreUtils::getDefaultFile(ST, region) != "") {

		FILE *file = fopen(CoreUtils::getDefaultFile(ST, region).c_str(), "rb");
		if (file) {
			fseek(file, 0, SEEK_END);
			u32 size = ftell(file);
			if (size < (pSize)) return output;
			fseek(file, 0, SEEK_SET);
			std::shared_ptr<u8[]> data = std::shared_ptr<u8[]>(new u8[size]);
			fread(data.get(), 1, size, file);
			fclose(file);

			/* fetch! */
			if (data) {
			
				/* String read. */
				if (UTF8Read) {
					output.PatternName = StringUtils::ReadUTF8String(data.get(), 0, patternLength, region);
					output.CreatorName = StringUtils::ReadUTF8String(data.get(), creatorNameStart, creatorLength, region);
					output.TownName = StringUtils::ReadUTF8String(data.get(), townNameStart, townLength, region);
				} else {
					output.PatternName = StringUtils::ReadUTF16String(data.get(), 0, patternLength);
					output.CreatorName = StringUtils::ReadUTF16String(data.get(), creatorNameStart, creatorLength);
					output.TownName = StringUtils::ReadUTF16String(data.get(), townNameStart, townLength);
				}

				/* ID read. */
				output.CreatorID = SaveUtils::Read<u16>(data.get(), creatorIDStart);
				output.TownID = SaveUtils::Read<u16>(data.get(), townIDStart);
				output.CreatorGender = data.get()[creatorGenderStart];
			}
		}
	}

	return output;
}


/* Dump Pattern Information for the Pattern Editor Tool. */
void CoreUtils::dumpPatternInformation(SaveType ST, WWRegion region, std::unique_ptr<Pattern> &ptrn) {
	if (!ptrn) return; /* Pattern is not valid. */

	bool UTF8Read = true; /* If UTF-8 or UTF-16 Read. */
	u8 patternLength = 0, creatorLength = 0, townLength = 0; /* Name Reading Length. */
	u32 creatorNameStart = 0, townNameStart = 0, creatorIDStart = 0, townIDStart = 0, creatorGenderStart = 0;
	std::string file = "";
	u32 pSize = 0;

	switch(ST) {
		case SaveType::WW:
			/* Switch Region. */
			switch(region) {
				case WWRegion::JPN_REV0:
				case WWRegion::JPN_REV1:
					patternLength = 9;
					creatorNameStart = 0x9;
					creatorLength = 6;
					townNameStart = 0xF;
					townLength = 6;
					creatorIDStart = 0x15;
					townIDStart = 0x17;
					creatorGenderStart = 0x19;
					file = "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwJPNDefault.pt";
					pSize = 0x1A;
					break;

				case WWRegion::USA_REV0:
					patternLength = 15;
					creatorNameStart = 0xF;
					creatorLength = 7;
					townNameStart = 0x16;
					townLength = 7;
					creatorIDStart = 0x1D;
					townIDStart = 0x1F;
					creatorGenderStart = 0x21;
					file = "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwUSADefault.pt";
					pSize = 0x22;
					break;

				case WWRegion::USA_REV1:
				case WWRegion::EUR_REV1:
					patternLength = 15;
					creatorNameStart = 0xF;
					creatorLength = 7;
					townNameStart = 0x16;
					townLength = 7;
					creatorIDStart = 0x1D;
					townIDStart = 0x1F;
					creatorGenderStart = 0x21;
					file = "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwEURDefault.pt";
					pSize = 0x22;
					break;

				case WWRegion::KOR_REV1:
					patternLength = 10;
					creatorNameStart = 0x14;
					creatorLength = 6;
					townNameStart = 0x20;
					townLength = 6;
					creatorIDStart = 0x2C;
					townIDStart = 0x2E;
					creatorGenderStart = 0x30;
					file = "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/wwKORDefault.pt";
					pSize = 0x31;
					UTF8Read = false;
					break;

				case WWRegion::UNKNOWN:
					return;
			}

			break;

		case SaveType::NL:
			patternLength = 20;
			creatorNameStart = 0x28;
			creatorLength = 8;
			townNameStart = 0x38;
			townLength = 8;
			creatorIDStart = 0x48;
			townIDStart = 0x4A;
			creatorGenderStart = 0x4C;
			file = "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/nlDefault.pt";
			pSize = 0x4D;
			UTF8Read = false;
			break;

		case SaveType::WA:
		case SaveType::HHD:
			patternLength = 20;
			creatorNameStart = 0x28;
			creatorLength = 8;
			townNameStart = 0x38;
			townLength = 8;
			creatorIDStart = 0x48;
			townIDStart = 0x4A;
			creatorGenderStart = 0x4C;
			file = "sdmc:/3ds/LeafEdit/Pattern-Editor/defaults/waDefault.pt";
			pSize = 0x4D;
			UTF8Read = false;
			break;

		case SaveType::UNUSED:
			return;
	}

	FILE *dmp = fopen(file.c_str(), "w");
	if (dmp) {
		std::unique_ptr<u8[]> data = std::unique_ptr<u8[]>(new u8[pSize]);

		/* Write. */
		if (data) {
			/* String write. */
			if (UTF8Read) {
				StringUtils::WriteUTF8String(data.get(), ptrn->name(), 0, patternLength, region);
				StringUtils::WriteUTF8String(data.get(), ptrn->creatorname(), creatorNameStart, creatorLength, region);
				StringUtils::WriteUTF8String(data.get(), ptrn->origtownname(), townNameStart, townLength, region);
			} else {
				StringUtils::WriteUTF16String(data.get(), ptrn->name(), 0, patternLength);
				StringUtils::WriteUTF16String(data.get(), ptrn->creatorname(), creatorNameStart, creatorLength);
				StringUtils::WriteUTF16String(data.get(), ptrn->origtownname(), townNameStart, townLength);
			}

			/* ID write. */
			SaveUtils::Write<u16>(data.get(), creatorIDStart, ptrn->creatorid());
			SaveUtils::Write<u16>(data.get(), townIDStart, ptrn->origtownid());
			data.get()[creatorGenderStart] = ptrn->creatorGender();

			fwrite(data.get(), 1, pSize, dmp);
			fclose(dmp);
		}
	}
}