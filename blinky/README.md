# blinky - nRF52840 baremetal

### Demo

<video width="320" height="240" controls>
  <source src="./assets/demo.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

### Commands

- Build:
  ```shell
  make
  ```
- Clean:
  ```shell
  make clean
  ```

### Convert to `uf2` using [`uf2conv`](https://github.com/makerdiary/uf2utils) tool:

Since, my nRF52840 uses the [Adafruit nRF52 bootloader](https://github.com/adafruit/Adafruit_nRF52_Bootloader),
I need to convert the generated ELF file to UF2 format using the following command:

```shell
uf2conv build/blinky.elf --family 0xADA52840 --output blinky.uf2
```
