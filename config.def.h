/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 5;        /* border pixel of windows */
static unsigned int snap      = 0;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 20;       /* vert inner gap between windows */
static unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = {"SauceCodePro Nerd Font:size=10:style=Black:antialias=true:autohint=true"};
static char dmenufont[]       = "SauceCodePro Nerd Font Mono:size=10:style=Black:antialias=true:autohint=true";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static const char term[]            = "alacritty";

enum { SchemeNorm, SchemeSel }; /* color schemes */

static char *colors[][3] = {
	       /*               fg           bg           border   */
	       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
		      
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *sptermcmd[] = {term, "--class", "spterm", NULL };
const char *sprangercmd[] = {term, "--class", "spranger", "-e", "ranger", NULL };
const char *spcalccmd[] = {term, "--class", "spcalc", "-e", "calc", "-d", NULL };
const char *spcmuscmd[] = {term, "--class", "spcmus", "cmus", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      sptermcmd},
	{"spranger",    sprangercmd},
	{"spcalc",   	spcalccmd},
	{"spcmus",   	spcmuscmd},
};


/* tagging */
static const char *tags[] = { "1: ", "2: ", "3: ", "4: 爵","5: ", "6: 拾", "7:", "8: ", "9: " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       1 << 5,       0, 		  0,    	0,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0, 		  0, 		0,           -1 },
	{ NULL,       term,       NULL,       1 << 1,       0, 		  1, 		0,           -1 },
	{ "Brave",    NULL,       NULL,       1 << 3,       0, 		  0, 		0,           -1 },
	{ "Teams",    NULL,       NULL,       1 << 5,       0, 		  0, 		0,           -1 },
	{ "Caprine",  NULL,       NULL,       1 << 6,       0, 		  0, 		0,            1 },
	{ "discord",  NULL,       NULL,       1 << 6,       0, 		  0, 		0,            1 },
	{ NULL,   "libreoffice",  NULL,       1 << 4,       0, 		  0, 		0,           -1 },
	{ "Droidcam", NULL,       NULL,       1 << 2,       0, 		  0, 		0,           -1 },
	{ "zoom",     NULL,       NULL,       1 << 5,       0, 		  0, 		0,           -1 },
	{ "Apprun",   NULL,       NULL,       1 << 8,       0, 		  0, 		0,           -1 },
	{ "Godot",    NULL,       NULL,       1 << 7,       0, 		  0, 		0,           -1 },
                                                                                         
	{ NULL,       "spterm",	  NULL,	      SPTAG(0),     1, 		  0, 		0,	    -1 },
	{ NULL,       "spranger", NULL,	      SPTAG(1),     1, 		  0, 		0,	    -1 },
	{ NULL,       "spcalc",   NULL,	      SPTAG(2),     1, 		  0, 		0,	    -1 },
	{ NULL,       "spcmus",   NULL,	      SPTAG(3),     1, 		  0, 		0,	    -1 },

	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { term, NULL };
static const char *browsercmd[]  = { "brave", NULL };
static const char *fmcmd[]  = { term, "-e", "ranger", NULL };
static const char *languagecmd[]  = { "language.sh", "1", NULL };

static const char *incrvolcmd[]  = { "/mnt/data/SharedFiles/Documents/Sources/Suckless/dwmblocks/blocks/volume.sh", "+", NULL };
static const char *decrvolcmd[]  = { "/mnt/data/SharedFiles/Documents/Sources/Suckless/dwmblocks/blocks/volume.sh", "-", NULL };
static const char *mutevolcmd[]  = { "/mnt/data/SharedFiles/Documents/Sources/Suckless/dwmblocks/blocks/volume.sh", "3", NULL };

static const char *mediatogglecmd[]= { "playerctl", "play-pause", NULL };
static const char *mediapausecmd[] = { "playerctl", "pause", NULL };
static const char *mediaearlycmd[] = { "playerctl", "previous", NULL };
static const char *medianextcmd[]  = { "playerctl", "next", NULL };


