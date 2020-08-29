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

#ifndef _LEAFEDIT_PATTERN_EDITOR_STORAGE_HPP
#define _LEAFEDIT_PATTERN_EDITOR_STORAGE_HPP

#include "Pattern.hpp"
#include "PatternHHD.hpp"
#include "PatternNL.hpp"
#include "PatternWA.hpp"
#include "PatternWW.hpp"
#include "Sav.hpp"

class Pattern;
class PatternHHD;
class PatternWW;
class PatternNL;
class PatternWA;
class Storage {
public:
	Storage(const std::string& fileName);
	~Storage() { if (data)  data = nullptr; }
	std::unique_ptr<Pattern> pattern(int slot) const;
	void pattern(const Pattern &ptrn, int slot);
	void load();
	bool save() const;
	int boxes() const;
	int slots() const;
	void resize(size_t boxes);
	bool used(u32 slot) const;
	u32 getSize(u32 slot) const;
	std::string name() { return this->storageFileName; }
	void name(std::string v) { this->storageFileName = v; }
	void createStorage();
private:
	static constexpr int STORAGE_VERSION = 1;
	static std::string MAGIC;

	

	struct StorageHeader {
		const char MAGIC[4];
		int version;
		int slots;
		int boxes;
	};

	struct PatternEntry {
		bool used;
		WWRegion region;
		SaveType ST;
		u32 patternSize;
		u8 data[0x870];
	};

	std::unique_ptr<u8[]> data = nullptr;
	size_t size;
	std::string storageFileName;
};

#endif
