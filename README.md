# nobsbatt

A no bullshit battery monitor for OpenBSD. In particular, really
useful for running alongside cwm in case you don't run something like
`dzen2` or `lemonbar`. It is designed to be remisicent of the little
boxes you get on FVWM and GNUStep.

I wanted something that:

- Wasn't itself bloated, or depend on a whole bunch of extraneous shit.
- Fit my preferred colorscheme and font.
- Didn't offer stupid features I didn't want.
- Wasn't ugly with gross little bitmaps or eye-gouging colors.
- Quite simply: just show me the AC status, battery percentage and
estimated runtime (if applicable), stay in the corner near xclock and
get out of the way.

Maybe something like that is out there, but I looked and I couldn't
find it, so I wrote one. If you can use it too, great.

## Configuration

There's no runtime configuration. If you're familiar with Suckless
software, the `config.h` should be familar - basically these are all
the knobs you can turn, and you do so before you compile it.

That said, there are some rough edges. In particular with regard to
dealing with text sizing and positioning. The magic numbers in
`nobsbatt.c` for drawing the text work for my settings - and I don't
really have the desire to figure out a less icky way.

So you're on your own there, you might need some trial and error to
get it looking right.

## Install

`make`

`doas make install`

There are no dependencies on anything outside of the base system
(assuming you installed Xenocara), *except* for the Spleen font, which
is my monospaced font of choice. But, that's configurable - so if you
want to use some other font, you need to configure that.

## Does it work with anything else aside from OpenBSD?

No idea. And I don't care enough to find out.

## You're a fool! Use a bar! Use `insert bloated DE here` because it has this!

No. Because:

a) I don't have to justify my choices to you

b) I wrote this

c) Go away

d) No.

Also, no.

## Please implement `random bloated feature` because I want it!

No.

## I want to send you a PR implementing some brilliant feature or to fix some silly bug

Go right ahead. Though I reserve the right to ignore you because it's
probably a feature I don't want.

And like all programmers I'll claim this has no bugs until proven otherwise. :-)

## What's this look like?

See screenshot.png.

# License

This software is ISC licensed.

Copyright 2021, Dr Ashton Fagg <ashton@fagg.id.au>

Permission to use, copy, modify, and/or distribute this software for
any purpose with or without fee is hereby granted, provided that the
above copyright notice and this permission notice appear in all
copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