static const char *scrotalltolcmd[]  = { "scrot", "/mnt/data/SharedFiles/Pictures/%b%d::%H%M%S.png", NULL };
static const char *scrotalltoccmd[]  = { "scrot","/tmp/temp.png", "-e", "xclip -selection clipboard -t image/png -i $f", NULL };
static const char *scrotpartolcmd[]  = { "scrot", "/mnt/data/SharedFiles/Pictures/%b%d::%H%M%S.png", "-s",  NULL };
static const char *scrotpartoccmd[]  = { "scrot","/tmp/temp.png", "-s", "-e", "xclip -selection clipboard -t image/png -i $f", NULL };

/*
 * Xresources preferences to load at startup
 */

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "fonts",               STRING,  &fonts },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};


static Key keys[] = {
	/* eifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_r,      spawn,          {.v = fmcmd } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = languagecmd } },
	{ MODKEY, 	                XK_F4,      spawn,          {.v = languagecmd } },

	{ 0,        	XF86XK_AudioRaiseVolume,   spawn,          {.v = incrvolcmd } },
	{ 0,        	XF86XK_AudioLowerVolume,   spawn,          {.v = decrvolcmd } },
	{ 0,        	XF86XK_AudioMute,  	   spawn,          {.v = mutevolcmd } },


	{ 0,        	XF86XK_AudioPlay,   spawn,          {.v = mediatogglecmd } },
	{ 0,        	XF86XK_AudioStop,  spawn,          {.v = mediapausecmd } },
	{ 0,        	XF86XK_AudioPrev,   spawn,          {.v = mediaearlycmd } },
	{ 0,        	XF86XK_AudioNext,   spawn,          {.v = medianextcmd } },
	
	{ 0,	                        XK_Print,  spawn,          {.v = scrotalltolcmd } },
	{ ShiftMask,                    XK_Print,  spawn,          {.v = scrotpartolcmd } },
	{ ShiftMask|ControlMask,        XK_Print,  spawn,          {.v = scrotpartoccmd } },
	{ ControlMask,                  XK_Print,  spawn,          {.v = scrotalltoccmd } },

	{ MODKEY,                       XK_t,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_f,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_m,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_m,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_k,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_j,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = 0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
/*	{ NULL,            	        NULL,      incrgaps,       {.i = +1 } },
	{ NULL,                       NULL,      incrgaps,       {.i = -1 } },
	{ NULL,	                NULL,      incrigaps,      {.i = +1 } },
	{ NULL,  		        NULL,      incrigaps,      {.i = -1 } },
	{ NULL,         	        NULL,      incrogaps,      {.i = +1 } },
	{ NULL,                       NULL,      incrogaps,      {.i = -1 } },
	{ NULL,                       NULL,      incrihgaps,     {.i = +1 } },
	{ NULL,                       NULL,      incrihgaps,     {.i = -1 } },
	{ NULL,                       NULL,      incrivgaps,     {.i = +1 } },
	{ NULL,                       NULL,      incrivgaps,     {.i = -1 } },
	{ NULL,                       NULL,      incrohgaps,     {.i = +1 } },
	{ NULL,  		        NULL,      incrohgaps,     {.i = -1 } },
	{ NULL,      		        NULL,      incrovgaps,     {.i = +1 } },
	{ NULL,		        NULL,      incrovgaps,     {.i = -1 } },*/
	{ MODKEY,  	                XK_g,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,                       XK_g,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[13]}},
	{ MODKEY|ShiftMask,                       XK_y,      setlayout,      {.v = &layouts[11]} },
	{ MODKEY|ShiftMask,                       XK_u,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,                       XK_i,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY|ShiftMask,                       XK_o,      setlayout,      {.v = &layouts[13]}},

	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	{ MODKEY|ShiftMask,       	XK_t,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY|ShiftMask,            	XK_r,	   togglescratch,  {.ui = 1 } },
	{ MODKEY, 	           	XK_F11,	   togglescratch,  {.ui = 2 } },
	{ MODKEY, 	           	XK_F5,	   togglescratch,  {.ui = 3 } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

