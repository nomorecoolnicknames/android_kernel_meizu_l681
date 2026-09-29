# Meizu M3 Note L681 · Linux 3.10.72

Legacy kernel sources for the **Meizu M3 Note L681 / MT6755**, used by the
`cm-14.1` Android and recovery development track. This is the original 3.10
source family; it is separate from the newer M681/L681 3.18 and 4.x porting work.
L681 and M681 board resources and modem firmware are not interchangeable.

## Hardware and source map

The selections below are read from [`l681_defconfig`](arch/arm64/configs/l681_defconfig).
They list source support and requested options. This documentation update has
not generated a configuration, compiled this branch or tested it on a phone.

| Component | Source implementation | Defconfig selection | Compiled / working status |
|---|---|---|---|
| Display panels / bias | [LCM drivers](drivers/misc/mediatek/lcm) | ILI9885, HX8399 and NT35596 panel variants; TPS65132 helper | Source selected; panel variant and runtime unverified |
| Goodix touch variant | [GT9XX_MZ](drivers/input/touchscreen/mediatek/GT9XX_MZ) | `TOUCHSCREEN_MTK_GT9XX_MZ=y` | Source selected; board probe and payload unverified |
| FocalTech touch variant | [FT5436](drivers/input/touchscreen/mediatek/ft5436_zal1518) | `TOUCHSCREEN_MTK_FT5436_ZAL1518=y` | Source selected; variant and runtime unverified |
| GPU | [MediaTek Mali platform sources](drivers/misc/mediatek/gpu) | `MTK_GPU_SUPPORT=y` | Source selected; rendering and DVFS unverified |
| Cameras | [Image-sensor drivers](drivers/misc/mediatek/imgsensor) | OV13853 supplier variants, S5K5E8YX and OV5670 | Source selected; sensor population and pipeline unverified |
| PMIC / charging | [MT6755 power drivers](drivers/misc/mediatek/power/mt6755), [BQ24196](drivers/misc/mediatek/power/mt6755/bq24196.c) | `MTK_PMIC=y`, `MTK_PMIC_WRAP=y`, `MTK_BQ24196_SUPPORT=y` | Source selected; battery / thermal policy unverified |
| Audio | [MediaTek ASoC](sound/soc/mediatek) | `MT_SND_SOC_6755=y` | Source selected; playback / capture / call routes unverified |
| Modem transport | [ECCCI](drivers/misc/mediatek/eccci) | `MTK_ECCCI_DRIVER=y` | Requires matching L681 modem firmware and Android RIL |
| Wi-Fi / Bluetooth | [Connectivity drivers](drivers/misc/mediatek/connectivity) | `CONSYS_6755`, `MTK_COMBO_WIFI=y` | Source selected; firmware / radio tests needed |
| Storage | [MT6755 MMC host](drivers/misc/mediatek/mmc-host/mt6755) | Board-specific host resources | Build selection, I/O and suspend unverified |
| USB | [MediaTek USB 2.0](drivers/misc/mediatek/usb20) | `USB_MTK_DUALMODE=y`, `USB_MTK_HDRC_GADGET=y` | Source selected; device / host roles unverified |

“Source selected” means the defconfig requests the driver; it does not establish
that the object was built, bound to hardware or tested successfully.

## Build inputs

Select [`l681_defconfig`](arch/arm64/configs/l681_defconfig) for the Android kernel
or [`l681-twrp_defconfig`](arch/arm64/configs/l681-twrp_defconfig) for the recovery
configuration. Use `ARCH=arm64`, a compatible AArch64 Android GCC toolchain and
a separate Kbuild output directory. No exact toolchain or reproducible build
receipt is pinned by this branch's current documentation.

Board generation inputs, the matching L681 ramdisk and boot-image geometry are
needed to package an image. Consult the retained upstream [README](README) and
[Documentation](Documentation) for the Linux build system. Android vendor
components and device firmware are separate from these kernel sources.

## Credits and licensing

Based on the work of the Linux, Android, MediaTek, Meizu, LineageOS and recovery
communities. Original copyright notices, author history and per-file licenses
are retained. See [COPYING](COPYING); firmware and vendor inputs may have
different licensing and redistribution requirements.

[ReMeizu project status](https://github.com/nomorecoolnicknames/remeizu/blob/main/PROJECT_STATUS.md)
