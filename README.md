# LeafEdit-Pattern-Editor
This is LeafEdit's Pattern Editor as a separate app as well!

NOTE: This tool can _only_ edit and store pattern. For a more featured Save Editor please use [LeafEdit](https://github.com/Universal-Team/LeafEdit) instead.


## Current Features
- [x] Open `.acnl` and `.acww` files.

- [x] Automatically detection of Pattern format.

- [x] Have pre-included pattern for AC:NL & AC:WW.

- [x] Creation of empty pattern.

- [x] Edit Palettes through the Palette Menu.

- [x] Edit Pattern.

- [x] Export pattern to `.acnl` or `.acww` format, depending on the Pattern format.

- [x] Include a font, which should be able to display all characters.

- [x] A Pattern Tool Overlay.

- [x] Ability, to load a default pattern on each startup from the SD Card through the `Settings.json` file.

- [x] Ability to load Pattern "default" information, such as Pattern Name, Creator Name, Town Name, Creator ID, Town ID & Creator Gender.

- [x] Added a proper banner & icon.

- [x] German & English support.

- [x] Export Pattern Informations. // For the defaults.

- [x] Edit Pattern from AC:WW, AC:NL, AC:WA & AC:HHD's savefile directly.

- [x] A Storage System for your favorite AC:WW, AC:NL, AC:WA & AC:HHD's Pattern.

- [x] A Storage Menu for changing, resizing, creating, saving and managing the storage.

- [x] Music Playback support. (It supports .WAV and it MUST be MONO and smaller than 13 MB and it must be called `Music.wav` and inside `sdmc:/3ds/LeafEdit/Pattern-Editor/`).

## Planned | TODOs
- [ ] 16-bit BMP images to Pattern conversion.

- [ ] Add "Draw Modes".. for example: Line Mode, Normal Mode etc.

## Screenshots of LeafEdit-Pattern-Editor
### Credits
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/splash.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/credits.png)

### MainScreen
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/main.png)

### Palette Editor
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/paletteEditor.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/colorGroup.png)

### Import and Export
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/export.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/import.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/prompt.png)

### Pattern Tool
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/toolMenu.png)

### Selections
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/gameSelect.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/regionSelect.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/langSelect.png)

### ShareOverlay
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/shareOvl.png)

### Storage
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/storageMenu.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/storageMain.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/storageInfo.png)


## Compilation:
You need devkitARM along with Libctru, Citro2D & Citro3D for this to compile. You will also need Universal-Core, which should be included, when you git clone this repository properly. You also need bannertool and Makerom inside this folder and then run make. Then you should have a `LeafEdit-Pattern-Editor.3dsx` and a `LeafEdit-Pattern-Editor.cia` executable.

## Credits
### Main Developer
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): For creating this app.

### Translators
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): German & English.

### Other
- [devkitPro](https://github.com/devkitPro), [Fincs](https://github.com/fincs), [Smealum](https://github.com/smealum), [Wintermute](https://github.com/WinterMute): For devkitARM, Citro2D, Citro3D and Libctru.

- [LeafEdit-Core-Contributors](https://github.com/Universal-Team/LeafEdit): For helping on LeafEdit-Core.

- [PKSM](https://github.com/FlagBrew/PKSM): For the base of the storage / Bank system, which i used, but now reworked for the Pattern system.

- [Slattz](https://github.com/Slattz): For the region flags & game icons from [NLTK](https://github.com/Slattz/NLTK).

- [Universal-Team](https://github.com/Universal-Team): For [LeafEdit's](https://github.com/Universal-Team/LeafEdit) core and [Universal-Core](https://github.com/Universal-Team/Universal-Core).

- [TotallyNotGuy](https://github.com/TotallyNotGuy): For the amazing Graphics from LeafEdit.