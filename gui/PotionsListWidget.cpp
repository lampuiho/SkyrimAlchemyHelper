#include <QtWidgets>

#include "PotionsListWidget.h"

#include "EffectsList.h"
#include "IngredientsList.h"
#include "PotionsList.h"

PotionsListWidget::PotionsListWidget(QWidget* parent)
	: QFrame(parent)
{
	setFrameShape(QFrame::StyledPanel);
	PotionsList::instance().setUpdateCallback([this]() { refreshList(); });
	refreshList();
}

void PotionsListWidget::refreshList()
{
	auto l = layout();
	if (l)
		QWidget().setLayout(l);

	const auto& ingredients = IngredientsList::instance().ingredients();
	const auto& effects = EffectsList::instance().effects();
	const auto& potions = PotionsList::instance().filteredPotions();
	int nbPotions = potions.size();

	QVBoxLayout* vLayout = new QVBoxLayout;
	auto label = new QLabel("# of potions : " + QString::number(potions.size()));
	vLayout->addWidget(label);

	for (int i = 0, nb = std::min(50, nbPotions); i < nb; ++i)
	{
		const auto& potion = potions[i];
		auto potionWidget = new QFrame;
		potionWidget->setFrameShape(QFrame::Box);
		auto potionLayout = new QHBoxLayout(potionWidget);

		auto ingredientsLayout = new QVBoxLayout;
		for (int ingId = 0; ingId < PotionsList::maxIngredientsPerPotion
			&& potion.ingredients[ingId] != -1; ++ingId)
		{
			auto ingLabel = new QLabel(ingredients[potion.ingredients[ingId]].name);
			ingredientsLayout->addWidget(ingLabel);
		}
	//	ingredientsLayout->addStretch();
		potionLayout->addLayout(ingredientsLayout);

		auto effectsLayout = new QVBoxLayout;
		auto strengthLayout = new QVBoxLayout;
		for (int effId = 0; effId < PotionsList::maxEffectsPerPotion
			&& potion.effects[effId] != -1; ++effId)
		{
			auto effLabel = new QLabel(effects[potion.effects[effId]].name);
			effectsLayout->addWidget(effLabel);

			QString strengthText;
			if (potion.durations[effId] > 0)
				strengthText = QString("%1 in %2 sec").arg(potion.magnitudes[effId], 0, 'f', 2).arg(potion.durations[effId], 0, 'f', 1);
			else
				strengthText = QString::number(potion.magnitudes[effId], 'f', 2);
			strengthLayout->addWidget(new QLabel(strengthText));
		}
	//	effectsLayout->addStretch();
		potionLayout->addLayout(effectsLayout);
		potionLayout->addLayout(strengthLayout);

		vLayout->addWidget(potionWidget);
	}

	vLayout->addStretch();

	setLayout(vLayout);
}