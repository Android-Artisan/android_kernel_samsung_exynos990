## Build instructions:

1. Set up build environment as per Google documentation

https://source.android.com/docs/setup/start/requirements

* The `libarchive-tools` package is also necessary to patch the toolchain.
* The `ccache` package is necessary if you wish to build with CCache (Quicker subsequest builds)

2. Properly clone repository with submodules (KernelSU and toolchains)

```git clone --recurse-submodules https://github.com/Android-Artisan/android_kernel_samsung_exynos990.git```

3. Build for your device without CCache and with KSU

```./build.sh -m c2slte -k y -c n```

4. Fetch the flashable zip of the kernel that was just compiled

```build/out/[your_device]/ArtisanKRNL...zip```

5. Flash it using a supported recovery like TWRP either using the install function or ADB Sideload

6. Enjoy!

Linux kernel
============

There are several guides for kernel developers and users. These guides can
be rendered in a number of formats, like HTML and PDF. Please read
Documentation/admin-guide/README.rst first.

In order to build the documentation, use ``make htmldocs`` or
``make pdfdocs``.  The formatted documentation can also be read online at:

    https://www.kernel.org/doc/html/latest/

There are various text files in the Documentation/ subdirectory,
several of them using the Restructured Text markup notation.
See Documentation/00-INDEX for a list of what is contained in each file.

Please read the Documentation/process/changes.rst file, as it contains the
requirements for building and running the kernel, and information about
the problems which may result by upgrading your kernel.
