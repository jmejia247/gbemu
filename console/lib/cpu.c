#include <cpu.h>
#include <bus.h>
#include <emu.h>
#include <interrupts.h>
#include <timer.h>

cpu_context ctx = {0};

void cpu_init() {
    ctx.regs.pc = 0x100;
    ctx.regs.sp = 0xFFFE;
    *((short *)&ctx.regs.a) = 0xB001;
    *((short *)&ctx.regs.b) = 0x1300;
    *((short *)&ctx.regs.c) = 0xD800;
    *((short *)&ctx.regs.d) = 0x4D01;
    ctx.ie_register = 0;
    ctx.int_flags = 0;
    ctx.int_master_enabled = false;
    ctx.enabling_ime = false;

    timer_get_context()->div = 0xABCC;
}

static void fetch_instruction() {
    ctx.cur_opcode = bus_read(ctx.regs.pc++);
    ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);
}

void fetch_data();

static void execute() {
    // printf("Not executing yet... \n");
    IN_PROC proc = inst_get_processor(ctx.cur_inst->type);

    if (!proc) {
        NO_IMPL
    }

    proc(&ctx);
}

bool cpu_step() {
    
    if (!ctx.halted) {
        printf("Instruction started...\n");
        u16 pc = ctx.regs.pc;

        printf("fetching instruction...\n");
        fetch_instruction();
        printf("Instruction: %02X fetched,\nfetching data...\n", ctx.cur_opcode);
        ASSERT(ctx.cur_inst != NULL, ctx);
        fetch_data();
        printf("Data fetched\n");

        
        char flags[16];
        sprintf(flags, "%c%c%c%c", 
            ctx.regs.f & (1 << 7) ? 'Z' : '-',
            ctx.regs.f & (1 << 6) ? 'N' : '-',
            ctx.regs.f & (1 << 5) ? 'H' : '-',
            ctx.regs.f & (1 << 4) ? 'C' : '-'
        );

        char inst[16];
        inst_to_str(&ctx, inst);

        printf("%08llX - %04X: %-12s (%02X %02X %02X) A: %02X BC: %02X%02X DE: %02X%02X HL: %02X%02X\n", 
            emu_get_context()->ticks,
            pc, 
            inst, 
            ctx.cur_opcode,
            bus_read(pc + 1), 
            bus_read(pc + 2), 
            ctx.regs.a, 
            ctx.regs.b, 
            ctx.regs.c,
            ctx.regs.d, 
            ctx.regs.e, 
            ctx.regs.h, 
            ctx.regs.l
        );

        if (ctx.cur_inst == NULL) {
            printf("Unknown Instruction! %02X\n", ctx.cur_opcode);
            exit(-7);
        }

        execute();
        printf("Instruction executed\n -------------------------end of instruction------------------------\n");
    } else {
        // halted true
        emu_cycles(1);

        if (ctx.int_flags) {
            ctx.halted = false;
        }
    }

    if (ctx.int_master_enabled) {
        // pass the pointer to the context
        cpu_handle_interrupts(&ctx);
        ctx.enabling_ime = false;
    }

    if (ctx.enabling_ime) {
        ctx.int_master_enabled = true;
    }

    return true;
}

u8 cpu_get_ie_register() {
    return ctx.ie_register;
}

void cpu_set_ie_register(u8 n) {
    ctx.ie_register = n;
}

void cpu_request_interrupt(interrupt_type t) {
    ctx.int_flags |= t;
}
