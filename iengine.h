/**
 * @file iengine.h
 * @brief The interface the game uses to access the engine.
 *
 * The file contains functions which comprise the API for writing games. This
 * header file is not called by the game internally but should be included in
 * the game code to expose functions to the game.
 */

#ifndef IENGINE_H_
#define IENGINE_H_

//forward object declarations (used in some functions below)
struct DecalSlot;
struct VSlot;
struct Texture;
struct editinfo;
class model;
struct undoblock;
struct vslotmap;
struct prefab;

// ragdoll

 /**
  * @brief Update a ragdoll's position and physics.
  *
  * If the dynent pointed to does not have a valid ragdoll object as a member,
  * nothing happens.
  * @param d a dynent pointer.
  */
extern void moveragdoll(dynent *d);

/**
 * @brief Deletes the ragdoll associated with the passed dynent.
 *
 * The function does nothinf if the dynent does not point to a ragdoll object.
 * Also, the function does nothing if the dynent is a ragdoll that is not
 * present.
 * @param d a dynent pointer.
 */
extern void cleanragdoll(dynent *d);

// glemu

namespace gle
{
    extern void begin(GLenum mode);
    extern void begin(GLenum mode, int numverts);
    extern void attrib(const vec &v);
    extern void attribf(float x, float y);
    extern void colorf(float x, float y, float z, float w = 0.0f);
    extern void colorub(uchar x, uchar y, uchar z, uchar w = 255);
    extern void deftexcoord0(int size = 2, int format = GL_FLOAT);
    extern void defvertex(int size = 3, int format = GL_FLOAT);
    extern int end();
}
/*==============================================================================*\
 * Interface Functions & Values                                                 *
 *                                                                              *
 * Input, Scripting, Sound, UI                                                  *
 *                                                                              *
 * command.cpp                                                                  *
 * console.cpp                                                                  *
 * control.cpp                                                                  *
 * cubestd.cpp                                                                  *
 * input.cpp                                                                    *
 * menus.cpp                                                                    *
 * textedit.cpp                                                                 *
 * ui.cpp                                                                       *
\*==============================================================================*/

// command

/* see command.h */

/**
 * @brief Initializes core cubescript commands.
 *
 * `defvar`
 *
 * `defvarp`
 *
 * `deffvarp`
 *
 * `defsvar`
 *
 * `defsvarp`
 *
 * `getvarmin`
 *
 * `getfvarmin`
 *
 * `getfvarmax`
 *
 * `identexists`
 *
 * `getalias`
 *
 * `nodebug`
 *
 * `push`
 *
 * `alias`
 *
 * `resetvar`
 */

extern void initcscmds();

// console

/**
 * @brief Prints a message to the ingame console.
 *
 * This function behaves the same way as printf() except it prints to the console
 * instead.
 *
 * @param s the string to print
 * @param ... the names of variables to substitute into the string, in order
 */
extern void conoutf(const char *s, ...) PRINTFARGS(1, 2);

/**
 * @brief Prints a message to the ingame console, with a specific type flag.
 *
 * This function behaves the same way as printf() except it prints to the console
 * instead. It uses a parameter `type` to indicate what type of console message
 * to print. The console flags are defined in consts.h.
 *
 * @param type the flag to modify the console printout with
 * @param s the string to print
 * @param ... the names of variables to substitute into the string, in order
 */
extern void conoutf(int type, const char *s, ...) PRINTFARGS(2, 3);

extern int initing;

/**
 * @brief Clears the console KeyM history hashtable.
 *
 * Empties the KeyM hashtable keyms, typicall used when shutting down the program.
 */
extern void clear_console();

/**
 * @brief Initializes console cubescript commands.
 *
 * `fullconsole`
 *
 * `toggleconsole`
 *
 * `conskip`
 *
 * `miniconskip`
 *
 * `clearconsole`
 *
 * `keymap`
 *
 * `bind`
 *
 * `specbind`
 *
 * `editbind`
 *
 * `getbind`
 *
 * `getspecbind`
 *
 * `geteditbind`
 *
 * `searchbinds`
 *
 * `searchspecbinds`
 *
 * `searcheditbinds`
 *
 * `clearbinds`
 *
 * `clearspecbinds`
 *
 * `cleareditbinds`
 *
 * `clearallbinds`
 *
 * `inputcommand`
 *
 * `saycommand`
 *
 * `history`
 *
 * `onrelease`
 *
 * `complete`
 *
 * `listcomplete`
 */

extern void initconsolecmds();

// control
extern int curtime;                     /** current frame time */
extern int lastmillis;                  /** last time */
extern int elapsedtime;                 /** elapsed frame time */
extern int totalmillis;                 /** total elapsed time */
extern FILE *logfile;                   /** the file where the  */

/**
 * @brief Returns the logfile (Linux) or stdout (Windows)
 *
 * Log files are not supported on Windows, so this function returns stdout if
 * on Windows.
 */
FILE *getlogfile();

/**
 * @brief Prints a line to the log file.
 *
 * This function behaves the same way as printf(), except it prints to the
 * output specified in `getlogfile()`.
 *
 * @param fmt The string to print
 * @param ... the names of variables to substitute into the string, in order
 */
void logoutf(const char *fmt, ...);

/**
 * @brief The dynamic entity representing the player.
 */

extern dynent *player;
extern bool inbetweenframes,
            renderedframe;

/**
 * @brief Initializes SDL functions necessary to run a game.
 *
 * Initializes SDL_INIT_TIMER, SDL_INIT_VIDEO, and SDL_INIT_AUDIO.
 */
extern bool initsdl();

/**
 * @brief Immediately shuts down the game and prints error message(s)
 *
 * Shuts down SDL and closes the game immediately. Up to two error messages are
 * printed in a SDL message box after exiting the game.
 *
 * @param s a series of C strings corresponding to error messages
 */
extern void fatal(const char *s, ...) PRINTFARGS(1, 2);

/**
 * @brief Returns the number of milliseconds since the game started.
 *
 * Uses SDL to retrieve the amount of time since the game started.
 */
extern int getclockmillis();

extern int scr_w, scr_h;
extern int desktopw, desktoph;

/**
 * @brief Returns the engine library's build date.
 *
 * Returns the value of __DATE__ macro at the time the engine was built; this
 * value is in the format MMM DD YYYY (e.g. Jan 01 1970). The purpose of the
 * function is to quickly disambiguate how old a (development) build of the engine
 * is.
 *
 * @return a std::string corresponding to the engine's build date
 */
extern std::string enginebuilddate();

/**
 * @brief Returns the engine library's name string
 *
 * Returns the version string for the engine library, which is a compiled-in
 * constant inside the engine library. The purpose of the function is to quickly
 * disambiguate different versions of the library.
 *
 * @return a std::string corresponding to the engine's name string
 */
extern std::string enginestr();

// cubestd

/**
 * @brief Adds math commands to the cubescript interpreter
 */
extern void initmathcmds();

/**
 * @brief Adds string commands to the cubescript interpreter
 *
 * `echo` echoes the arguments passed directly to the console
 *
 * `error`
 *
 * `strstr`
 *
 * `strlen` returns the length of the string passed
 *
 * `strcode`
 *
 * `codestr`
 *
 * `struni`
 *
 * `unistr`
 *
 * `strlower` converts the string to lowercase
 *
 * `strupper` converts the string to uppercase
 *
 * `strsplice`
 *
 * `strreplace`
 *
 * `substr`
 *
 * `stripcolors` strips any color formatting from the argument passed
 *
 * `appendword`
 *
 * `concat` concatenates the strings passed, without inserting spaces between arguments
 *
 * `concatword` concatenates the strings passed, adding a space between each argument
 *
 * `format` nicely formats the string passed
 */
