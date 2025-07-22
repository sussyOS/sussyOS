from elftools.elf.elffile import ELFFile
import struct
import sys

def extract_section(elf, name):
    section = elf.get_section_by_name(name)
    if section:
        return section.data()
    return b''

def elf_to_sexe(elf_path, output_path):
    with open(elf_path, 'rb') as f:
        elf = ELFFile(f)

        entry_point = elf['e_entry']

        code = extract_section(elf, '.text')
        data = extract_section(elf, '.data') + extract_section(elf, '.rodata')

        # Create SEXE header
        magic = b'SEXE'
        version = 1
        reserved = b'\x00\x00\x00'
        entry_offset = 0  # Assume entry is at start of code
        code_size = len(code)
        data_size = len(data)

        header = struct.pack('<4sB3sIII', magic, version, reserved, entry_offset, code_size, data_size)

        with open(output_path, 'wb') as out:
            out.write(header)
            out.write(code)
            out.write(data)

        print(f"[+] .sexe file written to {output_path}")
        print(f"    Code: {code_size} bytes")
        print(f"    Data: {data_size} bytes")
        print(f"    Entry point: 0x{entry_point:X}")

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python3 elf2sexe.py input.elf output.sexe")
        sys.exit(1)

    elf_to_sexe(sys.argv[1], sys.argv[2])
