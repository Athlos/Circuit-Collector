#include "player.h"
#include "label.h"

// Constructor
Player::Player()
	: m_baseMaxHealth(100)
	, m_currentHealth(100)
	, m_healthMultiplier(1.0f)
	, m_currency(100)
{
	m_currentMaxHealth = (int)(m_baseMaxHealth * m_healthMultiplier);
	InitialiseLabels();
}

// Destructor
Player::~Player()
{
	delete m_currencyLabel;
	m_currencyLabel = 0;

	delete m_healthLabel;
	m_healthLabel = 0;
}

/*
Draw the player ui: health, currency
*/
void Player::drawPlayerUI(BackBuffer& backBuffer)
{
	m_currencyLabel->Draw(backBuffer);
	m_healthLabel->Draw(backBuffer);
}

/*
Draw the player model
*/
void Player::drawPlayer(BackBuffer& backBuffer)
{

}

/*
Adds amount to current health
*/
void Player::UpdateCurrentHealth(int amount)
{
	if (amount != 0)
	{
		m_currentHealth += amount;

		if (m_currentHealth < 0)
		{
			m_currentHealth = 0;
		}

		if (m_currentMaxHealth < m_currentHealth)
		{
			m_currentHealth = m_currentMaxHealth;
		}

		// Update labels
		UpdateHealthLabel();
	}
}

/*
Adds amount to max health
*/
void Player::UpdateMaximumHealth(int amount)
{
	if (amount != 0)
	{
		m_baseMaxHealth += amount;

		if (m_baseMaxHealth <= 0)
		{
			m_baseMaxHealth = 1;
		}

		m_currentMaxHealth = (int)(m_baseMaxHealth * m_healthMultiplier);

		if (m_currentMaxHealth < m_currentHealth)
		{
			m_currentHealth = m_currentMaxHealth;
		}

		// Update labels
		UpdateHealthLabel();
	}
}

/*
Adds amount to health multiplier
*/
void Player::UpdateHealthMultiplier(float amount)
{
	if (amount != 0)
	{
		m_healthMultiplier += amount;

		if (m_healthMultiplier <= 0)
		{
			m_healthMultiplier = 0.01f;
		}

		m_currentMaxHealth = (int)(m_baseMaxHealth * m_healthMultiplier);

		// Update labels
		UpdateHealthLabel();
	}
}

/*
Adds amount to currency
*/
void Player::UpdateCurrency(int amount)
{
	if (amount != 0)
	{
		m_currency += amount;

		if (m_currency < 0)
		{
			m_currency = 0;
		}

		// Update labels
		UpdateCurrencyLabel();
	}
}

/*
Initialise the UI labels
*/
void Player::InitialiseLabels()
{
	std::string healthString = "Health: " + std::to_string(m_currentHealth) + "/" + std::to_string(m_currentMaxHealth);
	m_healthLabel = new Label(healthString);
	m_healthLabel->SetBounds(0, 32, 256, 32);
	m_healthLabel->SetColour(255, 0, 0, 50);

	std::string electricityString = "Electricity: " + std::to_string(m_currency);
	m_currencyLabel = new Label(electricityString);
	m_currencyLabel->SetBounds(0, 68, 256, 32);
	m_currencyLabel->SetColour(216, 191, 0, 50);
}

/*
Update health label info
*/
void Player::UpdateHealthLabel()
{
	std::string healthString = "Health: " + std::to_string(m_currentHealth) + "/" + std::to_string(m_currentMaxHealth);
	m_healthLabel->SetText(healthString);
}

/*
Update currency label info
*/
void Player::UpdateCurrencyLabel()
{
	std::string currencyString = "Electricity: " + std::to_string(m_currency);
	m_currencyLabel->SetText(currencyString);
}