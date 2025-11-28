#ifndef SEXE_H
#define SEXE_H

/* sexe.h
 *
 * Minimal SEXE (.sexe) on-disk format + loader API for sussyOS.
 *
 * This header is freestanding-friendly (no stdio/malloc). The loader
 * uses a caller-supplied read callback so it integrates with your
 * existing disk/FAT implementation (stage2).
 *
 * Filename extension: .sexe
 */

#include "stdint.h"

/* Return codes */
#define SEXE_OK                    0
#define SEXE_ERR_IO                1
#define SEXE_ERR_BAD_MAGIC         2
#define SEXE_ERR_BAD_HEADER        3
#define SEXE_ERR_PAYLOAD_TOO_LARGE 4
#define SEXE_ERR_CHKSUM            5
#define SEXE_ERR_NO_SCRATCH        6
#define SEXE_ERR_UNALIGNED         7

/* SEXE header layout (24 bytes) - little endian */
struct sexe_header {
    uint32_t magic;        /* 'SEXE' = 0x45584553 */
    uint32_t entry_offset; /* offset from load_addr to entry point (bytes) */
    uint32_t load_addr;    /* physical address where payload will be copied */
    uint32_t payload_size; /* size in bytes of the payload */
    uint32_t flags;        /* reserved (alignment, flags) */
    uint32_t crc32;        /* CRC32 over payload (0 disables check) */
} __attribute__((packed));

/* Reader callback:
 *   path: file identifier (e.g. "KERNEL.SEXE" or directory entry name)
 *   offset: file offset to read from
 *   buf/count: destination buffer and requested byte count
 *
 * Return: number of bytes actually read (>=0) or negative on error.
 *
 * Implement this in your FAT/disk layer and pass as reader to the loader.
 */
typedef int32_t (*sexe_read_fn)(const char *path, uint32_t offset, void *buf, uint32_t count);

/* Load a .sexe file using the provided reader.
 *
 * path: file identifier/path for reader callback
 * out_entry: on success receives pointer to entry (hdr.load_addr + hdr.entry_offset)
 * reader: function to read file data
 * scratch: temporary buffer (>=512 recommended)
 * scratch_size: size of scratch buffer
 *
 * Returns SEXE_OK on success, otherwise a SEXE_ERR_* code.
 *
 * Caller must ensure:
 *  - load_addr is safe to write (does not clobber bootloader or stack)
 *  - if the payload expects a different CPU mode (protected/long), the caller
 *    performs the transition prior to invoking the entry.
 */
int sexe_load_with_reader(const char *path,
                          void **out_entry,
                          sexe_read_fn reader,
                          void *scratch,
                          uint32_t scratch_size);

#endif /* SEXE_H */
