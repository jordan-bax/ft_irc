#include <iostream>

enum	e_hand_sign  {
	rock,
	paper,
	scissor
};

static const char *g_hand_sign[] = {
	"rock","paper", "scissor" , 0};

static const char *g_hand_sign_art[] = {
	"    _______\n"
	"---'   ____)\n"
	"      (_____)\n"
	"      (_____)\n"
	"      (____)\n"
	"---.__(___)\n"
	,
	"    _______\n"
	"---'   ____)____\n"
	"          ______)\n"
	"          _______)\n"
	"         _______)\n"
	"---.__________)\n"
	,
	"    _______\n"
	"---'   ____)____\n"
	"          ______)\n"
	"       __________)\n"
	"      (____)\n"
	"---.__(___)\n",
	0
};
