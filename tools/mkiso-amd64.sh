#!/bin/sh

PORT=amd64

LIMINE=vendor/limine
ISOROOT=build-${PORT}/isoroot
ISO=build-${PORT}/azalea-${PORT}-limine.iso

mkdir -p "${ISOROOT}" &&
	cp build-${PORT}/kernel/symbols.map build-${PORT}/kernel/azalea ${LIMINE}/limine-bios.sys ${LIMINE}/limine-bios-cd.bin ${LIMINE}/limine-uefi-cd.bin "${ISOROOT}" &&
	cp tools/limine-amd64.conf "${ISOROOT}/limine.conf" &&
	xorriso -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin -efi-boot-part --efi-boot-image \
		--protective-msdos-label \
		"${ISOROOT}" -o "${ISO}" &&
	./build-${PORT}/limine bios-install "${ISO}"
