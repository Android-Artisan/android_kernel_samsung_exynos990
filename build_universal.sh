#!/bin/bash

SOURCE_DIR="build/out/all/zip"
UNIVERSAL_DIR="build/universal"
OUTPUT_DIR="$(realpath "$UNIVERSAL_DIR/output")"
TMP_DIR="tmp_universal_all"

# Clean and recreate
rm -rf "$TMP_DIR"
mkdir -p "$TMP_DIR/files"
mkdir -p "$TMP_DIR/META-INF/com/google/android"
mkdir -p "$OUTPUT_DIR"

# Copy required TWRP files
cp "$UNIVERSAL_DIR/update-binary" "$TMP_DIR/META-INF/com/google/android/" || { echo "❌ Missing update-binary"; exit 1; }
cp "$UNIVERSAL_DIR/updater-script" "$TMP_DIR/META-INF/com/google/android/" || { echo "❌ Missing updater-script"; exit 1; }

# Process ZIPs
for zipfile in "$SOURCE_DIR"/*.zip; do
    filename=$(basename "$zipfile")
    model=$(echo "$filename" | grep -oP '_\K[a-z0-9]+(?=_UNOFFICIAL)')

    echo "Processing: $filename (Model: $model)"

    unzip_dir="tmp_extracted_$model"
    mkdir -p "$unzip_dir"

    if ! unzip -qq "$zipfile" -d "$unzip_dir"; then
        echo "❌ Failed to unzip $zipfile"
        rm -rf "$unzip_dir"
        continue
    fi

    [ -f "$unzip_dir/files/boot.img" ] && cp "$unzip_dir/files/boot.img" "$TMP_DIR/files/boot${model}.img"
    [ -f "$unzip_dir/files/dtbo.img" ] && cp "$unzip_dir/files/dtbo.img" "$TMP_DIR/files/dtbo${model}.img"

    rm -rf "$unzip_dir"
done

# Final ZIP
pushd "$TMP_DIR" > /dev/null
zip -r -qq "$OUTPUT_DIR/ArtisanKRNL-v0.0.5_990_UNOFFICIAL_KernelSU_NEXT.zip" . || { echo "❌ Failed to create universal zip"; exit 1; }
popd > /dev/null

rm -rf "$TMP_DIR"

echo "-----------------------------------------------------"
echo " ✅ All-in-one TWRP flashable zip created:"
echo "     → $OUTPUT_DIR/ArtisanKRNL-v0.0.5_990_UNOFFICIAL_KernelSU_NEXT.zip"
echo "-----------------------------------------------------"
