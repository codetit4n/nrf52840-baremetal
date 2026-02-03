# Universal Asynchronous Receiver/Transmitter with EasyDMA (UARTE) - nRF52840 baremetal

This project demonstrates a [Universal asynchronous receiver/transmitter with EasyDMA](https://docs.nordicsemi.com/bundle/ps_nrf52840/page/uarte.html)
implementation on the nRF52840-DK, written in pure baremetal.

### Commands

- Build:
  ```shell
  make
  ```
- Clean:
  ```shell
  make clean
  ```
- Flash the firmware using [nrfjprog](https://www.nordicsemi.com/Products/Development-tools/nRF-Command-Line-Tools/)
  on the [nRF52840 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF52840-DK)
  ```shell
  nrfjprog --recover
  nrfjprog --program build/uarte.elf --chiperase --verify --reset
  ```
