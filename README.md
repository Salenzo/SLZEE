Ŝalenzo Equation Editor 2
=========================

An incomplete and unstable engine for typesetting mathematical formulas with bitmap fonts and bitmap outputs. The quality is, needless to say, far coarser than other tools such as TeX; it is nevertheless much smaller than those tools in size. There's only one bitmap font with a very limited character set, after all!

This is a complete rewrite of SLZEE 1, which was written in Ruby and a mess, in C99, and now it seems even messier.

Usage
-----

To build on Windows or \*nix systems:

```bash
wget -O lodepng.c https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.cpp
cc slzee2.c lodepng.c -o slzee
```

SLZEE 2 reads the equation to be typeset from the standard input only.

### Syntax ###

◊ ⟨mandatory⟩
⌷ [optional]

- ◊. *— single line*
- ◊\◊\◊. *— multiline*
- r⟨glyph index⟩ *— character*
- F⟨numerator⟩.⟨denominator⟩. *— fraction*
- ⌷S[superscript].[subscript]. *— super- and subscript*
- B⟨middle-delimiter count⟩⟨upper delimiter #0 glyph index⟩⟨lower delimiter #0 glyph index|-1⟩⟨upper delimiter #1 glyph index⟩⟨lower delimiter #1 glyph index|-1⟩…◊.◊.…◊. *— bracket*
- X◊. *— box*
- s0 *— normal size*
- s1024 *— script size*
- ! *— negative thin space*
- , *— thin space*
- \> *— medium space*
- ; *— thick space*
- c⟨0..15⟩ *— set color*

#### Sample inputs ####

- `r6r1r7r4.` → 6174
- `Fr1.r2..` → ½
- `r2Sr3r2...` → 2<sup>32</sup>

Credits and license
-------------------

SLZEE 2 is released under the MIT License; see LICENSE.txt.

The SLZEE 2 default font (slzee2-font.png) is licensed under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0).

SLZEE 2 uses [LodePNG](https://lodev.org/lodepng/) (under the zlib License) for reading and writing images.

The code for drawing lines and ellipses is adapted from [*The Beauty of Bresenham’s Algorithm*](http://members.chello.at/~easyfilter/bresenham.html).
