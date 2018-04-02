#include <assert.h>

#include <libguile.h>
#include <SDL.h>

#include "keytable.h"

static const char *keystr[] = {
	"KEY_0",
	"KEY_1",
	"KEY_2",
	"KEY_3",
	"KEY_4",
	"KEY_5",
	"KEY_6",
	"KEY_7",
	"KEY_8",
	"KEY_9",
	"KEY_a",
	"KEY_AC_BACK",
	"KEY_AC_BOOKMARKS",
	"KEY_AC_FORWARD",
	"KEY_AC_HOME",
	"KEY_AC_REFRESH",
	"KEY_AC_SEARCH",
	"KEY_AC_STOP",
	"KEY_AGAIN",
	"KEY_ALTERASE",
	"KEY_AMPERSAND",
	"KEY_APPLICATION",
	"KEY_ASTERISK",
	"KEY_AT",
	"KEY_AUDIOMUTE",
	"KEY_AUDIONEXT",
	"KEY_AUDIOPLAY",
	"KEY_AUDIOPREV",
	"KEY_AUDIOSTOP",
	"KEY_b",
	"KEY_BACKQUOTE",
	"KEY_BACKSLASH",
	"KEY_BACKSPACE",
	"KEY_BRIGHTNESSDOWN",
	"KEY_BRIGHTNESSUP",
	"KEY_c",
	"KEY_CALCULATOR",
	"KEY_CANCEL",
	"KEY_CAPSLOCK",
	"KEY_CARET",
	"KEY_CLEAR",
	"KEY_CLEARAGAIN",
	"KEY_COLON",
	"KEY_COMMA",
	"KEY_COMPUTER",
	"KEY_COPY",
	"KEY_CRSEL",
	"KEY_CURRENCYSUBUNIT",
	"KEY_CURRENCYUNIT",
	"KEY_CUT",
	"KEY_d",
	"KEY_DECIMALSEPARATOR",
	"KEY_DELETE",
	"KEY_DISPLAYSWITCH",
	"KEY_DOLLAR",
	"KEY_DOWN",
	"KEY_e",
	"KEY_EJECT",
	"KEY_END",
	"KEY_EQUALS",
	"KEY_ESCAPE",
	"KEY_EXCLAIM",
	"KEY_EXECUTE",
	"KEY_EXSEL",
	"KEY_f",
	"KEY_F1",
	"KEY_F10",
	"KEY_F11",
	"KEY_F12",
	"KEY_F13",
	"KEY_F14",
	"KEY_F15",
	"KEY_F16",
	"KEY_F17",
	"KEY_F18",
	"KEY_F19",
	"KEY_F2",
	"KEY_F20",
	"KEY_F21",
	"KEY_F22",
	"KEY_F23",
	"KEY_F24",
	"KEY_F3",
	"KEY_F4",
	"KEY_F5",
	"KEY_F6",
	"KEY_F7",
	"KEY_F8",
	"KEY_F9",
	"KEY_FIND",
	"KEY_g",
	"KEY_GREATER",
	"KEY_h",
	"KEY_HASH",
	"KEY_HELP",
	"KEY_HOME",
	"KEY_i",
	"KEY_INSERT",
	"KEY_j",
	"KEY_k",
	"KEY_KBDILLUMDOWN",
	"KEY_KBDILLUMTOGGLE",
	"KEY_KBDILLUMUP",
	"KEY_KP_0",
	"KEY_KP_00",
	"KEY_KP_000",
	"KEY_KP_1",
	"KEY_KP_2",
	"KEY_KP_3",
	"KEY_KP_4",
	"KEY_KP_5",
	"KEY_KP_6",
	"KEY_KP_7",
	"KEY_KP_8",
	"KEY_KP_9",
	"KEY_KP_A",
	"KEY_KP_AMPERSAND",
	"KEY_KP_AT",
	"KEY_KP_B",
	"KEY_KP_BACKSPACE",
	"KEY_KP_BINARY",
	"KEY_KP_C",
	"KEY_KP_CLEAR",
	"KEY_KP_CLEARENTRY",
	"KEY_KP_COLON",
	"KEY_KP_COMMA",
	"KEY_KP_D",
	"KEY_KP_DBLAMPERSAND",
	"KEY_KP_DBLVERTICALBAR",
	"KEY_KP_DECIMAL",
	"KEY_KP_DIVIDE",
	"KEY_KP_E",
	"KEY_KP_ENTER",
	"KEY_KP_EQUALS",
	"KEY_KP_EQUALSAS400",
	"KEY_KP_EXCLAM",
	"KEY_KP_F",
	"KEY_KP_GREATER",
	"KEY_KP_HASH",
	"KEY_KP_HEXADECIMAL",
	"KEY_KP_LEFTBRACE",
	"KEY_KP_LEFTPAREN",
	"KEY_KP_LESS",
	"KEY_KP_MEMADD",
	"KEY_KP_MEMCLEAR",
	"KEY_KP_MEMDIVIDE",
	"KEY_KP_MEMMULTIPLY",
	"KEY_KP_MEMRECALL",
	"KEY_KP_MEMSTORE",
	"KEY_KP_MEMSUBTRACT",
	"KEY_KP_MINUS",
	"KEY_KP_MULTIPLY",
	"KEY_KP_OCTAL",
	"KEY_KP_PERCENT",
	"KEY_KP_PERIOD",
	"KEY_KP_PLUS",
	"KEY_KP_PLUSMINUS",
	"KEY_KP_POWER",
	"KEY_KP_RIGHTBRACE",
	"KEY_KP_RIGHTPAREN",
	"KEY_KP_SPACE",
	"KEY_KP_TAB",
	"KEY_KP_VERTICALBAR",
	"KEY_KP_XOR",
	"KEY_l",
	"KEY_LALT",
	"KEY_LCTRL",
	"KEY_LEFT",
	"KEY_LEFTBRACKET",
	"KEY_LEFTPAREN",
	"KEY_LESS",
	"KEY_LGUI",
	"KEY_LSHIFT",
	"KEY_m",
	"KEY_MAIL",
	"KEY_MEDIASELECT",
	"KEY_MENU",
	"KEY_MINUS",
	"KEY_MODE",
	"KEY_MUTE",
	"KEY_n",
	"KEY_NUMLOCKCLEAR",
	"KEY_o",
	"KEY_OPER",
	"KEY_OUT",
	"KEY_p",
	"KEY_PAGEDOWN",
	"KEY_PAGEUP",
	"KEY_PASTE",
	"KEY_PAUSE",
	"KEY_PERCENT",
	"KEY_PERIOD",
	"KEY_PLUS",
	"KEY_POWER",
	"KEY_PRINTSCREEN",
	"KEY_PRIOR",
	"KEY_q",
	"KEY_QUESTION",
	"KEY_QUOTE",
	"KEY_QUOTEDBL",
	"KEY_r",
	"KEY_RALT",
	"KEY_RCTRL",
	"KEY_RETURN",
	"KEY_RETURN2",
	"KEY_RGUI",
	"KEY_RIGHT",
	"KEY_RIGHTBRACKET",
	"KEY_RIGHTPAREN",
	"KEY_RSHIFT",
	"KEY_s",
	"KEY_SCROLLLOCK",
	"KEY_SELECT",
	"KEY_SEMICOLON",
	"KEY_SEPARATOR",
	"KEY_SLASH",
	"KEY_SLEEP",
	"KEY_SPACE",
	"KEY_STOP",
	"KEY_SYSREQ",
	"KEY_t",
	"KEY_TAB",
	"KEY_THOUSANDSSEPARATOR",
	"KEY_u",
	"KEY_UNDERSCORE",
	"KEY_UNDO",
	"KEY_UNKNOWN",
	"KEY_UP",
	"KEY_v",
	"KEY_VOLUMEDOWN",
	"KEY_VOLUMEUP",
	"KEY_w",
	"KEY_WWW",
	"KEY_x",
	"KEY_y",
	"KEY_z",
};