extern void initstrcmds();

/**
 * @brief Adds program control commands to the cubescript interpreter.
 *
 * `exec`
 *
 * `escape`
 *
 * `unescape`
 *
 * `writecfg`
 *
 * `changedvars`
 *
 * `doargs`
 *
 * `if`
 *
 * `?`
 *
 * `pushif`
 *
 * `do`
 *
 * `append`
 *
 * `result`
 *
 * `listlen`
 *
 * `at`
 *
 * `sublist`
 *
 * `listcount`
 *
 * `at`
 *
 * `sublist`
 *
 * `listcount`
 *
 * `listfind`
 *
 * `listfind=`
 *
 * `loop`
 *
 * `loop+`
 *
 * `loop*`
 *
 * `loop+*`
 *
 * `loopconcat`
 *
 * `loopconcat+`
 */
extern void initcontrolcmds();

// input

extern bool grabinput,
            minimized; /**< Set to 1 if the window is minimized. */

extern bool interceptkey(int sym);
extern void inputgrab(bool on);

/**
 * @brief Handles input from SDL.
 *
 * Executes the CubeScript commands associated with any SDL key operations queued.
 *
 * @param map the keymap to use: 0 for default, 1 for edit, 2 for spec
 */
extern void checkinput(int map = 0);

/**
 * @brief Flushes all queued mouse events.
 *
 * For all SDL_MOUSEMOTION events currently queued in SDL's event queue, this
 * function dumps them one by one while doing nothing. This is used to clear
 * any mouse events which may happen while the game is starting up, to ensure
 * the cursor spawns in the middle of the screen.
 */
extern void ignoremousemotion();

/**
 * @brief Sets the key repeat mask for SDL.
 *
 * If on is true, sets keyrepeatmask to the OR of the existing mask and the passed mask;
 * if on is false, sets keyrepeat mask to the AND of the existing mask and the inverse of the passed mask.
 *
 * The keyrepeat enum is defined in consts.h with the appropriate masks to pass.
 *
 * @param on the mode for the mask to be combined
 * @param mask the bitmask to apply
 */
extern void keyrepeat(bool on, int mask = ~0);


// menus

extern void menuprocess();
extern int mainmenu; /**< toggles if the main menu is shown, bool-like int */

/**
 * Initializes menus CubeScript commands.
 *
 * `applychanges`
 *
 * `pendingchanges`
 */
extern void initmenuscmds();

//textedit

/**
 * @brief Initializes textedit cubescript commands.
 *
 * `textinit`
 *
 * `textlist`
 *
 * `textshow`
 *
 * `textfocus`
 *
 * `textprev`
 *
 * `textmode`
 *
 * `textsave`
 *
 * `textload`
 *
 * `textcopy`
 *
 * `textpaste`
 *
 * `textmark`
 *
 * `textselectall`
 *
 * `textclear`
 *
 * `textcurrentline`
 *
 * `textexec`
 */

extern void inittextcmds();

// UI

namespace UI
{
    /**
     * @brief Queries UI windows' cursor input ability.
     *
     * Returns whether the UI system is accepting cursor input; if any windows in
     * the UI system accept cursor input, returns True.
     *
     * @return true if any UI window is accepting cursor input
     * @return false if no windows exist that accept cursor input
     */
    bool hascursor();

    /**
     * @brief Queries current UI cursor location.
     *
     * Returns to the parameters passed by reference the location of the UI cursor.
     * This location ranges from 0 to 1.f in x and y, with 0,0 being the bottom
     * left part of the screen, and 0.5 0.5 being the middle of the screen.
     * The x and y parameters do not scale by aspect ratio, and 1,1 is always the
     * top right corner of the screen
     *
     * @param x variable to assign x-coordinate of the cursor's location to
     * @param y variable to assign y-coordinate of the cursor's location to
     */
    void getcursorpos(float &x, float &y);

    /**
     * @brief Sets the cursor to the middle of the screen.
     *
     * Sets the location of the UI cursor to the middle of the screen; that is,
     * the cursor's x and y values are both set to 0.5.
     */
    void resetcursor();

    /**
     * @brief Moves the UI cursor by the amount perscribed.
     *
     * Moves the UI cursor by the amount described (positive or negative), scaled
     * by the UI sensitivity variable. Values are capped in both dimensions between
     * 0 and 1, thereby preventing the cursor from completely leaving the screen.
     *
     * @param dx amount to move the cursor in the x direction
     * @param dy amount to move the cursor in the y direction
     */
    bool movecursor(int dx, int dy);

    /**
     * @brief Creates a new UI World object, used globally for UI functionality.
     */
    void setup();

    /**
     * @brief Updates cursor vis a vis the UI elements.
     */
    void update();

    /**
     * @brief Deletes all of the open windows exposed and deletes the UI world object.
     */
    void cleanup();

    /**
     * @brief Shows a UI window with the specified name.
     *
     * If the specified window does not exist or already is shown, no action is
     * taken. Otherwise, displays the window specified.
     *
     * @param name the name of the UI to show
     *
     * @return true if the window was shown
     * @return false if the window was hidden, or if no window exists
     */
    bool showui(const char *name);

    /**
     * @brief Hides an open UI window with the specified name.
     *
     * If the specified window is not shown or does not exist, no action is taken,
     * Otherwise, hides the open window specified
     *
     * @param name the name of the UI window to show
     *
     * @return true if the window was hidden
     * @return false if the window was not previously shown or did not exist
     */
    bool hideui(const char *name);

    /**
     * @brief Toggles the open/closed state of a window.
     *
     * If the speficied window is open, closes the window; if it is closed, opens
     * it. If the window does not exist, does nothing.
     *
     * @param name the name of the window to toggle
     *
     * @return true if the window was shown
     * @return false if the window was hidden, or if no window exists
     */
    bool toggleui(const char *name);

    /**
     * @brief Opens or closes the specified UI.
     *
     * Opens the specified UI if `on` is `true` and closes it if `on` is false;
     * calls either showui() or hideui().
     *
     * @param name the name of the window to hold
     * @param on whether to open or close the specified UI window
     */
    void holdui(const char *name, bool on);

    /**
     * @brief Gets the open/close status of a UI window.
     *
     * Returns whether the specified window is open or closed (displayed onscreen
     * or hidden).
     *
     * @return true if the window is open
     * @return false if the window does not exist or is closed
     */
    bool uivisible(const char *name);

