# LeafEdit-Pattern-Editor
This is LeafEdit's Pattern Editor as a separate app as well!

This app is made for fun, those Features will be part of LeafEdit itself as well.

This app is mainly used, to test Pattern Editing, before it gets officially implemented into LeafEdit itself.

## Current Features
- [x] Open `.acnl` and `.acww` files.
- [x] Automatically detection of Pattern format.
- [x] Have pre-included pattern for AC:NL & AC:WW.
- [x] Creation of empty pattern.
- [x] Edit Palettes through the Palette Menu.
- [x] Edit Pattern.
- [x] Export pattern to `.acnl` or `.acww` format, depending on the Pattern format.
- [x] Include a font, which should be able to display all characters.
- [x] A Pattern Tool Overlay. // TODO: Second page for Pattern Name, Creator Name etc.
- [x] Ability, to load a default pattern on each startup from the SD Card through the `Settings.ini` file. // TODO: Do a Selection from the app itself, instead of manually modifying.

## TODOs
- [ ] Add Pattern Name, Creator Name & Origin Town Name editing.
- [ ] Add Player ID, Origin Town ID editing.
- [ ] Add Creator Gender editing. (Is AC:NL only(?))
- [ ] Add a proper banner & icon.

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
![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/saveSelect.png) ![](https://github.com/SuperSaiyajinStackZ/LeafEdit-Pattern-Editor/blob/master/screenshots/regionSelect.png)


## Compilation:
You need devkitARM along with Libctru, Citro2D & Citro3D for this to compile. You will also need Universal-Core, which should be included, when you git clone this repository properly. You also need bannertool and Makerom inside this folder and then run make. Then you should have a `LeafEdit-Pattern-Editor.3dsx` and a `LeafEdit-Pattern-Editor.cia` executable.

## Credits
### Main Developer
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): For creating this app.

### Other
- [devkitPro](https://github.com/devkitPro), [Fincs](https://github.com/fincs), [Smealum](https://github.com/smealum), [Wintermute](https://github.com/WinterMute): For devkitARM, Citro2D, Citro3D and Libctru.

- [LeafEdit-Core-Contributors](https://github.com/Universal-Team/LeafEdit): For helping on LeafEdit-Core.

- [Slattz](https://github.com/Slattz): For the region flags & game icons from [NLTK](https://github.com/Slattz/NLTK).

- [Universal-Team](https://github.com/Universal-Team): For [LeafEdit's](https://github.com/Universal-Team/LeafEdit) pattern core and [Universal-Core](https://github.com/Universal-Team/Universal-Core).

- [TotallyNotGuy](https://github.com/TotallyNotGuy): For the amazing Graphics from LeafEdit.