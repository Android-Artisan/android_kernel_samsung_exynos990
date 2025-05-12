# ArtisanKernel v0.0.4 for Exynos 990 devices

## [![Build Android Kernel (All Devices)](https://github.com/Android-Artisan/android_kernel_samsung_exynos990/actions/workflows/build_all.yml/badge.svg)](https://github.com/Android-Artisan/android_kernel_samsung_exynos990/actions/workflows/build_all.yml)

Based off ExtremeXT's [990_upstream_v2](https://github.com/ExtremeXT/990_upstream_v2)

And a huge thank you to Windblade-Furisode

## About Kernel
- Linux 4.19.325
- Name "People's Front"
- Display Version "4.19.325-ArtisanKRNL-v0.0.4+"


## Features
- Auto Builds in 45-50 minutes for all devices with Github Actions thanks to Windblade-Furisode
- OneUI 7.x support
- HyperOS 2.x support
- KernelSU Next support
- SUSFS support (Kingslayer9988)
- Fully open source with a clean commit history

## Issues
- OTG (Issues with usb_rules)

## Windful (To Be Added)
- LineageOS support (USB ...etc...)
- Pulnice? (Eyeless's governor)
- Gpu overclocking (front & back end)

## Supported devices:

- G980F - S20 4G - x1slte

- G981B - S20 5G - x1s

- G985F - S20+ 4G - y2slte

- G986B - S20+ 5G - y2s

- G988B - S20 Ultra (5G) - z3s

- G780F - S20 FE - r8s

- N980F - Note 20 4G - c1slte

- N981B - Note 20 5G - c1s

- N985F - Note 20 Ultra 4G - c2slte

- N986B - Note 20 Ultra 5G - c2s

## Build using Google Colab (Might Will Not Work)

- [Start Building](https://colab.research.google.com/github/Android-Artisan/android_kernel_samsung_exynos990/blob/main/Build_ArtisanKRNL.ipynb)

## Build instructions:

1. Set up build environment as per Google documentation

https://source.android.com/docs/setup/start/requirements

* The `libarchive-tools` package is also necessary to patch the toolchain.
* The `ccache` package is necessary if you wish to build with CCache (Quicker subsequest builds)

2. Copy this script into any x86-64 Linux Distro

```bash
curl -fsSL -o kernelify.sh https://raw.githubusercontent.com/Android-Artisan/android_kernel_samsung_exynos990/refs/heads/main/kernelify.sh
```

3. Once the script is Downloaded, run

```bash
bash kernelify.sh
```


## Contributors

- Android Artisan
- Windblade-Furisode
- Kingslayer9988
- YacineGTI-DZ