    /**
     * @brief Binds the UI commands to the command parser.
     *
     * Places into the CubeScript interpreter a series of UI commands, required
     * before it is possible to use CubeScript to create UI elements.
     *
     * Commands defined:
     *
     * `showui`
     *
     * `hideui`
     *
     * `hidetopui`
     *
     * `hideallui`
     *
     * `toggleui`
     *
     * `holdui`
     *
     * `uivisiblecmd`
     *
     * `uiname`
     *
     * `uihover`
     *
     * `uihover?`
     *
     * `uihover+`
     *
     * `uihover+?`
     *
     * `uipress`
     *
     * `uipress?`
     *
     * `uipress+`
     *
     * `uipress+?`
     *
     * `uihold`
     *
     * `uihold?`
     *
     * `uihold+`
     *
     * `uihold+?`
     *
     * `uirelease`
     *
     * `uirelease?`
     *
     * `uirelease+`
     *
     * `uirelease+?`
     *
     * `uialthold`
     *
     * `uialthold?`
     *
     * `uialthold+`
     *
     * `uialthold+?`
     *
     * `uialtpress`
     *
     * `uialtpress?`
     *
     * `uialtpress+`
     *
     * `uialtpress+?`
     *
     * `uialtrelease`
     *
     * `uialtrelease?`
     *
     * `uialtrelease+`
     *
     * `uialtrelease+?`
     *
     * `uieschold`
     *
     * `uieschold?`
     *
     * `uieschold+`
     *
     * `uieschold+?`
     *
     * `uiescpress`
     *
     * `uiescpress?`
     *
     * `uiescpress+`
     *
     * `uiescpress+?`
     *
     * `uiescrelease`
     *
     * `uiescrelease?`
     *
     * `uiescrelease+`
     *
     * `uiescrelease+?`
     *
     * `uiscrollup`
     *
     * `uiscrollup?`
     *
     * `uiscrollup+`
     *
     * `uiscrollup+?`
     *
     * `uiscrolldown`
     *
     * `uiscrolldown?`
     *
     * `uiscrolldown+`
     *
     * `uiscrolldown+?`
     *
     * `uifocus`
     *
     * `uifocus?`
     *
     * `uifocus+`
     *
     * `uifocus+?`
     *
     * `uialign`
     *
     * `uialign-`
     *
     * `uialign*`
     *
     * `uiclamp`
     *
     * `uiclamp-`
     *
     * `uiclamp*`
     *
     * `uigroup`
     *
     * `uihlist`
     *
     * `uivlist`
     *
     * `uilist`
     *
     * `uigrid`
     *
     * `uitableheader`
     *
     * `uitablerow`
     *
     * `uitable`
     *
     * `uispace`
     *
     * `uioffset`
     *
     * `uifill`
     *
     * `uitarget`
     *
     * `uiclip`
     *
     * `uiscroll`
     *
     * `uihscrolloffset`
     *
     * `uivscrolloffset`
     *
     * `uihscrollbar`
     *
     * `uivscrollbar`
     *
     * `uiscrollarrow`
     *
     * `uiscrollbutton`
     *
     * `uihslider`
     *
     * `uivslider`
     *
     * `uisliderarrow`
     *
     * `uisliderbutton`
     *
     * `uicolor`
     *
     * `uimodcolor`
     *
     * `uivgradient`
     *
     * `uimodvgradient`
     *
     * `uihgradient`
     *
     * `uimodhgradient`
     *
     * `uioutline`
     *
     * `uiline`
     *
     * `uitriangle`
     *
     * `uitriangleoutline`
     *
     * `uimodtriangle`
     *
     * `uicircle`
     *
     * `uicircleoutline`
     *
     * `uimodcircle`
     *
     * `uicolortext`
     *
     * `uitext`
     *
     * `uitextfill`
     *
     * `uiwrapcolortext`
     *
     * `uiwraptext`
     *
     * `uicolorcontext`
     *
     * `uicontext`
     *
     * `uicontextfill`
     *
     * `uiwrapcolorcontext`
     *
     * `uiwrapcontext`
     *
     * `uitexteditor`
     *
     * `uifont`
     *
     * `uiabovehud`
     *
     * `uiconsole`
     *
     * `uifield`
     *
     * `uikeyfield`
     *
     * `uiimage`
     *
     * `uistretchedimage`
     *
     * `uicroppedimage`
     *
     * `uiborderedimage`
     *
     * `uitiledimage`
     *
     * `uimodelpreview`
     *
     * `uiprefabpreview`
     *
     * `uislotview`
     *
     * `uivslotview`
     *
     * `uicontextscale`
     *
     * `newui`
     *
     * `uiallowinput`
     *
     * `uieschide`
     */
    void inituicmds();
}

/*==============================================================================*\
 * Render Functions & Values                                                    *
 *                                                                              *
 * World, Model, Material, Screenspace FX Rendering                             *
 *                                                                              *
 * aa.cpp                                                                       *
 * grass.cpp                                                                    *
 * hud.cpp                                                                      *
 * normal.cpp                                                                   *
 * octarender.cpp                                                               *
 * radiancehints.cpp                                                            *
 * rendergl.cpp                                                                 *
 * renderlights.cpp                                                             *
 * rendermodel.cpp                                                              *
 * renderparticles.cpp                                                          *
 * rendersky.cpp                                                                *
 * rendertext.cpp                                                               *
 * renderva.cpp                                                                 *
 * renderwindow.cpp                                                             *
 * shader.cpp                                                                   *
 * stain.cpp                                                                    *
 * texture.cpp                                                                  *
 * water.cpp                                                                    *
\*==============================================================================*/

// hud

/**
 * @brief Initializes HUD cubescript commands
 *
 * `loadcrosshair`
 *
 * `getcrosshair`
 */
extern void inithudcmds();

// normal

/**
 * @brief Initializes normal.cpp cubescript commands
 *
 * `smoothangle`
 */
extern void initnormalcmds();

// octarender

/**
 * @brief Initializes octarender.cpp cubescript commands
 *
 * `recalc`
 */
extern void initoctarendercmds();

// rendergl

/**
 * @brief Initializes rendergl.cpp cubescript commands
 *
 * `glext`
 *
 * `getcamyaw`
 *
 * `getcampitch`
 *
 * `getcamroll`
 *
 * `getcampos`
 */
extern void initrenderglcmds();

extern physent *camera1; /**< Camera location for the renderer to render at */
extern vec worldpos, camdir, camright, camup;
extern float fovy; /**< Field of view in degrees in the vertical direction (normal fov is X dimension). */
extern bool detachedcamera; /**< read only variable corresponding to camera at ent location (1p) or away from it (3p) */
extern int xtraverts;

/**
 * @brief Unsets any zoom progress and disables zooming.
 *
 * Sets the zoom variable to 0 (disabled) and removes any zoom progress that may
 * be occuring.
 */
extern void disablezoom();

extern vec calcavatarpos(const vec &pos, float dist);

extern void damageblend(int n);
extern void damagecompass(int n, const vec &loc);

extern vec minimapcenter, minimapradius, minimapscale;

/**
 * @brief Binds a GL texture to the minimaptex global object.
 *
 * The global variable minimap texture, minimaptex, has a GL buffer bound to it.
 * Before this is called, no texture is associated with it.
 */
extern void bindminimap();

extern matrix4 hudmatrix;
extern void resethudmatrix();
extern void pushhudmatrix();
extern void flushhudmatrix(bool flushparams = true);
extern void pophudmatrix(bool flush = true, bool flushparams = true);

/**
 * @brief Binds the HUD shader via glUseProgram()
 *
 * Also sets the color of the gle system to white (1,1,1).
 */
extern void resethudshader();
extern void initgbuffer();

extern void computezoom();
/**
 * @brief Enables GL offset mode for overlay drawing.
 *
 * Controlled by polygonoffsetfactor/polygonoffsetunits variables;
 * negative values will overlay in depth so there is no z fighting.
 * Enables the GL mode passed to type.
 *
 * @param type the GL mode to glEnable()
 */
extern void enablepolygonoffset(GLenum type);

/**
 * @brief Disables GL offset mode for overlay drawing.
 *
 * Also disables the GL mode passed to type.
 *
 * @param type the GL mode to glDisable()
 */
