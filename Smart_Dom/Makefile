all:
	cmake -B build -GNinja -DBOARD=nucleo_f103rb
	ninja -C build
	renode --execute "s @renode/macros.resc"

sim:
	renode --execute "s @renode/macros.resc"

remake:
	cmake -B build -GNinja -DBOARD=nucleo_f103rb
	ninja -C build

.PHONY: all renode
