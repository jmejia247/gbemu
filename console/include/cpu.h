#pragma once

#include <common.h>
#include <instructions.h>
#include <emu.h>
#include <bus.h>

typedef struct {
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;

} cpu_registers;

typedef struct {
    cpu_registers regs;
    u16 fetched_data;
    u16 mem_dest;
    bool dest_is_mem;
    u8 cur_opcode;
    instruction *cur_inst;
    bool halted;
    bool stepping;
    bool int_master_enabled;
    bool enabling_ime;
    u8 ie_register;
    u8 int_flags;
} cpu_context;

cpu_registers *cpu_get_regs();

void cpu_init();
bool cpu_step();

typedef void (*IN_PROC)(cpu_context *);

IN_PROC inst_get_processor(in_type type);

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_N BIT(ctx->regs.f, 6)
#define CPU_FLAG_H BIT(ctx->regs.f, 5)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)

u16 cpu_read_reg(reg_type rt);
void cpu_set_reg(reg_type rt, u16 val);

u8 cpu_get_ie_register();
void cpu_set_ie_register(u8 n);

u8 cpu_read_reg8(reg_type rt);
void cpu_set_reg8(reg_type rt, u8 val);

u8 cpu_get_int_flags();
void cpu_set_int_flags(u8 value);

void inst_to_str(cpu_context *ctx, char *str);

#define DEBUG 

#ifndef DEBUG
    #define ASSERT(n, ctx);
#else
    #define ASSERT(n, ctx) \
    if (!(n)) { \
        char flags[16]; \
        sprintf(flags, "%c%c%c%c", \
            (ctx).regs.f & (1 <<  7) ? 'Z' : '-', \
            (ctx).regs.f & (1 <<  6) ? 'N' : '-', \
            (ctx).regs.f & (1 <<  5) ? 'H' : '-', \
            (ctx).regs.f & (1 <<  4) ? 'C' : '-' \
        ); \
        char inst[16]; \
        inst_to_str(&(ctx), inst); \
        \
        printf("ASSERT FAILED: %s\n", #n); \
        printf("On %s\n", __DATE__); \
        printf("At %s\n", __TIME__); \
        printf("In File %s\n", __FILE__); \
        printf("At Line %d\n", __LINE__); \
        printf("%08llX - %04X: %-12s (%02X %02X %02X) A: %02X BC: %02X%02X DE: %02X%02X HL: %02X%02X\n", \
            emu_get_context()->ticks, \
            (ctx).regs.pc, \
            inst, \
            (ctx).cur_opcode, \
            bus_read((ctx).regs.pc + 1), \
            bus_read((ctx).regs.pc + 2), \
            (ctx).regs.a, \
            (ctx).regs.b, \
            (ctx).regs.c, \
            (ctx).regs.d, \
            (ctx).regs.e, \
            (ctx).regs.h, \
            (ctx).regs.l \
        ); \
    }
#endif