extern void disablepolygonoffset(GLenum type);

/**
 * @brief Returns 2 Euler angles for a given vector.
 *
 * Given a vector v, yaw and pitch are assigned to the orientation of the vector
 * in 3D space. Roll is not assigned, as it is ambiguous for a vector.
 *
 * @param v     vector to be used to calc yaw & pitch
 * @param yaw   output parameter v's yaw is assigned to
 * @param pitch output parameter v's pitch is assigned to
 */
extern void vectoryawpitch(const vec &v, float &yaw, float &pitch);

/**
 * @brief Ensures computer environment can run the engine.
 *
 * This function checks for the OpenGL contexts, GLSL version information, and
 * other graphics capabilities required to run the engine. This function will
 * call fatal() and exit the game if any of its conditions are not met.
 */
extern void gl_checkextensions();

/**
 * @brief Sets up GL global parameters for the game
 *
 * This function sets up the global variables which allow for the appropriate
 * operation of the renderer, and sets up the basic shaders required.
 */
extern void gl_init();

/**
 * @brief Changes the viewport size to set values
 *
 * This function sets the size of the viewport to the dimensions defined in
 * `hudw` and `hudh`. It also calls `gl_setupframe()`.
 */
extern void gl_resize();

/**
 * @brief Sets render frame variables up.
 *
 * Sets renderw/h and hudw/h to the screen size, then sets up deferred buffers
 * if `force` is true.
 */
extern void gl_setupframe(bool force = false);

/**
 * @brief Draws the frame to the screen.
 *
 * @param crosshairindex the crosshair to render at the center of the screen
 * @param gamefxn the game interface rendering function
 * @param hudfxn the 3d HUD rendering function
 * @param editfxn the game's edit rendering function
 * @param hud2d the game's HUD rendering function
 *
 */
extern void gl_drawframe(int crosshairindex, void (*gamefxn)(), void (*hudfxn)(), void (*editfxn)(), void (*hud2d)());

/**
 * @brief Draws a minimap texture.
 *
 * Draws a minimap buffer at the specified orientation and location on the map.
 * Does not render it to the screen, instead rendering it to the minimaptex buffer.
 *
 * @param yaw the yaw angle to draw the texture at
 * @param pitch the pitch angle to draw the texture at
 * @param loc the location about which to draw the texture
 * @param cubeworld the world to draw
 * @param scalefactor the texture zoom factor
 */
extern void drawminimap(int yaw, int pitch, vec loc, const cubeworld& world, int scalefactor = 1);

// renderlights

/**
 * @brief Initializes renderlights CubeScript commands
 *
 * `usepacknorm`
 */
extern void initrenderlightscmds();

/**
 * @brief Clears the shadow maps cached in the shadow atlas
 *
 * Also clears the radiance hints cache, if enabled.
 */
extern void clearshadowcache();

// rendermodel

/**
 * @brief Initializes rendermodel CubeScript commands
 *
 * `mdlcullface`
 *
 * `mdlcolor`
 *
 * `mdlcollide`
 *
 * `mdlellipsecollide`
 *
 * `mdltricollide`
 *
 * `mdlspec`
 *
 * `mdlgloss`
 *
 * `mdlalphatest`
 *
 * `mdldepthoffset`
 *
 * `mdlglow`
 *
 * `mdlfullbright`
 *
 * `mdlshader`
 *
 * `mdlspin`
 *
 * `mdlscale`
 *
 * `mdltrans`
 *
 * `mdlyaw`
 *
 * `mdlpitch`
 *
 * `mdlroll`
 *
 * `mdlshadow`
 *
 * `mdlalphashadow`
 *
 * `mdlbb`
 *
 * `mdlextendbb`
 *
 * `mdlname`
 *
 * `rdvert`
 *
 * `rdeye`
 *
 * `rdtri`
 *
 * `rdjoint`
 *
 * `rdlimitdist`
 *
 * `rdlimitrot`
 *
 * `rdanimjoints`
 *
 * `mapmodelreset`
 *
 * `mapmodel`
 *
 * `mapmodelname`
 *
 * `mapmodelloaded`
 *
 * `nummapmodels`
 *
 * `clearmodel`
 *
 * `findanims`
 */
extern void initrendermodelcmds();

extern std::vector<std::string> animnames; /**< A list of anim names to be loaded into the engine by the game. */

/**
 * @brief Renders a world model
 *
 * This function renders a model inside the world, given the following parameters.
 * If the model is not found, the function fails silently.
 *
 * @param mdl         name of the model to load
 * @param anim        index of the animation to use
 * @param o           location of the model
 * @param yaw         yaw orientation
 * @param pitch       pitch orientation
 * @param roll        roll orientation
 * @param cull        occlusion culling flags
 * @param d           dynent to associate model with
 * @param a           attached models to load
 * @param basetime    animation interpolation factor
 * @param basetime2   animation interpolation factor
 * @param size        scale factor for model
 * @param color       rgba color of the model
 */
extern void rendermodel(const char *mdl,
                        int anim,
                        const vec &o,
                        float yaw = 0,
                        float pitch = 0,
                        float roll = 0,
                        int cull = Model_CullVFC | Model_CullDist | Model_CullOccluded,
                        dynent *d = nullptr,
                        modelattach *a = nullptr,
                        int basetime = 0,
                        int basetime2 = 0,
                        float size = 1,
                        const vec4<float> &color = vec4<float>(1, 1, 1, 1));

extern int intersectmodel(const char *mdl, int anim, const vec &pos, float yaw, float pitch, float roll, const vec &o, const vec &ray, float &dist, int mode = 0, dynent *d = nullptr, modelattach *a = nullptr, int basetime = 0, int basetime2 = 0, float size = 1);

/**
 * @brief Adds the z height of the model to the vector passed.
 *
 * Takes the passed-by-reference vector and adds the z height of the bounding
 * box to the vector given.
 *
 * @param o the vector to add the height to
 * @param mdl the name of the model object to use
 */
extern void abovemodel(vec &o, const char *mdl);

/**
 * @brief Returns the mapmodel name at the given index.
 *
 * If the passed index corresponds to a valid mapmodel, the function returns a C
 * string with its name; if not, the function returns a nullptr.
 *
 * @return a C string with the name of the model
 */
extern const char *mapmodelname(int i);

/**
 * @brief Adds a model to the cache of models to load.
 *
 * When flushpreloadedmodels() is called, the entries added to the preloadedmodels
 * vector by this function are loaded. Silently fails and does nothing if the model
 * name passed is not valid or already exists in the vector.
 */
extern void preloadmodel(const char *name);

/**
 * @brief Loads the mapmodel assigned to the specified index.
 *
 * Loads the mapmodel assigned to the specified model index into a model object.
 * If a model object already exists, a new model is not created and a pointer to
 * that model is returned instead; otherwise, returns a pointer to the new model
 * object.
 * If the index is out of bounds, nullptr is returned.
 *
 * @return a pointer to the model object
 */
extern model *loadmapmodel(int n);

/**
 * @brief Loads the model with the associated name.
 *
 * @param name the name of the model to load
 * @param i the number of models to search through before giving up
 * @param msg toggles rendering a loading progress meter while loading the model
 */
extern model *loadmodel(const char *name, int i = -1, bool msg = false);

/**
 * @brief Clears the loaded model cache.
 *
 * @param msg toggles rendering a loading progress meter
 */
extern void flushpreloadedmodels(bool msg = true);

