/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 0;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {"SauceCodePro Nerd Font:size=10:style=Black:antialias=true:autohint=true"};
static const char dmenufont[]       = "SauceCodePro Nerd Font Mono:size=10:style=Black:antialias=true:autohint=true";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#000000";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#18a8c0";
static const char col1[]            = "#ffffff";//white
static const char col2[]            = "#ffffff";
static const char col3[]            = "#ffffff";
static const char col4[]            = "#ffffff";
static const char col5[]            = "#ffffff";
static const char col6[]            = "#ffffff";

enum { SchemeNorm, SchemeCol1, SchemeCol2, SchemeCol3, SchemeCol4,
       SchemeCol5, SchemeCol6, SchemeSel }; /* color schemes */

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]  = { col_gray3, col_gray1, col_gray2 },
	[SchemeCol1]  = { col1,      col_gray1, col_gray2 },
	[SchemeCol2]  = { col1,      col_cyan,  col_cyan  },
	[SchemeCol3]  = { col3,      col_gray1, col_gray2 },
	[SchemeCol4]  = { col4,      col_gray1, col_gray2 },
	[SchemeCol5]  = { col5,      col_gray1, col_gray2 },
	[SchemeCol6]  = { col6,      col_gray1, col_gray2 },
	[SchemeSel]   = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1: ", "2: ", "3: ", "4: 爵","5: ", "6: 拾", "7:", "8: ", "9: " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ "st",       NULL,       NULL,       1 << 1,       0,           -1 },
	{ "Brave",    NULL,       NULL,       1 << 3,       0,           -1 },
	{ "Teams",    NULL,       NULL,       1 << 5,       0,           -1 },
	{ "Caprine",  NULL,       NULL,       1 << 6,       0,            1 },
	{ "discord",  NULL,       NULL,       1 << 6,       0,            1 },
	{ NULL,   "libreoffice",  NULL,       1 << 4,       0,           -1 },
	{ "Droidcam", NULL,       NULL,       1 << 2,       0,           -1 },
	{ "zoom",     NULL,       NULL,       1 << 5,       0,           -1 },
	{ "Apprun",   NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Godot",    NULL,       NULL,       1 << 7,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "brave", NULL };
static const char *fmcmd[]  = { "st", "-e", "ranger", NULL };
static const char *languagecmd[]  = { "language.sh", "1", NULL };

static const char *incrvolcmd[]  = { "volume.sh", "+", NULL };
static const char *decrvolcmd[]  = { "volume.sh", "-", NULL };
static const char *mutevolcmd[]  = { "volume.sh", "3", NULL };

static const char *mediatogglecmd[]= { "playerctl", "play-pause", NULL };
static const char *mediapausecmd[] = { "playerctl", "pause", NULL };
static const char *mediaearlycmd[] = { "playerctl", "previous", NULL };
static const char *medianextcmd[]  = { "playerctl", "next", NULL };


static const char *scrotalltolcmd[]  = { "scrot", "/mnt/data/SharedFiles/Pictures/%b%d::%H%M%S.png", NULL };
static const char *scrotalltoccmd[]  = { "scrot","/tmp/temp.png", "-e", "xclip -selection clipboard -t image/png -i $f", NULL };
static const char *scrotpartolcmd[]  = { "scrot", "/mnt/data/SharedFiles/Pictures/%b%d::%H%M%S.png", "-s",  NULL };
static const char *scrotpartoccmd[]  = { "scrot","/tmp/temp.png", "-s", "-e", "xclip -selection clipboard -t image/png -i $f", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
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

