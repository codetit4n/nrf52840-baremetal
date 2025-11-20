# blinky - nRF52840 baremetal

> [!IMPORTANT]
> Detailed article about this project coming soon on my [blog](https://loke.sh/blog).

<details>
    <summary>Demo</summary>
    <img src="assets/demo.gif" width="150px" />
</details>

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
