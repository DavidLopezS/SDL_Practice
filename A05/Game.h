#pragma once

/**
* Project Untitled
*/


#ifndef _GAME_H
#define _GAME_H

class Game {
private:
	//void GameState : Gamestate(MENU, PLAY, EXIT);
	enum Gamestate
	{
		MENU, PLAY, EXIT
	};
	//	void GameState:Gamestate(MENU, PLAY,EXIT);
	enum SceneState 
	{
		RUNNING, EXIT
	};
public:

};

#endif //_GAME_H
