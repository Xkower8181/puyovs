#include "Menu.h"
#include "Game.h"
#include "../PVS_ENet/PVS_Client.h"

namespace ppvs
{

Menu::Menu(Game* g)
{
	currentgame = g;
	data = g->data;
	background.setImage(nullptr);
	background.setScale(2 * 192, 336);
	background.setColor(0, 0, 0);
	background.setTransparency(0.5f);
	FieldProp p = currentgame->players[0]->activeField->getProperties();
	background.setPosition(p.offsetX - 192.f / 2.f, p.offsetY - 336.f / 4.f);
	select = 0;

	option[0].setImage(data->imgMenu[0][0]);
	option[1].setImage(data->imgMenu[1][0]);
	option[2].setImage(data->imgMenu[2][0]);
	option[0].setPosition(-200, -200);
	option[1].setPosition(-200, -200);
	option[2].setPosition(-200, -200);
	disableRematch = false;
}

Menu::~Menu()
{
}

void Menu::draw()
{
	background.draw(data->front);
	option[0].draw(data->front);
	option[1].draw(data->front);
	option[2].draw(data->front);

}

void Menu::play()
{
	FieldProp p = currentgame->players[0]->activeField->getProperties();

	// Set option sprite
	for (int i = 0; i < 3; i++)
	{
		// Correct image
		if (select == i)
		{
			// Selected
			option[i].setImage(data->imgMenu[i][1]);
			option[i].setCenter();
			option[i].setScale(option[i].getScaleX() + (1 - option[i].getScaleX()) / 3.f);
		}
		else
		{
			// Not selected
			option[i].setImage(data->imgMenu[i][0]);
			option[i].setCenter();
			option[i].setScale(0.8f);
		}
		// Position
		option[i].setPosition(p.offsetX + p.centerX, p.offsetY + p.centerY / 2 - 60 + i * 40);
		option[i].setColor(255, 255, 255);
	}

	// Enable/disable buttons
	if (!currentgame->settings->useCPUplayers)
	{
		disableRematch = false;
		option[0].setTransparency(1);
		// Disable
		if (currentgame->currentGameStatus == GAMESTATUS_WAITING
			|| currentgame->countBoundPlayers() < 2
			|| currentgame->rankedState > 0)
		{
			disableRematch = true;
			option[0].setTransparency(0.5);
			option[0].setImage(data->imgMenu[0][0]);
		}

	}

	// Set options
	Controller* ctrls = &(currentgame->players[0]->controls);
	if (ctrls->Down == 1 && select < 2)
	{
		select++;
		data->snd.cursor.play(data);
	}
	if (ctrls->Up == 1 && select > 0)
	{
		select--;
		data->snd.cursor.play(data);
	}
	// Choose
	if (ctrls->A == 1)
	{
		ctrls->A++;
		if (!(select == 0 && disableRematch))
			data->snd.decide.play(data);
		if (select == 0)
		{
			// Rematch
			// Offline
			if (currentgame->settings->useCPUplayers)
			{
				currentgame->menuSelect = 0;
				currentgame->resetPlayers();
				currentgame->currentGameStatus = GAMESTATUS_PLAYING;
			}
			else
			{
				if (!disableRematch)
				{
					if (currentgame->connected)
					{
						// Find peers,
						std::string peers = "";

						// Set everyone in channel to active and add them to list
						for (size_t i = 0; i < currentgame->players.size(); i++)
						{
							if (currentgame->players[i]->onlineName != "")
							{
								currentgame->players[i]->active = true;
								peers += "|" + to_string(currentgame->players[i]->onlineID);
							}
						}

						// Propose new random seed
						currentgame->players[0]->proposedRandomSeed = getRandom(1000000);

						// Send rematch message
						// 0[rematch]1[randomseed]2[wins]3....[player ids]
						if (currentgame->channelName.compare("") != 0)
						{
							currentgame->network->sendToChannel(CHANNEL_GAME, "rematch|" + to_string(currentgame->players[0]->proposedRandomSeed) + "|" + to_string(currentgame->players[0]->wins)
								+ peers, currentgame->channelName);
							currentgame->network->sendToServer(CHANNEL_MATCH, "accept");
						}

						// Set state to rematching
						currentgame->currentGameStatus = GAMESTATUS_REMATCHING;

						// Set own rematch value
						currentgame->players[0]->rematch = true;
						currentgame->players[0]->rematchIcon.setVisible(true);
						currentgame->players[0]->rematchIconTimer = 0;

						// Change channel description
						currentgame->sendDescription();

						// Do normal stuff
						currentgame->menuSelect = 0;
						currentgame->loadMusic();
					}
				}
			}
		}
		else if (select == 1)
		{
			// Character select
			currentgame->menuSelect = 1;
			currentgame->charSelectMenu->prepare();
		}
		else if (select == 2)
		{
			// Quit
			if (currentgame->settings->rankedMatch && currentgame->countBoundPlayers() > 1)
			{
				// Cannot close
			}
			else
			{
				currentgame->runGame = false;
			}
		}

		// Reset selection
		select = 0;
	}

	// Disabled
	if (!currentgame->settings->startWithCharacterSelect)
	{
		option[1].setColor(128, 128, 128);
		if (ctrls->Down == 1 && select == 1)
		{
			select++;
		}
		if (ctrls->Up == 1 && select == 1)
		{
			select--;
		}

	}
}

}
