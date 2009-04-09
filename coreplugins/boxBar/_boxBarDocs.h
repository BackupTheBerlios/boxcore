/** @page boxBar Documentation for the boxBar plugin
 * boxBar is a <A HREF="http://bb4win.sourceforge.net/">Blackbox for Windows</A> plugin that provides
 * a system bar. It is designed to be easily extensible, and is becoming more configurable. Its current
 * major feature is that allows a vertical bar.
 *
 * boxBar has been tested with boxCore on Windows Vista x64 by the author, and on Windows XP by users.
 * It should also work under other branches (except xoblite at this point, due to task support), and
 * has had some testing under vanilla bbLean.
 *
 * @section boxBarToc Table of Contents
 * - @ref boxBarChangelog
 * - @ref boxBarSettings
 *
 * @section boxBarChangelog Changelog
 * @subsection boxBar_0_2_0 boxBar 0.2.0 (2008-04-??)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - Added code so that tasks flash when they want attention
 * - Tasks now fetch their own icons, allowing 32px icons under all branches
 * - Tasks support Unicode captions
 * - Added icon support to the button, default blackbox icon only
 * - Some settings renamed
 *
 * @subsection boxBar_0_1_7 boxBar 0.1.7 (2008-02-18)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - Fixes a bug in sizing things which left a gap when used vertically.
 * - Some fixes for tooltips. Some where never being displayed (uTorrent) and some things
 * would cause tooltips to flash
 *
 * @subsection boxBar_0_1_6 boxBar 0.1.6 (2008-01-18)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - This fixes a crashing bug when clicking on tasks that were no longer valid,
 * a consequence of this is that Task reordering has been removed as this was the cause of the problem.
 * - The text changing style on tasks should also be fixed
 * - Added a "button" item. The default config has all its options
 * - Added drag-over support to focus a task. So now dragging a file over a task will bring
 * that task up so you can drop the file in its window
 * - Maximum width of tasks now configurable. See the default config file or use the
 * right-click config menu to change it (0 disables)
 * - The &&& problem in tooltips is also hopefully fixed
 *
 * @subsection boxBar_0_1_5 boxBar 0.1.5 (2008-01-16)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - A config menu for tray options ;)
 * - There is a new config option on the tray to reverse the order of drawing icons (including spacers)
 * - Fixed the tooltips staying behind bug and plugged a memory leak that was causing it
 * - Positioning of balloon tooltips is now slightly better
 *
 * @subsection boxBar_0_1_4 boxBar 0.1.4 (2008-01-06)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - Add option for tray direction
 * - Uses proposed Unicode api
 * - Support unicode tray tooltips on boxCore
 *
 * @subsection boxBar_0_1_3 boxBar 0.1.3 (2008-12-26)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - Add Slit support
 * - Add support for toggle with plugins
 * - Add task reorganising
 * - Fix clock errors in some languages
 * - Fix some sizing bugs
 *
 * @subsection boxBar_0_1_2 boxBar 0.1.2 (2008-12-06)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - First standalone release
 * - Improved drawing (less flicker)
 * - Added workspace label item
 *
 * @subsection boxBar_0_1_1 boxBar 0.1.1 (2008-12-02)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - Added size for task and tray icons
 * - Improved reloading of settings on reconfigure
 *
 * @subsection boxBar_0_1_0 boxBar 0.1.0 (2008-11-28)
 * @author
 * <A HREF="mailto:carsomyr@users.berlios.de">Carsomyr</A>
 *
 * - First release as part of boxCore
 * @section boxBarSettings Settings
 * For settings that you can change in the RC file you can see
 * - @subpage boxBarRC
 * - @subpage boxBarRCAdvanced
 */

/**
 * @page boxBarRC RC Settings for boxBar
 * These are the settings that you will typically change to configure boxBar
 * to work the way you want it to. Many of these options can also be set through the
 * right-click configuration menu. Some notes for items follow.
 *
 * @section boxBarNameable Nameable Items
 * Some items are listed as being nameable. This means that when adding them in "boxBar.items:" you
 * can use itemType:Name syntax, instead of just itemType. For example, you can say "Button:Start"
 * to create a named button item, or you can just say "Button" to create a button with the default name.
 * If you create a named item, the settings for that item use the item name instead of the item type.
 * so for the named button, its settings would be "boxBar.Start.ShowIcon:" instead
 * of "boxBar.Button.ShowIcon". This lets you add more than one button, with different settings.
 */

/**
 * @page boxBarRCAdvanced Advanced RC settings for boxBar
 * Settings listed here should be used with care, as they may have unintended effects.
 */
