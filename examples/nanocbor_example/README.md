### To build the code
```bash
make BOARD=arduino-nano-33-ble 
make BOARD=nrf52840dk
```
### Flash on board
```bash
make BOARD=arduino-nano-33-ble -C examples/nanocbor_example/ flash
make BOARD=nrf52840dk -C examples/nanocbor_example/ flash

```
### Terminal output on board
```bash
make BOARD=arduino-nano-33-ble -C examples/nanocbor_example/ term
make BOARD=nrf52840dk -C examples/nanocbor_example/ term

```

