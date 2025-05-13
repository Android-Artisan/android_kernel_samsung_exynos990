#!/bin/bash

set -e

# ASCII Banner for Artisan Kernel
cat << "EOF"
========================================================================
                                                                        
     ___           __   _                      __ __  ____   _   __ __  
    /   |   _____ / /_ (_)_____ ____ _ ____   / //_/ / __ \ / | / // /  
   / /| |  / ___// __// // ___// __ '// __ \ / ,<   / /_/ //  |/ // /   
  / ___ | / /   / /_ / /(__  )/ /_/ // / / // /| | / _, _// /|  // /___ 
 /_/  |_|/_/    \__//_//____/ \__,_//_/ /_//_/ |_|/_/ |_|/_/ |_//_____/ 
                                                                        
            A R T I S A N   K E R N E L   V 0 . 0 . 4                   
========================================================================

EOF

# Function: Check for existing repo or clone it
prepare_repo() {
    echo "--------------------------------------------------------------------"
    echo " Searching for existing 'android_kernel_samsung_exynos990' folder..."
    echo "--------------------------------------------------------------------"
    echo " "
    REPO_PATH=$(find "$HOME" -type d -name "android_kernel_samsung_exynos990" 2>/dev/null | head -n 1)

    if [ -n "$REPO_PATH" ]; then
        echo "--------------------------------------------------------------------"
        echo " Found existing repository at: $REPO_PATH"
        echo "--------------------------------------------------------------------"
        echo " "
        cd "$REPO_PATH"
    else
        echo "--------------------------------------------------------------------"
        echo " Repository not found. Cloning from GitHub..."
        echo "--------------------------------------------------------------------"
        echo " "
        git clone --recurse-submodules https://github.com/Android-Artisan/android_kernel_samsung_exynos990.git
        cd android_kernel_samsung_exynos990
    fi
}

# Function: Prompt for single device build
build_individual() {
    echo "--------------------------------------------------------------------"
    echo " Available device codenames:"
    echo " x1slte x1s y2slte y2s z3s c1slte c1s c2slte c2s r8s"
    echo "--------------------------------------------------------------------"
    echo " "
    read -p " Enter the device codename you want to build for: " codename

    if [[ ! " x1slte x1s y2slte y2s z3s c1slte c1s c2slte c2s r8s " =~ " ${codename} " ]]; then
        echo "--------------------------------------------------------------------"
        echo " Invalid codename. Exiting."
        echo "--------------------------------------------------------------------"
        exit 1
    fi

    chmod +x build.sh
    ./build.sh -m "$codename" -k y -r n
}

# Function: Build all devices
build_all() {
    chmod +x build_all.sh
    ./build_all.sh

    read -p " Do you want to build a universal TWRP install zip for all devices? (y/n): " build_zip
    if [[ "$build_zip" == "y" || "$build_zip" == "Y" ]]; then
        chmod +x build_universal.sh
        ./build_universal.sh
    fi
}

### Main Script ###
prepare_repo

echo ""
echo " Choose build mode:"
echo " 1) Build for individual device"
echo " 2) Build for all devices"
read -p " Enter your choice (1 or 2): " choice

if [ "$choice" == "1" ]; then
    build_individual
elif [ "$choice" == "2" ]; then
    build_all
else
    echo "Invalid choice. Exiting."
    exit 1
fi