/**
 * @brief Preloads mapmodels corresponding to the world's mapmodel entities
 *
 * @param msg toggles printing warning messages
 * @param bih toggles preloading the bounding interval hierarchy (BIH) for the model
 */
extern void preloadusedmapmodels(bool msg = false, bool bih = false);

/**
 * @brief Clears all loaded models' basic information.
 *
 * Clears the model information from its associated hashmap. Does not clear preloaded
 * model information.
 */
extern void clear_models();

// renderparticles

extern std::vector<std::string> entnames; /**< The names of the map entities, set by the application code. */

/**
 * @brief Returns the entity name of of a given entity type index.
 *
 * For the given entity type index (light, model, etc.) returns the
 * human readable name of the entity type.
 * Returns an empty string if index out of bounds.
 *
 * @return a C string corresponding to the ent type's name
 */
extern const char * getentname(int i);

/**
 * @brief Spawns a splash particle with the designated properties.
 *
 * ```
 * dir: (where dir%3 is similar to offsetvec with 0=up)
 * 0..2 circle
 * 3.. 5 cylinder shell
 * 6..11 cone shell
 * 12..14 plane volume
 * 15..20 line volume, i.e. wall
 * 21 sphere
 * 24..26 flat plane
 * +32 to inverse direction
 * ```
 * @param type the type of particle to spawn
 * @param num the number indicating the spawning direction/pattern
 * @param fade the number of ms before the particle fades
 * @param p the location for the particle
 * @param color the color of the particle (web hex color)
 * @param size the size factor for the particle
 * @param radius the spawn area radius for the particle
 * @param gravity the gravity factor for particles after spawning
 * @param delay the delay in milliseconds between particle spawns
 */
extern void regular_particle_splash(int type, int num, int fade, const vec &p, int color = 0xFFFFFF, float size = 1.0f, int radius = 150, int gravity = 2, int delay = 0);

/**
 * @brief Spawns a flame particle
 *
 * @param type the type of particle to spawn
 * @param p location to spawn at
 * @param radius spawning radius of the particle
 * @param color the color of the particle (web hex color)
 * @param density the density of particle spawns
 * @param scale the scale factor for the particle
 * @param speed initial speed of particles after spawning
 * @param fade time in ms before the particle fades
 * @param gravity gravity factor for particles after spawning
 */
extern void regular_particle_flame(int type, const vec &p, float radius, float height, int color, int density = 3, float scale = 2.0f, float speed = 200.0f, float fade = 600.0f, int gravity = -15);
extern void particle_splash(int type, int num, int fade, const vec &p, int color = 0xFFFFFF, float size = 1.0f, int radius = 150, int gravity = 2);
extern void particle_trail(int type, int fade, const vec &from, const vec &to, int color = 0xFFFFFF, float size = 1.0f, int gravity = 20);
extern void particle_text(const vec &s, const char *t, int type, int fade = 2000, int color = 0xFFFFFF, float size = 2.0f, int gravity = 0);
extern void particle_textcopy(const vec &s, const char *t, int type, int fade = 2000, int color = 0xFFFFFF, float size = 2.0f, int gravity = 0);
extern void particle_icon(const vec &s, int ix, int iy, int type, int fade = 2000, int color = 0xFFFFFF, float size = 2.0f, int gravity = 0);
extern void particle_meter(const vec &s, float val, int type, int fade = 1, int color = 0xFFFFFF, int color2 = 0xFFFFF, float size = 2.0f);
extern void particle_flare(const vec &p, const vec &dest, int fade, int type, int color = 0xFFFFFF, float size = 0.28f, physent *owner = nullptr);
extern void particle_fireball(const vec &dest, float max, int type, int fade = -1, int color = 0xFFFFFF, float size = 4.0f);
extern void removetrackedparticles(physent *owner = nullptr);

/**
 * @brief Loads the shaders/renderer objects to render particles.
 *
 * Loads a series of particle related shader (particle, particlenotexture,
 * particlesoft, particletext) as well as initializing particlerenderer objects
 * as necessary to begin particle rendering.
 */
extern void initparticles();

// rendertext

/**
 * @brief Initializes rendertext Cubescript commands.
 *
 * `fontalias`
 *
 * `font`
 *
 * `fontborder`
 *
 * `fontoutline`
 *
 * `fontoffset`
 *
 * `fontscale`
 *
 * `fonttex`
 *
 * `fontchar`
 *
 * `fontskip`
 */
extern void initrendertextcmds();

/**
 * @brief Sets the currently used font.
 *
 * This function sets the current font to use to the one associated with the name
 * passed. If the function does not find the font requestion, the function returns
 * false.
 *
 * @return true  if the font was found and changed
 * @return false if the font was not found and not changed
 */
extern bool setfont(const char *name);

// renderwindow

extern SDL_Window *screen;

/**
 * @brief Initializes renderwindow Cubescript commands
 *
 * `getfps`
 *
 * `resetgl`
 *
 * `screenres`
 */
extern void initrenderwindowcmds();

/**
 * @brief Refreshes the rendered screenbuffer.
 *
 * This function refreshes the display buffer. The paramater has no effect.
 *
 * @param overlay Has no effect.
 */
extern void swapbuffers(bool overlay = true);
extern int fullscreen; /**< Toggles fullscreen, behaves like bool; 0 for windowed and 1 for fullscreen. */

/**
 * @brief Sets up the SDL window the application runs in.
 *
 * Should only be called once in the lifetime of the program. This function opens a window onscreen that the application runs
 * in.
 */
extern void setupscreen();
extern void restoregamma();
extern void restorevsync();

/**
 * @brief Sets the entirety of the fps history buffer (used for averaging the FPS readout) to 1
 *
 * Sets the fps history buffer values all to 1.
 */
extern void resetfpshistory();

/**
 * @brief delays a frame by a specified amount
 *
 * Uses SDL_Delay to delay a frame, given the time the last frame was
 * rendered and the current time. Takes into account the global fps cap global variable (maxfps) to calculate delay.
 *
 * @param millis: the time (in ms) since program started
 * @param curmillis: the last registered frame time
 */
extern void limitfps(int &millis, int curmillis);

/**
 * @brief Delays rendering of a frame.
 *
 * Uses SDL_Delay to delay a frame, given the time the last frame was
 * rendered and the current time. The amount of delay is calculated from the
 * global maxfps and menufps variables.
 *
 * @param millis the time (in ms) since program started
 * @param curmillis the last registered frame time
 */
extern void limitfps(int &millis, int curmillis);

/**
 * @brief Adds an entry to the fps history.
 *
 * The value millis passed is added to the fps history array, at a position greater
 * one greater than the previous entry.
 *
 * @param millis the timestamp to add to the fps history
 */
extern void updatefpshistory(int millis);

/**
 * @brief Sets the SDL gamma level to 1.
 *
 * Sets SDL_SetWindowBrightness of the screen the engine is rendering to to 1, if the
 * screen exists and the current gamma global variable has been changed from its default.
 */
extern void cleargamma();

extern void renderbackground(const char *caption = nullptr, Texture *mapshot = nullptr, const char *mapname = nullptr, const char *mapinfo = nullptr, bool force = false);

/**
 * @brief Renders a loading progress bar at the bottom center of the window.
 *
 * Intended for indicating asset loading or similar technical holding of the game state.
 *
 * @param bar from 0...1, indicates how full the progress bar should be
 * @param text the string to overlay on the progress bar
 * @param toggles rendering a background for the loading bar
 */
extern void renderprogress(float bar, const char *text, bool background = false);

