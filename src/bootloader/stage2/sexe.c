/* sexe.c
 *
 * Implementation of the .sexe loader.
 *
 * Link this into your stage2 build (freestanding flags: -ffreestanding -fno-builtin).
 * Provide a reader function that reads file bytes from disk/FAT and pass it in.
 */

#include "sexe.h"
#include "memory.h" 

/* Minimal types - rely on your stdint.h from the tree */
typedef unsigned char  u8;
typedef unsigned int   u32;


/* Small CRC32 (polynomial 0xEDB88320) */
static u32 crc32_compute_chunk(const void *data, u32 len, u32 crc_in) {
    const u8 *p = (const u8*)data;
    u32 crc = ~crc_in;
    for (u32 i = 0; i < len; ++i) {
        crc ^= p[i];
        for (int j = 0; j < 8; ++j) {
            u32 mask = (u32)(-(int)(crc & 1));
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }
    return ~crc;
}

/* read header helper */
static int read_validate_header(const char *path, sexe_read_fn reader, struct sexe_header *out_h, void *scratch, u32 scratch_size) {
    if (!reader) return SEXE_ERR_IO;
    if (!scratch || scratch_size < (u32)sizeof(struct sexe_header)) return SEXE_ERR_NO_SCRATCH;

    int32_t got = reader(path, 0, scratch, (uint32_t)sizeof(struct sexe_header));
    if (got < (int32_t)sizeof(struct sexe_header)) return SEXE_ERR_IO;

    /* copy into struct (unaligned-safe) */
    memcpy(out_h, scratch, (u32)sizeof(struct sexe_header));

    if (out_h->magic != 0x45584553u) return SEXE_ERR_BAD_MAGIC; /* 'SEXE' */
    if (out_h->payload_size == 0) return SEXE_ERR_BAD_HEADER;
    /* simple sanity cap (adjust as needed) */
    if (out_h->payload_size > 0x04000000u) return SEXE_ERR_PAYLOAD_TOO_LARGE; /* 64 MB */

    return SEXE_OK;
}

/* Stream payload into memory, compute/verify CRC if present */
static int stream_payload(const char *path,
                          sexe_read_fn reader,
                          u32 payload_file_offset,
                          u32 payload_size,
                          u32 load_addr,
                          void *scratch,
                          u32 scratch_size,
                          u32 expected_crc)
{
    if (!reader) return SEXE_ERR_IO;
    if (!scratch || scratch_size == 0) return SEXE_ERR_NO_SCRATCH;

    u32 remaining = payload_size;
    u32 file_off = payload_file_offset;
    u8 *dst = (u8*)(uintptr_t)load_addr;
    u32 running_crc = 0;

    while (remaining) {
        u32 to_read = (remaining > scratch_size) ? scratch_size : remaining;
        int32_t got = reader(path, file_off, scratch, to_read);
        if (got <= 0) return SEXE_ERR_IO;

        memcpy(dst, scratch, (u32)got);

        if (expected_crc != 0) {
            running_crc = crc32_compute_chunk(scratch, (u32)got, running_crc);
        }

        dst += (u32)got;
        file_off += (u32)got;
        remaining -= (u32)got;
    }

    if (expected_crc != 0 && running_crc != expected_crc) return SEXE_ERR_CHKSUM;
    return SEXE_OK;
}

/* Public loader */
int sexe_load_with_reader(const char *path,
                          void **out_entry,
                          sexe_read_fn reader,
                          void *scratch,
                          uint32_t scratch_size)
{
    if (out_entry) *out_entry = 0;
    if (!path || !reader) return SEXE_ERR_IO;
    if (!scratch || scratch_size < 512) return SEXE_ERR_NO_SCRATCH;

    struct sexe_header hdr;
    int r = read_validate_header(path, reader, &hdr, scratch, scratch_size);
    if (r != SEXE_OK) return r;

    /* payload begins immediately after header */
    uint32_t payload_file_offset = (uint32_t)sizeof(struct sexe_header);

    /* optional alignment requirement: require 4-byte align for load_addr */
    if (hdr.load_addr & 0x3u) return SEXE_ERR_UNALIGNED;

    r = stream_payload(path, reader, payload_file_offset, hdr.payload_size, hdr.load_addr, scratch, scratch_size, hdr.crc32);
    if (r != SEXE_OK) return r;

    /* compute entry pointer and return it */
    void *entry = (void*)(uintptr_t)(hdr.load_addr + hdr.entry_offset);
    if (out_entry) *out_entry = entry;
    return SEXE_OK;
}
