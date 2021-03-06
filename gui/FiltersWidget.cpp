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
#include <QtWidgets>

#include "FiltersWidget.h"
#include "FlowLayout.h"

#include "Config.h"
#include "PotionsList.h"

FiltersWidget::FiltersWidget(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->setSpacing(3);

	auto widgetsLayout = new FlowLayout(5, 5, 5);
	m_inventoryCheckBox = new QCheckBox("Only available ingredients");
//	m_inventoryCheckBox->setCheckState(Qt::Checked);
	widgetsLayout->addWidget(m_inventoryCheckBox);
	connect(m_inventoryCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updatePotionsListFilters()));

	m_ingredientsCountComboBox = new QComboBox;
	QStringList ingredientsCountChoices;
	ingredientsCountChoices << "2 or 3 ingredients" << "2 ingredients" << "3 ingredients";
	m_ingredientsCountComboBox->addItems(ingredientsCountChoices);
	widgetsLayout->addWidget(m_ingredientsCountComboBox);
	connect(m_ingredientsCountComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePotionsListFilters()));

	m_purityComboBox = new QComboBox;
	QStringList purityChoices;
	purityChoices << "Any purity" << "Pure" << "Pure positive" << "Pure negative";
	m_purityComboBox->addItems(purityChoices);
	widgetsLayout->addWidget(m_purityComboBox);
	connect(m_purityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePotionsListFilters()));

	vLayout->addLayout(widgetsLayout);

	m_effectsLayout = new FlowLayout(0, 0, 0);
	m_effectsLayout->setSpacing(0);
	vLayout->addLayout(m_effectsLayout);

	m_ingredientsLayout = new FlowLayout(0, 0, 0);
	m_ingredientsLayout->setSpacing(0);
	vLayout->addLayout(m_ingredientsLayout);

	vLayout->addStretch();
	setLayout(vLayout);
}

QSize FiltersWidget::sizeHint() const
{
	return QSize(500, 110);
}

void FiltersWidget::clear()
{
	while (auto child = m_effectsLayout->takeAt(0))
	{
		auto w = child->widget();
		if (w) delete w;
		delete child;
	}

	while (auto child = m_ingredientsLayout->takeAt(0))
	{
		auto w = child->widget();
		if (w) delete w;
		delete child;
	}

	m_effectsFilters.clear();
	m_ingredientsFilters.clear();

	const QSignalBlocker invBlocker(m_inventoryCheckBox);
	const QSignalBlocker ingBlocker(m_ingredientsCountComboBox);
	const QSignalBlocker purBlocker(m_purityComboBox);

	m_inventoryCheckBox->setCheckState(Qt::Unchecked);
	m_ingredientsCountComboBox->setCurrentIndex(0);
	m_purityComboBox->setCurrentIndex(0);
}

bool FiltersWidget::updateExisting(std::vector<FilterItem>& list, FlowLayout *layout, FilterActionType action, int id)
{
	auto it = std::find_if(list.begin(), list.end(), [id](const FilterItem& item){
		return item.id == id;
	});
	if (it != list.end())
	{
		auto oldAction = it->actionType;
		auto widget = it->widget;
		switch (action)
		{
		case FilterActionType::addFilterContains:
			if (oldAction == FilterActionType::addFilterContains)
				return false;
			else if (oldAction == FilterActionType::addFilterDoesNotContain)
			{
				list.erase(it);
				removeWidget(layout, widget);
			}
			break;
		case FilterActionType::addFilterDoesNotContain:
			if (oldAction == FilterActionType::addFilterDoesNotContain)
				return false;
			else if (oldAction == FilterActionType::addFilterContains)
			{
				list.erase(it);
				removeWidget(layout, widget);
			}
			break;
		case FilterActionType::RemoveFilter:
			list.erase(it);
			removeWidget(layout, widget);
			updatePotionsListFilters();
			return false;
		}
	}

	return true;
}

void FiltersWidget::effectFilterAction(FilterActionType action, int id)
{
	// Do we already have this effect ?
	if (!updateExisting(m_effectsFilters, m_effectsLayout, action, id))
		return;

	if (action == FilterActionType::RemoveFilter)
		return;

	// Add this effect
	const auto& effect = Config::main().effects[id];

	auto boxWidget = new QWidget;
	auto boxLayout = new QHBoxLayout(boxWidget);
	boxLayout->setContentsMargins(0, 0, 5, 0);

	auto removeAction = new QAction(boxLayout);
	removeAction->setData(QVariant(id));
	auto removeButton = new QPushButton(QIcon("://icons/Delete.png"), "");
	removeButton->setMaximumSize(24, 24);
	removeButton->setFlat(true);
	removeButton->setToolTip(tr("Remove this filter"));
	connect(removeButton, SIGNAL(clicked(bool)), removeAction, SLOT(trigger()));
	connect(removeAction, SIGNAL(triggered(bool)), this, SLOT(removeEffect()));
	boxLayout->addWidget(removeButton);

	auto label = new QLabel((action == FilterActionType::addFilterContains ? "Has " : "Does not have ") + effect.name);
	boxLayout->addWidget(label);

	m_effectsLayout->addWidget(boxWidget);

	m_effectsFilters.emplace_back(action, id, boxWidget);

	updatePotionsListFilters();
}