// shader

/**
 * @brief Initializes shader Cubescript commands.
 *
 * `defershader`
 *
 * `forceshader`
 *
 * `shader` type name ps vs
 *  - Adds a shader to the engine's catalog.
 *  - `type` a bitmask describing the properties of the shader
 *  - `name` the name of the shader for calls from e.g. `useshaderbyname`
 *  - `ps` the pixel shader
 *  - `vs` the vertex shader
 *
 * `variantshader`
 *
 * `setshader`
 *
 * `isshaderdefined`
 *
 * `setshaderparam`
 *
 * `reuseuniformparam`
 *
 * `clearpostfx`
 *
 * `addpostfx`
 *
 * `setpostfx`
 *
 * `resetshaders`
 */
extern void initshadercmds();

/**
 * @brief Loads a series of shaders hardcoded into the engine.
 *
 * The shaders loaded are:
 *
 * `null`
 *
 * `hud`
 *
 * `hudtext`
 *
 * `hudnotexture`
 *
 * `stdworld`
 *
 * `nocolor`
 *
 * `fogged`
 *
 * `foggednotexture`
 *
 * `ldr`
 *
 * `ldrnotexture`
 *
 * If the shaders `null`...`stdworld` are not found, the engine exits by calling `fatal`.
 * `setupshaders` creates these textures and adds them to the shader global hashtable.
 */
extern void loadshaders();

// stain

/**
 * @brief Loads stains into the global stains array.
 *
 * Sets up each entry in the stains global variable array using init() method
 * and then preloads them.
 *
 * Fails to do anything if initing is set (early game loading time).
 */
extern void initstains();
extern void addstain(int type, const vec &center, const vec &surface, float radius, const bvec &color = bvec(0xFF, 0xFF, 0xFF), int info = 0);

inline void addstain(int type, const vec &center, const vec &surface, float radius, int color, int info = 0)
{
    addstain(type, center, surface, radius, bvec::hexcolor(color), info);
}

// texture

/**
 * @brief Initializes texture Cubescript commands.
 *
 * `texturereset`
 *
 * `materialreset`
 *
 * `decalreset`
 *
 * `compactvslots`
 *
 * `texture`
 *
 * `texgrass`
 *
 * `texscroll`
 *
 * `texoffset`
 *
 * `texrotate`
 *
 * `texangle`
 *
 * `texalpha`
 *
 * `texcolor`
 *
 * `texrefract`
 *
 * `texsmooth`
 *
 * `decaldepth`
 *
 * `reloadtex`
 *
 * `screenshot`
 */
extern void inittexturecmds();

extern Texture *notexture;

extern Texture *textureload(const char *name, int clamp = 0, bool mipit = true, bool msg = true);
extern void packvslot(std::vector<uchar> &buf, int index);
extern void packvslot(std::vector<uchar> &buf, const VSlot *vs);
extern bool settexture(const char *name, int clamp = 0);

/*==============================================================================*\
 * World Functions & Values                                                     *
 *                                                                              *
 * World Loading, Physics, Collision, Entity Handling                           *
 *                                                                              *
 * bih.cpp                                                                      *
 * dynlight.cpp                                                                 *
 * heightmap.cpp                                                                *
 * light.cpp                                                                    *
 * material.cpp                                                                 *
 * octaworld.cpp                                                                *
 * octaedit.cpp                                                                 *
 * physics.cpp                                                                  *
 * raycube.cpp                                                                  *
 * world.cpp                                                                    *
 * worldio.cpp                                                                  *
\*==============================================================================*/

// dynlight

extern void adddynlight(const vec &o, float radius, const vec &color, int fade = 0, int peak = 0, int flags = 0, float initradius = 0, const vec &initcolor = vec(0, 0, 0), physent *owner = nullptr, const vec &dir = vec(0, 0, 0), int spot = 0);
extern void removetrackeddynlights(const physent *owner = nullptr);

// heightmap

/**
 * @brief Determines if a heightmap is active for the current selection.
 *
 * @return true if there is a heightmap active
 * @return false if no heightmap is active
 */
extern bool isheightmap(int o, bool empty, const cube &c);
extern void heightmaprun(int dir, int mode);

/**
 * @brief Initializes heightmap Cubescript commands.
 *
 * `hmapcancel`
 *
 * `hmapselect`
 *
 * `clearhbrush`
 *
 * `hbrushvert`
 */
extern void initheightmapcmds();

// material
extern int showmat; /**< treat as bool, determines whether material outlines are rendered*/

/**
 * @brief Returns the bitmask for the given material name.
 *
 * Given a material name (water, clip, noclip, etc.), returns the bitmask value
 * the material is associated with in the octree world.
 *
 * @param name a C string with the name of the material to look up
 *
 * @return an integer corresponding to the bitmask value of the material
 */
extern int findmaterial(const char *name);

// octaworld

/**
 * @brief Initializes octaworld Cubescript commands.
 *
 * `printcube` prints debug information for a cube
 */
extern void initoctaworldcmds();

/**
 * @brief Deletes a cube and its children.
 *
 * Recursively deletes child members of the cube passed, then deletes the cube
 * itself. The node allocation counter is also decreased to correspond to the
 * removed cubes.
 *
 * @param c the cube object to be deleted
 */
extern void freeocta(cube *c);
extern void getcubevector(const cube &c, int d, int x, int y, int z, ivec &p);

extern void optiface(const uchar *p, cube &c);

// octaedit

/**
 * @brief Initializes the octaedit cubescript commands.
 *
 * `moving`
 *
 * `entcancel`
 *
 * `cubecancel`
 *
 * `cancelsel`
 *
 * `reorient`
 *
 * `selextend`
 *
 * `selmoved`
 *
 * `selsave`
 *
 * `selrestore`
 *
 * `selswap`
 *
 * `havesel`
 *
 * `selchildnum`
 *
 * `selchildmat`
 *
 * `clearundos`
 *
 * `delprefab`
 *
 * `saveprefab`
 *
 * `pasteprefab`
 *
 * `wtr`
 *
 * `vtr`
 *
 * `wvt`
 *
 * `vvt`
 *
 * `evt`
 *
 * `eva`
 *
 * `octa`
 *
 * `va`
 *
 * `gldes`
 *
 * `geombatch`
 *
 * `oq`
 */
extern void initoctaeditcmds();

extern bool allowediting;
extern bool multiplayer;
extern std::vector<ushort> texmru;
extern selinfo repsel;
extern int reptex;
extern int lasttex; /**< index for the most recently used texture*/
extern int curtexindex;
extern int moving, orient, horient;
extern bool editmode;
extern int entmoving;
extern int entediting;
extern selinfo sel, lastsel;
extern std::deque<undoblock *> undos, /**< list of cube undo operations */
                               redos; /**< list of cube operations available to redo */
extern int nompedit;
extern int hmapedit; /**< used as boolean, 0 if not heightmapping, 1 otherwise */
extern bool havesel; /**< true if there is a set of cubes in selection, false otherwise */
extern std::vector<editinfo *> editinfos;
extern int texpaneltimer; /**< Sets the length before the texture scroll panel turns off. */

extern ivec cor, cur, lastcor;
extern int gridsize;
extern int passthroughsel;
extern int outline;
extern int dragging;
extern int selectcorners;

extern void clearremappedvslots();

