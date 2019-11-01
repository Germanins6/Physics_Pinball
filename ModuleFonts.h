#include "Module.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct Font
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};

class ModuleFonts : public Module
{
public:
	ModuleFonts(Application* app, bool start_enabled = true);
	virtual ~ModuleFonts();

	bool Load(const char* path, const char* characters, uint rows);
	void ModuleFonts::UnLoad(int font_id);
	void BlitText(int x, int y, int font_id, const char* text) const;



private:
	Font	 fonts[MAX_FONTS];
};