static SDL_Keycode keycode[] = {
	SDLK_0,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_5,
	SDLK_6,
	SDLK_7,
	SDLK_8,
	SDLK_9,
	SDLK_a,
	SDLK_AC_BACK,
	SDLK_AC_BOOKMARKS,
	SDLK_AC_FORWARD,
	SDLK_AC_HOME,
	SDLK_AC_REFRESH,
	SDLK_AC_SEARCH,
	SDLK_AC_STOP,
	SDLK_AGAIN,
	SDLK_ALTERASE,
	SDLK_AMPERSAND,
	SDLK_APPLICATION,
	SDLK_ASTERISK,
	SDLK_AT,
	SDLK_AUDIOMUTE,
	SDLK_AUDIONEXT,
	SDLK_AUDIOPLAY,
	SDLK_AUDIOPREV,
	SDLK_AUDIOSTOP,
	SDLK_b,
	SDLK_BACKQUOTE,
	SDLK_BACKSLASH,
	SDLK_BACKSPACE,
	SDLK_BRIGHTNESSDOWN,
	SDLK_BRIGHTNESSUP,
	SDLK_c,
	SDLK_CALCULATOR,
	SDLK_CANCEL,
	SDLK_CAPSLOCK,
	SDLK_CARET,
	SDLK_CLEAR,
	SDLK_CLEARAGAIN,
	SDLK_COLON,
	SDLK_COMMA,
	SDLK_COMPUTER,
	SDLK_COPY,
	SDLK_CRSEL,
	SDLK_CURRENCYSUBUNIT,
	SDLK_CURRENCYUNIT,
	SDLK_CUT,
	SDLK_d,
	SDLK_DECIMALSEPARATOR,
	SDLK_DELETE,
	SDLK_DISPLAYSWITCH,
	SDLK_DOLLAR,
	SDLK_DOWN,
	SDLK_e,
	SDLK_EJECT,
	SDLK_END,
	SDLK_EQUALS,
	SDLK_ESCAPE,
	SDLK_EXCLAIM,
	SDLK_EXECUTE,
	SDLK_EXSEL,
	SDLK_f,
	SDLK_F1,
	SDLK_F10,
	SDLK_F11,
	SDLK_F12,
	SDLK_F13,
	SDLK_F14,
	SDLK_F15,
	SDLK_F16,
	SDLK_F17,
	SDLK_F18,
	SDLK_F19,
	SDLK_F2,
	SDLK_F20,
	SDLK_F21,
	SDLK_F22,
	SDLK_F23,
	SDLK_F24,
	SDLK_F3,
	SDLK_F4,
	SDLK_F5,
	SDLK_F6,
	SDLK_F7,
	SDLK_F8,
	SDLK_F9,
	SDLK_FIND,
	SDLK_g,
	SDLK_GREATER,
	SDLK_h,
	SDLK_HASH,
	SDLK_HELP,
	SDLK_HOME,
	SDLK_i,
	SDLK_INSERT,
	SDLK_j,
	SDLK_k,
	SDLK_KBDILLUMDOWN,
	SDLK_KBDILLUMTOGGLE,
	SDLK_KBDILLUMUP,
	SDLK_KP_0,
	SDLK_KP_00,
	SDLK_KP_000,
	SDLK_KP_1,
	SDLK_KP_2,
	SDLK_KP_3,
	SDLK_KP_4,
	SDLK_KP_5,
	SDLK_KP_6,
	SDLK_KP_7,
	SDLK_KP_8,
	SDLK_KP_9,
	SDLK_KP_A,
	SDLK_KP_AMPERSAND,
	SDLK_KP_AT,
	SDLK_KP_B,
	SDLK_KP_BACKSPACE,
	SDLK_KP_BINARY,
	SDLK_KP_C,
	SDLK_KP_CLEAR,
	SDLK_KP_CLEARENTRY,
	SDLK_KP_COLON,
	SDLK_KP_COMMA,
	SDLK_KP_D,
	SDLK_KP_DBLAMPERSAND,
	SDLK_KP_DBLVERTICALBAR,
	SDLK_KP_DECIMAL,
	SDLK_KP_DIVIDE,
	SDLK_KP_E,
	SDLK_KP_ENTER,
	SDLK_KP_EQUALS,
	SDLK_KP_EQUALSAS400,
	SDLK_KP_EXCLAM,
	SDLK_KP_F,
	SDLK_KP_GREATER,
	SDLK_KP_HASH,
	SDLK_KP_HEXADECIMAL,
	SDLK_KP_LEFTBRACE,
	SDLK_KP_LEFTPAREN,
	SDLK_KP_LESS,
	SDLK_KP_MEMADD,
	SDLK_KP_MEMCLEAR,
	SDLK_KP_MEMDIVIDE,
	SDLK_KP_MEMMULTIPLY,
	SDLK_KP_MEMRECALL,
	SDLK_KP_MEMSTORE,
	SDLK_KP_MEMSUBTRACT,
	SDLK_KP_MINUS,
	SDLK_KP_MULTIPLY,
	SDLK_KP_OCTAL,
	SDLK_KP_PERCENT,
	SDLK_KP_PERIOD,
	SDLK_KP_PLUS,
	SDLK_KP_PLUSMINUS,
	SDLK_KP_POWER,
	SDLK_KP_RIGHTBRACE,
	SDLK_KP_RIGHTPAREN,
	SDLK_KP_SPACE,
	SDLK_KP_TAB,
	SDLK_KP_VERTICALBAR,
	SDLK_KP_XOR,
	SDLK_l,
	SDLK_LALT,
	SDLK_LCTRL,
	SDLK_LEFT,
	SDLK_LEFTBRACKET,
	SDLK_LEFTPAREN,
	SDLK_LESS,
	SDLK_LGUI,
	SDLK_LSHIFT,
	SDLK_m,
	SDLK_MAIL,
	SDLK_MEDIASELECT,
	SDLK_MENU,
	SDLK_MINUS,
	SDLK_MODE,
	SDLK_MUTE,
	SDLK_n,
	SDLK_NUMLOCKCLEAR,
	SDLK_o,
	SDLK_OPER,
	SDLK_OUT,
	SDLK_p,
	SDLK_PAGEDOWN,
	SDLK_PAGEUP,
	SDLK_PASTE,
	SDLK_PAUSE,
	SDLK_PERCENT,
	SDLK_PERIOD,
	SDLK_PLUS,
	SDLK_POWER,
	SDLK_PRINTSCREEN,
	SDLK_PRIOR,
	SDLK_q,
	SDLK_QUESTION,
	SDLK_QUOTE,
	SDLK_QUOTEDBL,
	SDLK_r,
	SDLK_RALT,
	SDLK_RCTRL,
	SDLK_RETURN,
	SDLK_RETURN2,
	SDLK_RGUI,
	SDLK_RIGHT,
	SDLK_RIGHTBRACKET,
	SDLK_RIGHTPAREN,
	SDLK_RSHIFT,
	SDLK_s,
	SDLK_SCROLLLOCK,
	SDLK_SELECT,
	SDLK_SEMICOLON,
	SDLK_SEPARATOR,
	SDLK_SLASH,
	SDLK_SLEEP,
	SDLK_SPACE,
	SDLK_STOP,
	SDLK_SYSREQ,
	SDLK_t,
	SDLK_TAB,
	SDLK_THOUSANDSSEPARATOR,
	SDLK_u,
	SDLK_UNDERSCORE,
	SDLK_UNDO,
	SDLK_UNKNOWN,
	SDLK_UP,
	SDLK_v,
	SDLK_VOLUMEDOWN,
	SDLK_VOLUMEUP,
	SDLK_w,
	SDLK_WWW,
	SDLK_x,
	SDLK_y,
	SDLK_z,
};

static uint32_t mousecode[] = {
	SDL_BUTTON(SDL_BUTTON_LEFT),
	SDL_BUTTON(SDL_BUTTON_MIDDLE),
	SDL_BUTTON(SDL_BUTTON_RIGHT),
};

static const char *mousestr[] = {
	"MOUSE_LEFT",
	"MOUSE_MIDDLE",
	"MOUSE_RIGHT",
};

void init_keytable(){
	int size = sizeof(keystr) / sizeof(const char*);
	for(int i = 0; i < size; ++i){
		const char *str = keystr[i];
		SDL_Keycode code = keycode[i];
		SCM scm_code = scm_from_uint8(code);
		scm_c_define(str, scm_code);
		scm_c_export(str, NULL);
	}

	size = sizeof(mousestr) / sizeof(const char*);
	for(int i = 0; i < size; ++i){
		const char *str = mousestr[i];
		uint32_t mbutton = mousecode[i];
		SCM scm_mbutton = scm_from_uint32(mbutton);
		scm_c_define(str, scm_mbutton);
		scm_c_export(str, NULL);
	}
}