/**
 * @brief Draws a box corresponding to positions in worldspace.
 *
 * @param o the origin from where to draw the box
 * @param s the displacement vector from the origin to place the opposite corner
 * @param g the gridpower to multiply the box by
 * @param boxoutline toggles rendering such that it appears outside a cube with the same dimensions
 */
extern void boxs3D(const vec &o, vec s, int g, bool boxoutline);

/**
 * @brief Draws a box corresponding to positions in worldspace.
 *
 * @param orient the orientation direction of the square
 * @param o the origin from where to draw the box
 * @param s the displacement vector from the origin to place the opposite corner
 * @param size the scale factor of the box in the world
 * @param boxoutline toggles rendering such that it appears outside a cube with the same dimensions
 */
extern void boxs(int orient, vec o, const vec &s, float size, bool boxoutline);

/**
 * @brief Draws a box corresponding to positions in worldspace.
 *
 * @param orient the orientation direction of the square
 * @param o the origin from where to draw the box
 * @param s the displacement vector from the origin to place the opposite corner
 * @param boxoutline toggles rendering such that it appears outside a cube with the same dimensions
 */
extern void boxs(int orient, vec o, const vec &s, bool boxoutline);
extern void boxsgrid(int orient, vec o, vec s, int g, bool boxoutline);
extern bool editmoveplane(const vec &o, const vec &ray, int d, float off, vec &handle, vec &dest, bool first);

/**
 * @brief Assigns selchildcount the number of cubes in selection.
 *
 * Recursively counts the number of cubes being selected in the current selection,
 * and assigns this value to selchildcount
 */
extern void countselchild(const cube *c, const ivec &cor, int size);
extern void normalizelookupcube(const ivec &o);
extern void updateselection();

extern bool packeditinfo(const editinfo *e, int &inlen, uchar *&outbuf, int &outlen);
extern bool unpackeditinfo(editinfo *&e, const uchar *inbuf, int inlen, int outlen);

/**
 * @brief Safely frees an editinfo object.
 *
 * Deletes an editinfo object, including its position in the editinfos vector.
 *
 * @param e the editinfo object to destroy.
 */
extern void freeeditinfo(editinfo *&e);
extern bool packundo(bool undo, int &inlen, uchar *&outbuf, int &outlen);
extern bool noedit(bool view = false, bool msg = true);

/**
 * @brief Determines whether selection contains location at specified location.
 *
 * Returns whether the location of the vector passed falls within the selection
 * information object's volume. If the passed vector is exactly along the boundary,
 * the function still returns true.
 *
 * @param sel selinfo object corresponding to a world volume
 * @param origin vector corresponding to a world location
 *
 * @return true if the vector is inside the selinfo object
 * @return false if the vector is outside the selinfo object
 */
extern bool pointinsel(const selinfo &sel, const vec &o);

/**
 * @brief Deselects edit selection.
 *
 * This function deselects the cubes and entities currently selected.
 */
extern void cancelsel();

/**
 * @brief Adds an undo entry to the global undos vector.
 *
 * @param the undo object to add
 */
extern void addundo(undoblock *u);

/**
 * @brief Looks up a world cube inside the block passed.
 *
 * @param x the x coordiante inside the block3 to look up
 * @param y the y coordiante inside the block3 to look up
 * @param z the z coordiante inside the block3 to look up
 * @param b the block3 object to search inside
 * @param rgrid the grid power to look up with
 *
 * @return a reference to the cube found
 */
extern cube &blockcube(int x, int y, int z, const block3 &b, int rgrid);

/**
 * @brief Adds an undo entry to the undos vector given the passed selection.
 *
 * Uses the passed selection to add a new undo object to the global undos vector.
 *
 * @param the selection to use
 */
extern void makeundo(selinfo &s);

/**
 * @brief Adds an undo entry to the undos vector given the current selection.
 *
 * Uses the current global selection to add a new undo object to the global
 * undos vector.
 */
extern void makeundo();

/**
 * @brief Reorients `sel` to point in the direction of `orient`.
 *
 * See the cube faces in octa.h to interpret what the value of `orient` means in dimensions.
 *
 */
extern void reorient();

/**
 * @brief Counts the number of cubes (incl. children) within a block3 volume.
 *
 * @param b the block to evaluate
 *
 * @return the number of cubes in the block
 */
extern int countblock(block3 *b);

/**
 * @brief Sets `lastsel`'s orientation flag to -1.
 */
extern void forcenextundo();

/**
 * @brief Deletes an undoblock object.
 *
 * Destroys an undoblock and frees its memory allocation.
 * The pointer passed will become invalid.
 *
 * @param u the undoblock to destroy.
 */
extern void freeundo(undoblock *u);
extern void pasteblock(block3 &b, selinfo &sel, bool local);
extern void pasteundo(undoblock *u);

/**
 * @brief Creates an undoblock object given a cube selection.
 *
 * @param s the selection information to give to the undoblock object
 *
 * @return a pointer to a new heap-allocated undoblock object
 */
extern undoblock *newundocube(const selinfo &s);
extern void remapvslots(cube &c, bool delta, const VSlot &ds, int orient, bool &findrep, VSlot *&findedit);

/**
 * @brief Changes the texture for a cube and its children.
 *
 * The integer indices for the `orient` parameter are indicated below.
 * ```
 *  +z
 *   |  /+x
 *   | /
 *   |/___+y
 *      ______
 *     /.    /|
 *    / . 5 / |
 *   /__.__/  |
 *   |  ...|3./
 *   | .0  | /
 *   |.    |/
 *   .-----/
 *      ______
 *     /|    .|
 *    / |  1. |
 *   /..|...  |
 *   |2 |-----/
 *   | /   . /
 *   |/   4./
 *   .-----/
 * ```
 * @param c the cube to modify
 * @param tex the texture slot to apply
 * @param orient the face of the cube to apply to; -1 indicates all faces
 * @param findrep toggles whether to find the existing texture
 */
extern void edittexcube(cube &c, int tex, int orient, bool &findrep);

/**
 * @brief Queries whether the current selection has entities selected.
 *
 * @return true if there are entities selected
 * @return false if no entities selected
 */
extern bool haveselent();

/**
 * @brief Copies and pastes a single cube (with its children).
 *
 * Copies the cube located at `src`, along with its child cubes, and
 * pastes it into the cube node located at `dst`. If the `dst` cube is
 * of a smaller grid size than the source cube, the extra child levels are
 * truncated.
 *
 * @param src a reference to the source cube to copy
 * @param dst a reference to the destination cube where the selection is pasted
 */
extern void pastecube(const cube &src, cube &dst);

/**
 * @brief Pastes an undo block associated with the passed block3 object.
 *
 * @param b the block3 to use to paste
 * @param g the gridscale array to pass
 */
extern void pasteundoblock(block3 *b, const uchar *g);
extern bool uncompresseditinfo(const uchar *inbuf, int inlen, uchar *&outbuf, int &outlen);

/**
 * @brief Unpacks a buffer containing an undo action into a unsigned char array.
 *
 * This function does not preserve the original buffer.
 *
 * @param buf the buffer to unpack
 * @param outbuf the buffer to unpack into.
 *
 */
extern void unpackundocube(ucharbuf &buf, uchar *outbuf);

/**
 * @brief Prints a warning message to the console.
 *
 * This function prints a message warning the user that the operation is not
 * possible in multiplayer. It does nothing else.
 */
extern void multiplayerwarn();

