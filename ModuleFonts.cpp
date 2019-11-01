#include "ModuleFonts.h"

ModuleFonts::ModuleFonts(Application* app, bool start_enabled) : Module(app, start_enabled){
}

ModuleFonts::~ModuleFonts() {

}

bool ModuleFonts::Load(const char* path, const char* characters, uint rows) {

	int id = -1;

	if (path == nullptr || characters == nullptr || rows == 0) {
		LOG("Couldn't load fonts");
		return id;
	}

	SDL_Texture* tex = App->textures->Load(path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].graphic == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", path, MAX_FONTS);
		return id;
	}

	fonts[id].graphic = tex;
	fonts[id].rows = rows;
	fonts[id].len = strlen(characters);

	for (uint i = 0; i < MAX_FONT_CHARS; i++) {
		fonts[id].table[i] = characters[i];
	}


	uint width, height;
	App->textures->GetSize(tex, width, height);
	fonts[id].row_chars = strlen(characters) / fonts[id].rows;
	fonts[id].char_w = width / fonts[id].row_chars; 
	fonts[id].char_h = height / rows;

	LOG("Successfully loaded BMP font from %s", path);

	return id;

	return true;
}

void ModuleFonts::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		App->textures->Unload(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void ModuleFonts::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].graphic == nullptr)
	{
		//LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect rect;
	uint len = strlen(text);


	rect.w = font->char_w;
	rect.h = font->char_h;
	rect.y = 0;

	for (uint i = 0; i < len; ++i)
	{
		for (uint j = 0; j < fonts[font_id].row_chars; ++j) {
			if (fonts[font_id].table[j] == text[i]) {
				rect.x = j * fonts[font_id].char_w;
				App->renderer->Blit(fonts[font_id].graphic, x, y, &rect, false);
				x += rect.w;
			}
		}

	}
}