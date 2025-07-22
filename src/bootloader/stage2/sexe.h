typedef struct {
    char magic[4];          // "SEXE"
    uint8_t version;        // 0x01
    uint8_t reserved[3];    // padding
    uint32_t entry_point;   // offset into code
    uint32_t code_size;
    uint32_t data_size;
    // Followed by: code[code_size], data[data_size]
} __attribute__((packed)) SexeHeader;
