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
	

	// Setters
	void updateCurrentHealth(int amount);
	void updateMaximumHealth(int amount);
	void updateHealthMultiplier(float amount);
	void updateCurrency(int amount);

private:
	void initialiseLabels();

	void updateHealthLabel();
	void updateCurrencyLabel();

	//Member data:
public:
protected:
	int m_currentHealth;
	int m_currentMaxHealth;
	int m_baseMaxHealth;
	float m_healthMultiplier;

	int m_currency;

private:

	//UI Elements
	Label* m_healthLabel;
	Label* m_currencyLabel;

};

#endif //__PLAYER_H__