/**
 * @brief Returns the material bitmask for the given cube.
 *
 * Returns the bitmask for the cube passed, which corresponds to the sum of all
 * materials occupying the cube. Every sum is unique due to the basis selection,
 * and therefore any combination of materials can be represented in the output.
 * Returns 0 (air) if there is no valid material that describes the entire cube.
 *
 * @return An unsigned short integer corresponding to the material bitmask
 */
extern ushort getmaterial(cube &c);

// raycube

/**
 * @brief Returns the distance before a ray hits a cube.
 *
 * This function behaves the same as raycube() execept also returning the location
 * where the hit happened.
 *
 * @param o      the starting location of the ray to be drawn
 * @param ray    normalized direction vector for the ray to point
 * @param hit    output parameter for the vector location the hit happened at
 * @param radius region around ray that counts as a hit
 * @param mode   flags which determine what counts as a hit
 * @param size   size of cube which registers a hit
 * @param t      entity to check against
 */
extern float raycubepos(const vec &o, const vec &ray, vec &hit, float radius = 0, int mode = Ray_ClipMat, int size = 0);
extern float rayfloor  (const vec &o, vec &floor, int mode = 0, float radius = 0);
extern bool  raycubelos(const vec &o, const vec &dest, vec &hitpos);
extern float rayent(const vec &o, const vec &ray, float radius, int mode, int size, int &orient, int &ent);

/**
 * @brief Queries whether the given vector lies inside the octree world.
 *
 * Returns truth value for whether the vector given has coordinates which lie
 * within the space occupied by the worldroot cube. If the dimension passed is
 * exactly equal to a boundary, the function returns true (this is considered as
 * being inside the world).
 *
 * @return true if the vector is inside the world boundaries
 * @return false if the vector is outside the world
 *
 */
extern bool insideworld(const vec &o);

// physics

extern vec collidewall;
extern int collideinside;
extern physent *collideplayer;
extern int numdynents;
extern std::vector<dynent *> dynents;

/**
 * @brief Returns the dynent at index i.
 *
 * Returns a pointer to the dynent object stored at index i of the dynents
 * vector. Returns nullptr if index i is out of bounds.
 *
 * @param i the index to get the dynent at
 * @return a pointer to the dynent object at index i
 */
extern dynent *iterdynents(int i);

/**
 * @brief Determines whether the specified physent has collided with the octree world
 *
 * @param d a pointer to the physent
 * @param dir the direction to attempt to collide at
 * @param cutoff collision cutoff slope
 * @param playercol toggles colliding with outside players
 * @param insideplayercol tooggles collidign with players you share space with
 */
extern bool collide(const physent *d, const vec &dir = vec(0, 0, 0), float cutoff = 0.0f, bool playercol = true, bool insideplayercol = false);
extern void avoidcollision(physent *d, const vec &dir, physent *obstacle, float space);

extern bool movecamera(physent *pl, const vec &dir, float dist, float stepdist);

/**
 * @brief Drops the map entity to a set distance above the floor.
 *
 * Drops the entity passed such that it is 4 cube units above the floor, unless
 * the map entity is of the type "particles", whereupon it is dropped to be at
 * the same height as the floor.
 *
 * @param e the entity to drop towards the floor.
 */
extern void dropenttofloor(entity *e);

/**
 * @brief Rotates the center and radius vectors using the given Euler angles
 *
 * @param center the center of the bounding box
 * @param radius the size of the bounding box
 * @param yaw the yaw amount to modify by
 * @param pitch the pitch amount to modify by
 * @param roll the roll amount to modify by
 */
extern void rotatebb(vec &center, vec &radius, int yaw, int pitch, int roll = 0);

/**
 * @brief Modifies a vector based on passed movement modifiers.
 *
 * Given a vector and a series of movement parameters, the passed vector is modified
 * by the values of move (forward and backward) and strafe (side to side). These
 * axes are independent of the vec to be modified, and are oriented relative to
 * the yaw and pitch parameters passed.
 *
 * @param yaw    the angle the player is pointing
 * @param pitch  the angle at which the player is ascending
 * @param move   the quantity of forwards to backwards motion to add
 * @param strafe the quantity of side to side motion to add
 * @param m      the vector to modify
 */
extern void vecfromyawpitch(float yaw, float pitch, int move, int strafe, vec &m);
extern void cleardynentcache();
extern void updatedynentcache(physent *d);

/**
 * @brief Brute force but effective way to find a free spawn spot in the map.
 *
 * Attemps to place a dynent onto the map.
 *
 * @param d the dynent to place onto the map
 * @param avoidplayers toggles whether this dynent should be spawned on top of others
 */
extern bool entinmap(dynent *d, bool avoidplayers = false);

extern void recalcdir(const physent *d, const vec &oldvel, vec &dir);
extern void slideagainst(physent *d, vec &dir, const vec &obstacle, bool foundfloor, bool slidecollide);

/**
 * @brief Deletes dynamically allocated cubes to a block entity
 *
 * Deletes the cube octree associated with the block3 object. If alloced is true,
 * also deletes the block3 object itself.
 *
 * @param b a pointer to the block to free
 * @param alloced if true, deletes the block itself
 */
extern void freeblock(block3 *b, bool alloced = true);
extern block3 *blockcopy(const block3 &s, int rgrid);

// world

extern std::vector<int> entgroup;

/**
 * @brief Cancels selection of selected entities.
 *
 * Clears the selection of any entities in edit mode. Has no effect if no entities
 * are selected.
 */
extern void entcancel();

/**
 * @brief Attempts to attach the given entity to another entity.
 *
 * Attempts to attach the entity passed to another entity that it can be attached
 * to, as long as it is within the attach radius.
 *
 * Fails silently if the given entity has no close by entities to link to or the
 * entity type is ineligible to be linked
 *
 * @param e the extentity to attempt to link
 */
extern void attachentity(extentity &e);

/**
 * @brief Attempts to attach all entities in selection.
 *
 * For all eligible entities in the current selection, the engine attempts to link
 * it with its nearest entity it can link with; silently fails and does nothing if
 * no entities in selection are eligible or selected.
 */
extern void attachentities();
extern void removeentityedit(int id);
extern void addentityedit(int id);

/**
 * @brief Attempts to dettach the given entity.
 *
 * Attempts to detach the entity from any entity it may be attached to. Fails
 * silently if the entity is not attached.
 *
 * @param e the extentity to attempt to de-link
 */
extern void detachentity(extentity &e);
extern void entselectionbox(const entity &e, vec &eo, vec &es);
extern void mmboundbox(const entity &e, model *m, vec &center, vec &radius);

/**
 * @brief Gets the depth of a decal slot.
 *
 * Returns the depth value for a decal slot. The depth value determines the distance
 * between a decal and a piece of geometry where the decal will still render.
 *
 * @param s the decalslot to query
 * @return a floating point value indicating the slot's depth
 */
extern float getdecalslotdepth(const DecalSlot &s);

namespace entities
{
    /**
     * @brief The list of extentities corresponding to the cube world's entities
     */
    extern std::vector<extentity *> ents;

    /**
     * @brief Returns the entities::ents vector.
     */
    extern std::vector<extentity *> &getents();
    extern extentity *newentity();
    extern void deleteentity(extentity *e);
}

// worldio

/**
 * @brief Returns the name of the map currently loaded.
 */
const char *getclientmap();

/**
 * @brief Sets the client map string to the provided value.
 */
const void setmapname(const char * newname);

/**
 * @brief Initializes worldio Cubescript commands.
 *
 * `mapcfgname`
 */
extern void initworldiocmds();

#endif /* IENGINE_H_ */
