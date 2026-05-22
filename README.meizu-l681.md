# Android kernel tree for Meizu M3 Note / l681

Kernel source tree for the Meizu M3 Note L681/HQ6755 MT6755 LineageOS/TWRP bring-up path.

## Device scope

| Item | Value |
| --- | --- |
| Device family | Meizu M3 Note / L681 class |
| Platform | MediaTek MT6755 / Helio P10 |
| Android target | LineageOS 14.1 and recovery bring-up |
| Active branch | `cm-14.1` locally; pushed as `l681-bringup-cm-14.1` for this build-station snapshot |
| Current focus | stock EINT/DWS parity, legacy build compatibility, and recovery/ROM kernel reuse |

## Maintainer notes

Stock Flyme hardware data wins over donor board data when GPIO/EINT/PMIC/display/touch mappings disagree. Keep build products and device captures out of this repository.

## Credits

Thanks to Linus Torvalds and all Linux kernel contributors, Android kernel maintainers, MediaTek platform authors, Meizu stock-kernel authors, LineageOS/CyanogenMod maintainers, recovery developers, and all public MTK bring-up maintainers whose work made this tree possible.
