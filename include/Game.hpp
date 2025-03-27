#ifndef GAME_HPP
#define GAME_HPP

#include "../include/traffic-orchestrator.hpp"


class Game {
	public:

		std::vector<std::unique_ptr<Node>>	nodes;
		std::vector<std::unique_ptr<Road>>	roads;
		std::vector<std::string>	filenames;		// * in maps/
		std::string					openFilename;
		std::string					lastOpenFilename;	// * in config.txt
		Config		config;
		Ui			ui;
		Camera2D	camera;
		int			screenWidth;	// todo: dans config
		int			screenHeight;	// todo: dans config
		int			frameCounter;
		int			mode;	// * M_NONE, M_CREATE_ROAD, M_MAP_OPEN,...
		IPoint		mouse;	// * Position de la souris	// TODO use vector
		IPoint		cursor;	// * Position de la cureur arrondi a 100	// in world !!!	// TODO use vector + rename worldPosition
		Font		fontUI;
		Node		*newNodeStart;	// * Nouveau noeud de départ quand on cree une route
		int			selectedRoadId;	// * null == -1
		int			selectedNodeId;	// * null == -1
		int			hoveredRoadId;	// * null == -1
		int			hoveredNodeId;	// * null == -1
		float		timeScale;	// ! unused	// (1sec in game = timeScale sec in real)
		int			minDistBetweenCarsInLane;	// * in px
		// * UI:
		bool		showHelp;
		bool		showDebug;
		bool		showGridCoords;
		bool		showRoadsInfos;
		bool		asUnsavedChanges;

		// TODO:
		// float		scale;	// (1px = scale m)
		// bool			showGrid;
		// Rectangle	fullArea;	// * contains all nodes

		// * CONSTRUCTORS:
		Game(float scale, float timeScale);

		// * METHODS:
		void	toggleAddRoadMode();
		void	toggleEditRoadMode();
		void	toggleEditNodeMode();
		void	updateMouseAndCursor();
		void	handleEvents(UIPanelRoad &uiPanelRoad);
		bool	nodeExists(int x, int y);
		bool	roadExists(Node &nStart, Node &nEnd);
		Node*	findNode(int x, int y);
		void	createRoad(Node &node1, Node &node2, int lanesCount, int speedLimit);
		void	createNodesAndRoad(Node nodeS, Node nodeE, UIPanelRoad &uiPanelRoad);
		void	addCarsOnLane(Lane &lane, int count, bool atStart, Color color);
		void	centerCameraOnNodes();
		void	printMapInfos();

		// * Game_draw.cpp:
		void	drawMap();
		void	drawGrid();
		void	drawMapSelector();
		void	drawDebug();
		void	drawHelpPanel();
		// * Game_open_save_map.cpp:
		void	generateFromFile(const std::string& filename);
		void	newMapClick();
		void	openMapClick();
		bool	saveMapToFile(const std::string& filename);
		bool	saveMapClick();
		bool	saveMapAsClick();
		// * Game_ui.cpp:
		void	initUi();
		void	drawUIPanelRoad(UIPanelRoad &ui);
		void	drawUI();
		// * Game_update.cpp
		void	update();
		void	updateCarAtEndOfRoad(std::vector<Road*> &nextPossibleRoads, Lane &lane, int carId);

	private:
		void	handleZoom(float wheel);

};

#endif
