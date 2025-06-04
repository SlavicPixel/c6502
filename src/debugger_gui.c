#include "debugger_gui.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void step_instruction(Cpu *cpu, Bus *bus)
{   
    do
    {
        cpu_clock(cpu, bus);
    } while (cpu->cycles != 0);
    printf("PC: %04X  OPCODE: %02X  A:%02X X:%02X Y:%02X  STATUS:%02X\n",
        cpu->pc, cpu->opcode, cpu->accumulator, cpu->x_register, cpu->y_register, cpu->status);
}

void gui_debugger(Cpu *cpu, Bus *bus, uint16_t start_address, Line *first_entry)
{
    InitWindow(1200, 800, "6502 Debugger");
    SetTargetFPS(60);
    Font font = LoadFont("C64_Pro_Mono-STYLE.ttf");

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE)) {
            step_instruction(cpu, bus);
        }

        if (IsKeyPressed(KEY_R)) {
            reset(cpu, bus);
        }

        if (IsKeyPressed(KEY_I)) {
            irq(cpu, bus);
        }

        if (IsKeyPressed(KEY_N)) {
            nmi(cpu, bus);
        }

        BeginDrawing();
            ClearBackground(DARKBLUE);

        for (int row = 0; row < 16; row++) {
            int y = 10 + row * 20;

            // Print row label: $00X0:
            char label[10];
            snprintf(label, sizeof(label), "$%04X:", row * 0x10);
            DrawTextEx(font, label, (Vector2){10, y}, 15, 0, WHITE);

            for (int col = 0; col < 16; col++) {
                int x = 110 + col * 40;
                uint8_t value = bus->ram[row * 16 + col];

                char byteStr[4];
                snprintf(byteStr, sizeof(byteStr), "%02X", value);
                DrawTextEx(font, byteStr, (Vector2){x, y}, 15, 0, WHITE);
            }
        }
        // Offset below zero page
        int y_offset = 10 + 16 * 20 + 40;  // 16 rows + padding between regions

        // Draw Page $4000 ($4000–$40FF)
        for (int row = 0; row < 16; row++) {
            int y = y_offset + row * 20;

            uint16_t base_addr = start_address + row * 0x10;
            char label[10];
            snprintf(label, sizeof(label), "$%04X:", base_addr);
            DrawTextEx(font, label, (Vector2){10, y}, 15, 0, WHITE);

            for (int col = 0; col < 16; col++) {
                int x = 110 + col * 40;
                uint8_t value = bus->ram[base_addr + col];

                char byteStr[4];
                snprintf(byteStr, sizeof(byteStr), "%02X", value);
                DrawTextEx(font, byteStr, (Vector2){x, y}, 15, 0, WHITE);
            }
        }

        DrawTextEx(font, "STATUS: ", (Vector2){90 + 17 * 40 , 10 + 0 * 20}, 15, 0, WHITE);
        DrawTextEx(font, "N ", (Vector2){10 + 70 + 20 * 40 , 10 + 0 * 20}, 15, 0, get_flag(cpu, N) ? GREEN : RED);
        DrawTextEx(font, "V ", (Vector2){10 + 100 + 20 * 40 , 10 + 0 * 20}, 15, 0, get_flag(cpu, V) ? GREEN : RED);
        DrawTextEx(font, "U ", (Vector2){10 + 130 + 20 * 40 , 10 + 0 * 20}, 15, 0, WHITE);
        DrawTextEx(font, "B ", (Vector2){10 + 160 + 20 * 40 , 10 + 0 * 20}, 15, 0, get_flag(cpu, B) ? GREEN : RED);
        DrawTextEx(font, "D ", (Vector2){10 + 190 + 20 * 40 , 10 + 0 * 20}, 15, 0, get_flag(cpu, D) ? GREEN : RED);
        DrawTextEx(font, "I ", (Vector2){10 + 220 + 20 * 40 , 10 + 0 * 20}, 15, 0, get_flag(cpu, I) ? GREEN : RED);
        DrawTextEx(font, "Z ", (Vector2){10 + 250 + 20 * 40 , 10 + 0 * 20}, 15, 0, get_flag(cpu, Z) ? GREEN : RED);
        DrawTextEx(font, "C ", (Vector2){10 + 280 + 20 * 40 , 10 + 0 * 20}, 15, 0, get_flag(cpu, C) ? GREEN : RED);


        char pc_text[16];
        snprintf(pc_text, sizeof(pc_text), "PC: $%04X", cpu->pc);
        DrawTextEx(font, pc_text, (Vector2){90 + 17 * 40 , 10 + 1 * 20}, 15, 0, WHITE);

        char acc_text[16];
        snprintf(pc_text, sizeof(acc_text), "A:  $%02X [%d]", cpu->accumulator, cpu->accumulator);
        DrawTextEx(font, pc_text, (Vector2){90 + 17 * 40 , 10 + 2 * 20}, 15, 0, WHITE);

        char x_text[16];
        snprintf(x_text, sizeof(x_text), "X:  $%02X [%d]", cpu->x_register, cpu->x_register);
        DrawTextEx(font, x_text, (Vector2){90 + 17 * 40 , 10 + 3 * 20}, 15, 0, WHITE);

        char y_text[16];
        snprintf(y_text, sizeof(y_text), "Y:  $%02X [%d]", cpu->y_register, cpu->y_register);
        DrawTextEx(font, y_text, (Vector2){90 + 17 * 40 , 10 + 4 * 20}, 15, 0, WHITE);

        char sp_text[25];
        snprintf(sp_text, sizeof(sp_text), "Stack Pointer: $%04X", cpu->stack_pointer);
        DrawTextEx(font, sp_text, (Vector2){90 + 17 * 40 , 10 + 5 * 20}, 15, 0, WHITE);

        Line *entry = first_entry;

        int i = 7, j = 0;
        while (entry->hh.next && j <= 26)
        {
            DrawTextEx(font, entry->line, (Vector2){90 + 17 * 40 , 10 + i++ * 20}, 15, 0, entry->address == cpu->pc ? (Color){ 0, 200, 241, 255 } : WHITE);
            entry = entry->hh.next;
            j++;
        }

        DrawTextEx(font, "SPACE = Step Instruction   R = RESET   I = IRQ    N = NMI", (Vector2){25, 360 + 18 * 20}, 15, 0, WHITE);
        
        DrawTextEx(font, "6502 Debugger", (Vector2){1065, 780}, 10, 0, WHITE);

        EndDrawing();
    }

    CloseWindow();
}
