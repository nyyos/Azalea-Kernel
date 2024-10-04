#!/bin/bash
source "$(dirname "$0")/common.sh"

PORT=amd64
OVMF_URL="https://github.com/limine-bootloader/edk2-ovmf-nightly/releases/latest/download"

qemu_command="${QEMU_EXE:=qemu-system-x86_64}"
print_command=0
uefi=0
qemu_mem="${QEMU_MEM:=256M}"
qemu_smp="${QEMU_SMP:=2}"
qemu_args=
iso="build-${PORT}/azalea-amd64-limine.iso"
ovmf="build-${PORT}/ovmf"

while getopts "kngspHuUQGq:10" optchar; do
	case $optchar in
	s) qemu_args="$qemu_args -serial stdio" ;;
	k) qemu_args="$qemu_args -enable-kvm -cpu host" ;;
	n) qemu_args="$qemu_args -drive file=test.img,if=none,id=nvm -device nvme,serial=deadbeef,drive=nvm" ;;
	p) qemu_args="${qemu_args} -S" ;;
	q) qemu_args="${qemu_args} ${OPTARG}" ;;
	Q) print_command=1 ;;
	g) qemu_args="$qemu_args -M smm=off -d int -D qemulog.txt" ;;
	G) qemu_args="$qemu_args -nographic" 
		echo "!! Exit QEMU with Ctrl+A then X"
		;;
	u) iso="build-${PORT}/azalea-${PORT}-hyper.iso" ;;
	U) uefi=1 ;;
	H) qemu_args="$qemu_args -machine hpet=off" ;;
	*) exit 1 ;;
	esac
done

if [[ $uefi -eq 1 ]]; then
	mkdir -p "$ovmf"
	fetch "$ovmf/ovmf-code-${PORT}.fd" "${OVMF_URL}/ovmf-code-x86_64.fd"
	fetch "$ovmf/ovmf-vars-${PORT}.fd" "${OVMF_URL}/ovmf-vars-x86_64.fd"
	qemu_args="$qemu_args -drive if=pflash,unit=0,format=raw,file=$ovmf/ovmf-code-${PORT}.fd,readonly=on"
	qemu_args="$qemu_args -drive if=pflash,unit=1,format=raw,file=$ovmf/ovmf-vars-${PORT}.fd"
fi

qemu_args="$qemu_args -s"
qemu_args="$qemu_args -no-shutdown -no-reboot"
qemu_args="$qemu_args -cdrom ${iso}"
qemu_args="$qemu_args -smp $qemu_smp"
qemu_args="$qemu_args -m $qemu_mem"
qemu_args="$qemu_args -M q35"

if [[ $print_command -eq 1 ]]; then
	echo "${qemu_command} ${qemu_args}"
	exit
fi
${qemu_command} ${qemu_args}
