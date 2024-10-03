.text
.global kentry

.extern limine_entry

kentry:
	# TODO: set stack guard
	jmp limine_entry
