/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef GRAPHICS_MACGUI_MACFONTMANAGER_H
#define GRAPHICS_MACGUI_MACFONTMANAGER_H

#include "graphics/fontman.h"

namespace Graphics {

enum {
	kMacFontNonStandard = -1,
	kMacFontChicago = 0
};

enum {
	kMacFontRegular,
	kMacFontBold,
	kMacFontItalic
};

class BdfFont;

class MacFont {
public:
	MacFont(int id = kMacFontChicago, int size = 12, int slant = kMacFontRegular, FontManager::FontUsage fallback = Graphics::FontManager::kBigGUIFont) {
		_id = id;
		_size = size;
		_slant = slant;
		_fallback = fallback;
		_generated = false;
	}

	int getId() { return _id; };
	int getSize() { return _size; }
	int getSlant() { return _slant; }
	Common::String getName() { return _name; }
	void setName(Common::String &name) { _name = name; }
	void setName(const char *name) { _name = name; }
	FontManager::FontUsage getFallback() { return _fallback; }
	bool isGenerated() { return _generated; }
	void setGenerated(bool gen) { _generated = gen; }

private:
	int _id;
	int _size;
	int _slant;
	Common::String _name;
	FontManager::FontUsage _fallback;

	bool _generated;
};

class MacFontManager {
public:
	MacFontManager();

	/**
	 * Accessor method to check the presence of built-in fonts.
	 * @return True if there are bult-in fonts.
	 */
	bool hasBuiltInFonts() { return _builtInFonts; }
	/**
	 * Retrieve a font from the available ones.
	 * @param name Name of the desired font.
	 * @param fallback Fallback policy in case the desired font isn't there.
	 * @return The requested font or the fallback.
	 */
	const Font *getFont(MacFont macFont);

private:
	void loadFonts();

	/**
	 * Return font name from standard ID
	 * @param id ID of the font
	 * @param size size of the font
	 * @return the font name or NULL if ID goes beyond the mapping
	 */
	const char *getFontName(int id, int size, int slant = kMacFontRegular);
	const char *getFontName(MacFont &font);

	void generateFontSubstitute(MacFont &macFont);
	void generateFont(MacFont fromFont, MacFont toFont);

private:
	bool _builtInFonts;
	Common::HashMap<Common::String, MacFont *> _fontRegistry;

	Common::HashMap<Common::String, int> _fontNames;

	int parseFontSlant(Common::String slant);
};

} // End of namespace Graphics

#endif
