/** @page boxBar Documentation for the boxBar plugin
 * boxBar is a system tray plugin for Blackbox for Windows.
 * @author Shaun Barlow "Carsomyr" <carsomyr@users.berlios.de>
 *
 * @section boxBarChangelog Changelog
 * @subsection boxBar_0_1_0 boxBar 0.1.0
 *
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
 * can use itemType:Name syntax, instead of just itemtype. For example, you can say "Button:Start"
 * to create a named button item, or you can just say "Button" to create a button with the default name.
 * If you create a named item, the settings for that item use the item name instead of the item type.
 * so for the named button, its settings would be "boxBar.Start.ShowIcon:" instead
 * of "boxBar.Button.ShowIcon". This lets you add more than one button, with different settings.
 */

/**
 * @page boxBarRCAdvanced Advanced RC settings for boxBar
 * Settings listed here should be used with care, as they may have unintended effects.
 */
