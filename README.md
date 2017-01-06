- autoreconf --install
- ./configure
- make
- cp .libs/*.so ~/.local/lib/deadbeef
- cp .libs/*.la ~/.local/lib/deadbeef

Setup path in settings

Irssi: 
/alias np /exec -o cat np.txt
