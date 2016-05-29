#ifndef _FIELD_H_
#define _FIELD_H_

#include "service.h"

class Field
{
private:
	// floor tile texture
	HTEXTURE h_floorTileTex;

	// cursor texture
	HTEXTURE h_cursorTex;
	
	// background tile texture
	HTEXTURE h_backgroundTex;

	// tower texture
	HTEXTURE h_towerTex;

	// princess texture
	HTEXTURE h_princessTex;

	// left wall texture
	HTEXTURE h_wallLeftTex;

	// right wall texture
	HTEXTURE h_wallRightTex;

	// hole texture
	HTEXTURE h_holeTex;

	// cell selection texture
	HTEXTURE h_cellSelectionTex;

	// hero's portrait texture
	HTEXTURE h_portraitTex;

	// corner pattern texture
	HTEXTURE h_patternTex;
	
	// unique pointer on the floor tile sprite
	std::unique_ptr<hgeSprite> up_floorTile;

	// unique pointer on the cursor sprite
	std::unique_ptr<hgeSprite> up_cursor;

	// unique pointer on the background tile sprite
	std::unique_ptr<hgeSprite> up_background;

	// unique pointer on the tower sprite
	std::unique_ptr<hgeSprite> up_tower;

	// unique pointer on the princess sprite
	std::unique_ptr<hgeSprite> up_princess;

	// unique pointer on the left wall sprite
	std::unique_ptr<hgeSprite> up_wallLeft;

	// unique pointer on the right wall sprite
	std::unique_ptr<hgeSprite> up_wallRight;

	// unique pointer on the hole sprite
	std::unique_ptr<hgeSprite> up_hole;

	// unique pointer on the cell selection animation
	std::unique_ptr<hgeAnimation> up_cellSelectionAni;

	// unique pointer on the hero's portrait
	std::unique_ptr<hgeSprite> up_heroPicture;

	// unique pointer on the corner pattern
	std::unique_ptr<hgeSprite> up_pattern;

	// unique pointer on the font 
	std::unique_ptr<hgeFont> up_font;

	// X selection coordinate
	float f_xSelection;

	// Y selection coordinate
	float f_ySelection;

	// X cursor coordinate
	float f_xCursor; 

	// Y cursor coordinate
	float f_yCursor;

	// X right mouse click coordinate
	float f_xRightClick;

	// Y left mouse click coordinate
	float f_yRightClick;

	// Prepares field sources
	bool prepareSources();

	// Frees all sources
	void release();

	// Renders background
	void renderBackground();

	// Renders tower
	void renderTower();

	// Renders princess
	void renderPrincess();

	// Renders walls
	void renderWalls();
		
	// Renders field
	void renderField();

	// Renders holes
	void renderHoles();

	// Renders cell selection
	void renderCellSelection();

	// Renders hero's portrait
	void renderPortrait();

	// Renders corners 
	void renderPattern();

	// Renders game statistics
	void renderGameStatistics();
	
	// Container with all holes
	std::set<std::pair<float,float>> s_holes;

	// Private constructor
	Field();
	
	// Field intstance
	static Field* instance;

	// Validates hole instalation
	bool validateHoleInstalation(std::pair<float, float> instalationPoint);

	// Calculates cell centers of the field
	void calculateCells();
public:
	// Returns an instance of the Field object
	static Field* getInstance();

	// Destructor
	~Field();

	// Renders scene
	void renderScene();

	// Inserts the hole into the set
	void insertHole();

	// Get set of holes
	std::set<std::pair<float, float>>& getHoles(); 

	// Renders cursor
	void renderCursor();

	// Sets selection coordinates
	void setSelection(float x, float y);

	// Renders strings
	void renderFont(int a, int r, int g, int b, float scale, float x, float y, int align, const std::string& str);

	// Gets selected cell center
	std::pair<float,float> Field::getSelectedCellCenter(float x, float y);
};

inline void Field::setSelection(float x, float y)
{
	f_xSelection = x;
	f_ySelection = y;
}

inline std::set<std::pair<float, float>>& Field::getHoles()
{
	return s_holes;
}
	
#endif