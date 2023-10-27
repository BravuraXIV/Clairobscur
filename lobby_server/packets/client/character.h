#ifndef BRAVURA_EMU_PACKETS_CHARACTER_H
#define BRAVURA_EMU_PACKETS_CHARACTER_H

#include "../num_types.h"

namespace bravura
{

struct get_characters_packet
{
    static const u8 opcode = 0x03;
};

struct select_character_packet
{
    static const u8 opcode = 0x04;

    u64 sequence;
    u32 character_id;
    u32 unknown;
    u64 unknown2; // Mentionned as "ticket" in Project Meteor.
};

struct modify_character_packet
{
    static const u8 opcode = 0x0B;

    u64 sequence;
    u32 character_id;
    u32 person_type;
    u8  slot;
    u8  command;
    u16 world_id;
    u8  character_name[0x20];
    u8  character_info_encoded[0x190];
};

}

#endif
