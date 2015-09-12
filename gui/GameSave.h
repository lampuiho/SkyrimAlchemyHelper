#pragma once

#include <QString>
#include <QPixmap>
#include <QFileInfo>

#include <array>
#include <vector>

class GameSave
{
public:
	static GameSave& instance() // Not a singleton, just the one everyone use
	{ static GameSave save; return save; }

	GameSave();

	QFileInfoList savesList();
	void loadSaveFromConfig();
	void load(QString fileName);
	bool isLoaded() const;

	void setMaxValidIngredientCount(int count); // Everything higher than this is considered invalid (if 0, accept everything)
	void setMinValidNbIngredients(int nb); // Ignore containers (and the player inventory) if there is less than nb different ingredients (if 0, accept everything)

	QPixmap screenshot() const;

	struct Header
	{
		int saveNumber = 0, playerLevel = 0;
		QString playerName, playerLocation;
	};

	Header header() const;

	using KnownIngredients = std::vector<std::array<bool, 4>>;
	const KnownIngredients& knownIngredients() const; // Bool for each magical effect

	using InventoryItem = std::pair<int, int>; // Ingredient index in IngredientsList, number of ingredients
	using Inventory = std::vector<InventoryItem>;
	const Inventory& inventory() const;

	struct Container
	{
		quint32 id;
		Inventory inventory;
	};
	using Containers = std::vector<Container>;
	const Containers& containers() const;

	const std::vector<int> ingredientsCount() const; // Same as inventory, but more direct and with the 0-count ingredients

protected:
	void clear();

	bool m_isLoaded = false;
	QPixmap m_screenshot;
	Header m_header;
	KnownIngredients m_knownIngredients;
	Inventory m_inventory;
	Containers m_containers;
	std::vector<int> m_ingredientsCount;
	int m_maxValidIngredientCount, m_minValidNbIngredients;
};

//****************************************************************************//

inline bool GameSave::isLoaded() const
{ return m_isLoaded; }

inline QPixmap GameSave::screenshot() const
{ return m_screenshot; }

inline GameSave::Header GameSave::header() const
{ return m_header; }

inline const GameSave::KnownIngredients& GameSave::knownIngredients() const
{ return m_knownIngredients; }

inline const GameSave::Inventory& GameSave::inventory() const
{ return m_inventory; }

inline const std::vector<int> GameSave::ingredientsCount() const
{ return m_ingredientsCount; }

inline void GameSave::setMaxValidIngredientCount(int count)
{ m_maxValidIngredientCount = count; }

inline void GameSave::setMinValidNbIngredients(int nb)
{ m_minValidNbIngredients = nb; }

inline const GameSave::Containers& GameSave::containers() const
{ return m_containers; }
