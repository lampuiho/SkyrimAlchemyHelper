/******************************************************************************
*                            Skyrim Alchemy Helper                            *
*******************************************************************************
*                                                                             *
* Copyright (C) 2015 Christophe Guebert                                       *
*                                                                             *
* This program is free software; you can redistribute it and/or modify        *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation; either version 2 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc.,     *
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
*******************************************************************************
*                        Skyrim Alchemy Helper :: Gui                         *
*                                                                             *
*                  Contact: christophe.guebert+SAH@gmail.com                  *
******************************************************************************/
#pragma once

#include <QString>
#include <QVector>

class Settings
{
public:
	static Settings& instance();

	QString dataFolder,		// Skyrim data folder ([...]/Skyrim/Data)
		savesFolder,		// Saves folder ([User documents]/My Games/Skyrim/Saves)
		pluginsListPath,	// Path to the file containing the plugins list ([User Local App Data]/Skyrim/plugins.txt)
		modOrganizerPath,	// Path to ModOrganizer (C:/Program files/Mod Organizer/Mor Organizer.exe)
		selectedSavePath,	// File path of the game save to load
		language;			// When parsing mods, try to use the strings table for this language

	bool useModOrganizer = false,	// Is Mod Organizer used to separate mods ?
		loadMostRecentSave = true,	// If true, always load the most recent save instead of a user chosen one
		playerOnly = false,			// If true, only parse the player and ignore all containers
		getContainersInfo = true,	// If true, extract containers information from the mods
		sameCellAsPlayer = false,	// If true, only keep containers that are in the same cell as the player
		interiorCellsOnly = false;	// If true, only keep containers that are in interior cells

	int maxValidIngredientCount = 1000, // When parsing a save, an ingredient count higher than this is considered invalid (if 0, accept everything)
		minValidNbIngredients = 5, // When parsing a save, ignore containers and the player inventory if there is less than this number of different ingredients (if 0, accept everything)
		minTotalIngredientsCount = 25; // When parsing a save, ignore containers that have less than this total number of ingredients

	bool isEmpty();
	void clearEmptyFlag();

	void load();
	void save();

protected:
	Settings();

	bool m_isEmpty = false;
};
