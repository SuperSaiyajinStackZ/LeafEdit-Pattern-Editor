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

#include "storage.hpp"
#include <unistd.h>

#include "Pattern.hpp"


std::string Storage::MAGIC = "LEPE";

Storage::Storage(const std::string& fileName) : storageFileName(fileName) {
	load();
}

void Storage::load() {
	if (data) data = nullptr;

	bool needSave = false;
	FILE* in = fopen(storageFileName.c_str(), "rb");

	if (in) {
		StorageHeader h{"BAD", 0, 0, 0};
		fseek(in, 0, SEEK_END);
		size = ftell(in);
		fseek(in, 0, SEEK_SET);
		fread(&h, 1, sizeof(StorageHeader) - sizeof(int) - sizeof(int), in);
		if (memcmp(&h, MAGIC.data(), 4)) {
			fclose(in);
			createStorage();
			needSave = true;
		} else {
			data = std::unique_ptr<u8[]>(new u8[size]);
			fread(&h.slots, 1, sizeof(int), in);
			fread(&h.boxes, 1, sizeof(int), in);
			std::copy((char*)&h, (char*)(&h + 1), data.get());
			fread(data.get() + sizeof(StorageHeader), 1, size - sizeof(StorageHeader), in);
			fclose(in);
		}
		
	} else {
		createStorage();
		needSave = true;
	}

	if (needSave) {
		save();
	}
}

bool Storage::save() const {
	FILE* out = fopen(storageFileName.c_str(), "wb");
	if (out) {
		fwrite(data.get(), 1, sizeof(StorageHeader) + sizeof(PatternEntry) * slots(), out);
		fclose(out);
		return true;
	} else {
		return false;
	}
}

void Storage::resize(size_t boxes) {
	size_t newSize = sizeof(StorageHeader) + sizeof(PatternEntry) * boxes * 10;

	if (newSize != size) {
		std::unique_ptr<u8[]> newData = std::unique_ptr<u8[]>(new u8[newSize]);
		std::copy(data.get(), data.get() + std::min(newSize, size), newData.get());

		data = nullptr;
		if (newSize > size) {
			std::fill_n(newData.get() + size, newSize - size, 0x0);
		}
		
		data = std::move(newData);

		((StorageHeader*)data.get())->boxes = boxes;
		((StorageHeader*)data.get())->slots = boxes * 10;

		save();
	}
	
	size = newSize;
}

std::unique_ptr<Pattern> Storage::pattern(int slot) const {
	if (slot > this->slots()) return nullptr;

	PatternEntry* entries = (PatternEntry*)(data.get() + sizeof(StorageHeader));
	SaveType st = entries[slot].ST;

	switch(st) {
		case SaveType::WW:
			switch(entries[slot].region) {
				case WWRegion::USA_REV0:
				case WWRegion::USA_REV1:
					return std::make_unique<PatternWW>(entries[slot].data, 0, WWRegion::USA_REV1);

				case WWRegion::EUR_REV1:
					return std::make_unique<PatternWW>(entries[slot].data, 0, WWRegion::EUR_REV1);

				case WWRegion::JPN_REV0:
				case WWRegion::JPN_REV1:
					return std::make_unique<PatternWW>(entries[slot].data, 0, WWRegion::JPN_REV1);

				case WWRegion::KOR_REV1:
					return std::make_unique<PatternWW>(entries[slot].data, 0, WWRegion::KOR_REV1);

				case WWRegion::UNKNOWN:
					return nullptr;
			}
			
		case SaveType::NL:
			return std::make_unique<PatternNL>(entries[slot].data, 0);

		case SaveType::WA:
			return std::make_unique<PatternWA>(entries[slot].data, 0);
		case SaveType::HHD:
			return std::make_unique<PatternHHD>(entries[slot].data, 0);

		case SaveType::UNUSED:
			return nullptr;
	}

	return nullptr;
}

void Storage::pattern(const Pattern &ptrn, int slot) {
	if (slot > this->slots()) return;

	PatternEntry* entries = (PatternEntry*)(data.get() + sizeof(StorageHeader));
	PatternEntry newEntry;

	newEntry.ST = ptrn.getType();
	newEntry.region = ptrn.getRegion();
	newEntry.patternSize = ptrn.getSize();
	
	if (ptrn.getSize() > 0) newEntry.used = true;
	else newEntry.used = false;
	std::copy(ptrn.returnData(), ptrn.returnData() + ptrn.getSize(), newEntry.data);

	if (ptrn.getSize() < 0x870) {
		std::fill_n(newEntry.data + ptrn.getSize(), 0x870 - ptrn.getSize(), 0x0);
	}

	entries[slot] = newEntry;
}

void Storage::createStorage() {
	if (data) data = nullptr;

	/* Create new data. */
	data = std::unique_ptr<u8[]>(new u8[size = sizeof(StorageHeader) + sizeof(PatternEntry) * 10]);
	std::copy(MAGIC.data(), MAGIC.data() + MAGIC.size(), data.get());
	*(int*)(data.get() + 4)  = STORAGE_VERSION;
	*(int*)(data.get() + 8) = 10;
	*(int*)(data.get() + 12) = 1;
	std::fill_n(data.get() + sizeof(StorageHeader), sizeof(PatternEntry) * 10, 0x00);
}

int Storage::boxes() const {
	return ((StorageHeader*)data.get())->boxes;
}

int Storage::slots() const {
	return ((StorageHeader*)data.get())->slots;
}

bool Storage::used(u32 slot) const {
	if (slot > (u32)this->slots()) return false;
	PatternEntry* entries = (PatternEntry*)(data.get() + sizeof(StorageHeader));
	return entries[slot].used;
}

u32 Storage::getSize(u32 slot) const {
	if (slot > (u32)this->slots()) return 0;
	PatternEntry* entries = (PatternEntry*)(data.get() + sizeof(StorageHeader));
	return entries[slot].patternSize;
}