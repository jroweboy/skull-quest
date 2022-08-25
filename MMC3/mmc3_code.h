
#ifndef _MMC3_CODE_H
#define _MMC3_CODE_H

// Workaround for code analyzers that don't know about fastcall
#ifndef __fastcall__
#define __fastcall__ 
#endif

// Contains functions to help with working with multiple PRG/CHR banks
// For MMC3 code.

// Helper method for #pragma wrapped-call to call banked routines.
// this shouldn't need to be called directly. Instead just wrap the function declaration in the pragma as follows
// Example:
/*
    #pragma wrapped-call(push, bank_trampoline, bank)
    void my_banked_function(unsigned char parameters, const void* are_allowed);
    #pragma wrapped-call(pop)
*/
// Then you can just call the function like normal and it will bank through this tramponline code if needed.
// my_banked_function(1, fn);
void bank_trampoline(void);

// macro that uses the bank information provided by the linker to lookup a bank for a symbol
#define GET_BANK(sym) (__asm__("ldx #0\nlda #<.bank(%v)", sym), __A__)

// Helper macro to run a block of code with a specific bank loaded and then switch back after it has finished
#define RUN_WITH_BANK(bank_num, block) \
    do {\
        unsigned char saved_bank = current_code_bank;\
        bank_code(bank_num);\
        block;\
        bank_code(saved_bank);\
    } while(0);

// Readonly value of the current code bank. This is defined as either the $a000 bank or the $8000 bank depending on
// whether you are banking DPCM or not
extern unsigned char const volatile current_code_bank;

// Unconditionally switches the current code bank.
// CAUTION: if you call this while running code from the code bank, then it will CRASH
// Use the wrapped-call bank trampoline to be safe when you need to run code from a different bank
void __fastcall__ bank_code(unsigned char bank_id);

// The following two methods can be used for manually switching the banks if needed
// The preferred way is to use the bank_trampoline by wrapping your calls instead.
#ifdef USE_BANKABLE_DPCM
// Switch to the given bank (at $a000-bfff). Your prior bank is not saved.
// Can be used for reading data with a function in the fixed bank.
// bank_id: The bank to switch to.
void __fastcall__ set_prg_a000(unsigned char bank_id);

// WARNING, DON'T USE THIS IN THE CURRENT CFG, unless you
// really know what you're doing. El Crasho.
// Switch to the given bank (at $c000-dfff). Your prior bank is not saved.
// bank_id: The bank to switch to.
void __fastcall__ set_prg_c000(unsigned char bank_id);

// Readonly value of the current bank number for the prg $c000 bank
extern unsigned char const volatile current_prg_c000;

#else
// Switch to the given bank (at $8000-9fff). Your prior bank is not saved.
// Can be used for reading data with a function in the fixed bank.
// bank_id: The bank to switch to.
void __fastcall__ set_prg_8000(unsigned char bank_id);

// WARNING, DON'T USE THIS IN THE CURRENT CFG, unless you
// really know what you're doing. El Crasho.
// Switch to the given bank (at $a000-bfff). Your prior bank is not saved.
// bank_id: The bank to switch to.
void __fastcall__ set_prg_a000(unsigned char bank_id);

// Readonly value of the current bank number for the prg $8000 bank
extern unsigned char const volatile current_prg_8000;
#endif

// Readonly value of the current bank number for the prg $a000 bank
extern unsigned char const volatile current_prg_a000;

// Changes a portion of the tilsets
void __fastcall__ set_chr_mode_0(unsigned char chr_id);
void __fastcall__ set_chr_mode_1(unsigned char chr_id);
void __fastcall__ set_chr_mode_2(unsigned char chr_id);
void __fastcall__ set_chr_mode_3(unsigned char chr_id);
void __fastcall__ set_chr_mode_4(unsigned char chr_id);
void __fastcall__ set_chr_mode_5(unsigned char chr_id);


#define MIRROR_VERTICAL 0
#define MIRROR_HORIZONTAL 1

// Set the current mirroring mode. Your options are 
// MIRROR_HORIZONTAL, and MIRROR_VERTICAL.
void __fastcall__ set_mirroring(unsigned char mirroring);


#define WRAM_OFF 0x40
#define WRAM_ON 0x80
#define WRAM_READ_ONLY 0xC0

// Set the WRAM mode. Your options are 
// WRAM_OFF, WRAM_ON, and WRAM_READ_ONLY.
// May not work in some emulators. Init code turns it ON.
void __fastcall__ set_wram_mode(unsigned char mode);


// Turns off MMC3 irqs, and changes the array pointer
// to point to a default 0xff
void disable_irq(void);


// This points an array to the IRQ system 
// Also turns ON the system
void set_irq_ptr(char * address);


// Check if it's safe to write to the irq array
// The value will be 0xff if done, zero if not done
// if the irq pointer is pointing to 0xff it is
// safe to edit.
extern unsigned char const volatile irq_done;

#endif // _MMC3_CODE_H
