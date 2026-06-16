# Power Shield ADC Sample

This repository now carries the Zephyr ADC power measurement sample from
`zephyrproject-rtos/zephyr` PR #105807, adapted to live in this repository's
`app/` directory.

## What is included

- ADC power measurement application in `app/`
- Board overlays/config fragments for:
  - `frdm_k64f`
  - `frdm_mcxc242`
  - `frdm_mcxc444`
  - `mimxrt1040_evk`
- Sample documentation in `app/README.rst`
- Twister metadata in `app/sample.yaml`

## Build

From a Zephyr workspace with this repository available as the application:

```sh
west build -b frdm_mcxc444/mcxc444 app
```

To try another supported board, replace the board name with one of the boards
covered by the overlay/config files in `app/boards`.

## CI coverage

The application is included in CI through:

```sh
west twister -T app --integration
```

The current integration platforms for this sample are:

- `frdm_mcxc444`
- `mimxrt1040_evk`

## Notes

The application content was moved from the upstream Zephyr sample path:
`samples/drivers/adc/adc_power_measure`.
