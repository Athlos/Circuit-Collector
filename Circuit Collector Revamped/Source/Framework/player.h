#ifndef __PLAYER_H__
#define __PLAYER_H__

//Forward declarations
class Label;
class BackBuffer;

//Class
class Player
{
	//Member methods:
public:
	Player();
	~Player();

	// Drawing
	void drawPlayerUI(BackBuffer& backBuffer);
	void drawPlayer(BackBuffer& backBuffer);

	// Processing
	void Process(float deltaTime);

	// Updating
	void UpdateCurrentHealth(int amount);
	void UpdateMaximumHealth(int amount);
	void UpdateHealthMultiplier(float amount);
	void UpdateCurrency(int amount);

private:
	// Label handling
	void InitialiseLabels();
	void UpdateHealthLabel();
	void UpdateCurrencyLabel();

	//Member data:
public:
protected:
	int m_currentHealth; // Current health of the player
	int m_currentMaxHealth; // Current max health the player can have
	int m_baseMaxHealth; // Base max health used to calculate current max using multiplier
	float m_healthMultiplier; // Multiplier to max health, used with base max health

	int m_currency; // Currency the player has to spend

private:

	//UI Elements
	Label* m_healthLabel; // Label for health, formatted: Health: 100/100
	Label* m_currencyLabel; // Label for currency, formatted: Electricity: 100

};

#endif //__PLAYER_H__