void FiltersWidget::ingredientFilterAction(FilterActionType action, int id)
{
	// Do we already have this ingredient ?
	if (!updateExisting(m_ingredientsFilters, m_ingredientsLayout, action, id))
		return;

	if (action == FilterActionType::RemoveFilter)
		return;

	// Add this ingredient
	const auto& ingredient = Config::main().ingredients[id];

	auto boxWidget = new QWidget;
	auto boxLayout = new QHBoxLayout(boxWidget);
	boxLayout->setContentsMargins(0, 0, 5, 0);

	auto removeAction = new QAction(boxLayout);
	removeAction->setData(QVariant(id));
	auto removeButton = new QPushButton(QIcon("://icons/Delete.png"), "");
	removeButton->setMaximumSize(24, 24);
	removeButton->setFlat(true);
	removeButton->setToolTip(tr("Remove this filter"));
	connect(removeButton, SIGNAL(clicked(bool)), removeAction, SLOT(trigger()));
	connect(removeAction, SIGNAL(triggered(bool)), this, SLOT(removeIngredient()));
	boxLayout->addWidget(removeButton);

	auto label = new QLabel((action == FilterActionType::addFilterContains ? "Has " : "Does not have ") + ingredient.name);
	boxLayout->addWidget(label);

	m_ingredientsLayout->addWidget(boxWidget);

	m_ingredientsFilters.emplace_back(action, id, boxWidget);

	updatePotionsListFilters();
}

void FiltersWidget::removeWidget(FlowLayout *layout, QWidget* widget)
{
	int nb = layout->count();
	for (int i = 0; i < nb; ++i)
	{
		auto item = layout->itemAt(i);
		if (item && item->widget() == widget)
		{
			auto item = layout->takeAt(i);
			auto w = item->widget();
			if (w) delete w;
			delete item;
			layout->update();
			return;
		}
	}
}

void FiltersWidget::removeEffect()
{
	QAction* action = qobject_cast<QAction*>(sender());
	if (action)
	{
		bool ok = false;
		int id = action->data().toInt(&ok);
		if (ok)
		{
			auto it = std::find_if(m_effectsFilters.begin(), m_effectsFilters.end(), [id](const FilterItem& item){
				return item.id == id;
			});
			if (it != m_effectsFilters.end())
			{
				removeWidget(m_effectsLayout, it->widget);
				m_effectsFilters.erase(it);
				updatePotionsListFilters();
			}
			emit effectFilterRemoved(id);
		}
	}
}

void FiltersWidget::removeIngredient()
{
	QAction* action = qobject_cast<QAction*>(sender());
	if (action)
	{
		bool ok = false;
		int id = action->data().toInt(&ok);
		if (ok)
		{
			auto it = std::find_if(m_ingredientsFilters.begin(), m_ingredientsFilters.end(), [id](const FilterItem& item){
				return item.id == id;
			});
			if (it != m_ingredientsFilters.end())
			{
				removeWidget(m_ingredientsLayout, it->widget);
				m_ingredientsFilters.erase(it);
				updatePotionsListFilters();
			}
			emit ingredientFilterRemoved(id);
		}
	}
}

void FiltersWidget::updatePotionsListFilters()
{
	PotionsList::Filters filters;
	using FilterType = PotionsList::Filter::FilterType;
	for (const auto& ing : m_ingredientsFilters)
	{
		bool contains = (ing.actionType == FilterActionType::addFilterContains);
		filters.emplace_back(contains ? FilterType::HasIngredient : FilterType::DoesNotHaveIngredient, ing.id);
	}

	for (const auto& effect : m_effectsFilters)
	{
		bool contains = (effect.actionType == FilterActionType::addFilterContains);
		filters.emplace_back(contains ? FilterType::HasEffect : FilterType::DoesNotHaveEffect, effect.id);
	}

	if (m_inventoryCheckBox->checkState() == Qt::Checked)
		filters.emplace_back(FilterType::AvailableIngredients);

	auto ingCountChoice = m_ingredientsCountComboBox->currentIndex();
	if (ingCountChoice == 1)
		filters.emplace_back(FilterType::TwoIngredients);
	else if (ingCountChoice == 2)
		filters.emplace_back(FilterType::ThreeIngredients);

	auto purityChoice = m_purityComboBox->currentIndex();
	if (purityChoice == 1)
		filters.emplace_back(FilterType::Pure);
	else if (purityChoice == 2)
		filters.emplace_back(FilterType::PurePositive);
	else if (purityChoice == 3)
		filters.emplace_back(FilterType::PureNegative);

	auto& potionsList = PotionsList::instance();
	potionsList.setFilters(filters);
	potionsList.applyFilters();
	potionsList.sortPotions();
	emit modified();
}
